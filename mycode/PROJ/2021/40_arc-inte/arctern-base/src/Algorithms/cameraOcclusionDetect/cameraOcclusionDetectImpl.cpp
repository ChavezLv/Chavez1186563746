/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.11
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#include <utility>
#include <vector>
#include "cameraOcclusionDetectImpl.h"
#include "common/file_buffer.h"
#include "common/imageConvert.h"
#include "common/algUtil.h"
#include "common/arcterndefs_config_analyzer.h"


using namespace std;
using namespace cv;

namespace arctern {

static float get_motion_mean(std::vector<float> input_vec) {
  double sum = 0.0f;
  for(size_t i = 0; i < input_vec.size(); i ++){
    sum += input_vec[i];
  }
  double mean = sum / input_vec.size(); 
  return mean;
}

static double ssim_metric(cv::Mat img1, cv::Mat img2) {
  return background_estimation::ssim(img1, img2);
}

CameraOcclusionDetectImpl::CameraOcclusionDetectImpl() {
}

CameraOcclusionDetectImpl::~CameraOcclusionDetectImpl() noexcept {
  CameraOcclusionDetectImpl::Release();
}

void CameraOcclusionDetectImpl::Release() {
  if (nullptr != result_) {
    result_->Release();
    delete result_;
    result_ = nullptr;
  }
}

ErrorType CameraOcclusionDetectImpl::Init(CameraOcclusionDetectInitParameter *p) {
  s_param_ = p->s_param;
  n_param_ = p->n_param;
  resize_width_ = p->resize_width;
  resize_height_ = p->resize_height;
  x_blocks_ = p->x_blocks;
  y_blocks_ = p->y_blocks;
  least_frame_count_ = p->least_frame_count;
  motion_thr_ = p->motion_thr;
  regionmotion_thr_ = p->regionmotion_thr;
  lessmotioncount_thr_ = p->lessmotioncount_thr;
  static_thr_ = p->static_thr;
  ssim_thr_ = p->ssim_thr;
  labgen_p_ = std::make_shared<background_estimation::LaBGen_P>(resize_height_, resize_width_, s_param_, n_param_);
  motion_map_8u_ = unique_ptr<Mat>(new Mat(resize_height_, resize_width_, CV_8UC1));
  background_ = cv::Mat(resize_height_, resize_width_, CV_8UC3);
  if((resize_height_%y_blocks_ == 0)&&(resize_width_%x_blocks_ == 0)){
    motion_regions_.resize(x_blocks_*y_blocks_);
    motion_region_vec_.resize(x_blocks_*y_blocks_);
  }
  else{
    x_blocks_ = 2;
    y_blocks_ = 2;
    motion_regions_.resize(4);
    motion_region_vec_.resize(4);
  }
  for(int i = 0; i < y_blocks_; i ++){
    for(int j = 0; j < x_blocks_; j ++){
      motion_regions_[i*x_blocks_+j] = Mat(resize_height_ / y_blocks_, resize_width_ / x_blocks_, CV_8UC1);
    }
  }
  first_frame_ = true;
  return ErrorType::ERR_SUCCESS;
}

void CameraOcclusionDetectImpl::reset(){
  labgen_p_->reset_buffer(s_param_);
  first_frame_ = true;
  current_motion_buffer_.clear();
  for(int i = 0; i < y_blocks_; i ++){
    for(int j = 0; j < x_blocks_; j ++){
      motion_regions_[i*x_blocks_+j].setTo(0);
      motion_region_vec_[i*x_blocks_+j].clear();
    }
  }
}

void CameraOcclusionDetectImpl::CalcMotionInfo(){
  for(int i = 0; i < y_blocks_; i ++){
    for(int j = 0; j < x_blocks_; j ++){
      int start_y = (int)(i*resize_height_/y_blocks_);
      int end_y = (int)((i+1)*(resize_height_/y_blocks_)-1);
      int start_x = (int)(j*resize_width_/x_blocks_);
      int end_x = (int)((j+1)*resize_width_/x_blocks_-1);
      motion_regions_[i*x_blocks_+j] = (*motion_map_8u_)(cv::Range(start_y, end_y), cv::Range(start_x, end_x)).clone();
      motion_region_vec_[i*x_blocks_+j].push_back(mean(motion_regions_[i*x_blocks_+j])[0]);
    }
  }
}

std::pair<int, float> CameraOcclusionDetectImpl::CheckingOcclusion(const std::vector<cv::Mat> former_background_sequence){
  float ssim_sum = 0.0f;
  float ssim_mean = 0.0f;
  int lessmotion_count = 0;
  for(size_t i = 0; i < former_background_sequence.size(); i ++){
    ssim_sum += ssim_metric(background_, former_background_sequence[i]);
  }
  ssim_mean = ssim_sum / former_background_sequence.size();
  if(ssim_mean < ssim_thr_) {
    for(int i = 0; i < y_blocks_; i ++){
      for(int j = 0; j < x_blocks_; j ++){
        float motion_temp = get_motion_mean(motion_region_vec_[i*x_blocks_+j]);
        if(motion_temp < regionmotion_thr_)
          lessmotion_count += 1;
      }
    }
    if(lessmotion_count >= lessmotioncount_thr_) {
        return std::make_pair(1, ssim_mean);
    }
  }
  return std::make_pair(0, ssim_mean);
}
  
ErrorType CameraOcclusionDetectImpl::Preprocess(const RunParameter *p) {
  const auto implRunP = dynamic_cast<const CameraOcclusionDetectRunParameter *>(p);
  ErrorType ret = ErrorType::ERR_SUCCESS;
  preprocessed_imgs_.clear();
  size_t total1 = implRunP->imageV_.size();
  size_t total2 = implRunP->backgroundRefsV_.size();

  preprocessed_imgs_.resize(total1);
  for (size_t loopId = 0; loopId < total1; loopId++) {
    int ret01 = arctern::ArcternImage2Mat(implRunP->imageV_[loopId],
                                          preprocessed_imgs_[loopId]);
    cv::resize(preprocessed_imgs_[loopId], 
               preprocessed_imgs_[loopId],
               cv::Size(resize_width_, resize_height_));
    if (ret01 != 0) {
      return ErrorType::ERR_ARCTERN_IMAGE_MAT;
    }
  }

  if (total2 > 0) {
    preprocessed_refs_.clear();
    preprocessed_refs_.resize(total2);
    for (size_t loopId = 0; loopId < total2; loopId++) {
      int ret01 = arctern::ArcternImage2Mat(implRunP->backgroundRefsV_[loopId],
                                            preprocessed_refs_[loopId]);
      cv::resize(preprocessed_refs_[loopId], 
                 preprocessed_refs_[loopId],
                 cv::Size(resize_width_, resize_height_));
      if (ret01 != 0) {
        return ErrorType::ERR_ARCTERN_IMAGE_MAT;
      }
    }
  }
  return ret;
}

ErrorType CameraOcclusionDetectImpl::Process(const RunParameter *p,Result *r) {
  if(p == nullptr) return ErrorType ::ERR_RUN_PARM;
  if(r == nullptr) return ErrorType ::ERR_NULL_POINT;
  result_ = r;
  auto result = dynamic_cast<CameraOcclusionDetectResult *>(result_);

  auto ret = Preprocess(p);
  if (ret != ErrorType::ERR_SUCCESS) {
    return ret;
  }
  //reset buffer
  reset();
  //construction of background
  int n = preprocessed_imgs_.size();
  for(int i = 0; i < n; i ++){
    labgen_p_->insert(preprocessed_imgs_[i]);
    if(first_frame_){
        first_frame_ = false;
        continue;
    }
    labgen_p_->generate_background(background_);
    labgen_p_->get_motion_map().convertTo(*motion_map_8u_, CV_8U);

    CalcMotionInfo();
    if (i > (int)least_frame_count_){
      if (mean(*motion_map_8u_)[0] < motion_thr_){
        current_motion_buffer_.push_back(mean(*motion_map_8u_)[0]);
      }
      else{
        current_motion_buffer_.resize(0);
      }
      if (current_motion_buffer_.size() > static_thr_){
        break;
      }
    }
  }
  //generate backgound finish
  //checking whether occlusion occurred
  if (preprocessed_refs_.empty()) {
    result->isOcclusion = false;
    result->similarity  = 0;
  } else {
    std::pair<int, float> occ = CheckingOcclusion(preprocessed_refs_);
    result->isOcclusion = occ.first;
    result->similarity  = occ.second;
  }
  cv::imencode(".jpg", background_, result->backgroundImagePred);
 /* CameraOcclusionDetectResult *res = new CameraOcclusionDetectResult();
  res->isOcclusion = occ.first;
  res->similarity  = occ.second;
  res->backgroundImagePred  = r->backgroundImagePred;
  */
  return ret;
}


const Result *CameraOcclusionDetectImpl::GetRealBackground(const RunParameter *p) {
  auto r = dynamic_cast<CameraOcclusionDetectResult *>(result_);
  
  auto ret = Preprocess(p);
  if (ret != ErrorType::ERR_SUCCESS) {
    return r;
  }
  //reset buffer
  reset();
  //construction of background
  int n = preprocessed_imgs_.size();
  for(int i = 0; i < n; i ++){
    labgen_p_->insert(preprocessed_imgs_[i]);
    if(first_frame_){
        first_frame_ = false;
        continue;
    }
    labgen_p_->generate_background(background_);
    labgen_p_->get_motion_map().convertTo(*motion_map_8u_, CV_8U);
    CalcMotionInfo();
    if (i > (int)least_frame_count_){
      if (mean(*motion_map_8u_)[0] < motion_thr_){
        current_motion_buffer_.push_back(mean(*motion_map_8u_)[0]);
      }
      else{
        current_motion_buffer_.resize(0);
      }
      if (current_motion_buffer_.size() > static_thr_){
        break;
      }
    }
  }
  //generate backgound finish
  cv::imencode(".jpg", background_, r->backgroundImageReal);
  return this->result_;
}

} // namespace arctern 

namespace background_estimation{
    /* ========================================================================== *
     * Metric func                                                                *
     * ========================================================================== */
    double psnr(cv::Mat &I1, cv::Mat &I2) { 
      cv::Mat s1;
      cv::absdiff(I1, I2, s1);
      s1.convertTo(s1, CV_32F); 
      s1 = s1.mul(s1);
      Scalar s = sum(s1);  
      double sse = s.val[0] + s.val[1] + s.val[2];
      if (sse <= 1e-10) // for small values return zero  
      	return 0;
      else
      {
      	double mse = sse / (double)(I1.channels() * I1.total());   
      	double psnr = 10.0 * log10((255 * 255) / mse);
      	return psnr;
      }
    }

    double ssim(cv::Mat &i1, cv::Mat & i2) {
      const double C1 = 6.5025, C2 = 58.5225;
      int d = CV_32F;
      cv::Mat I1, I2;
      i1.convertTo(I1, d);
      i2.convertTo(I2, d);
      cv::Mat I1_2 = I1.mul(I1);
      cv::Mat I2_2 = I2.mul(I2);
      cv::Mat I1_I2 = I1.mul(I2);
      cv::Mat mu1, mu2;
      cv::GaussianBlur(I1, mu1, Size(11, 11), 1.5);
      cv::GaussianBlur(I2, mu2, Size(11, 11), 1.5);
      cv::Mat mu1_2 = mu1.mul(mu1);
      cv::Mat mu2_2 = mu2.mul(mu2);
      cv::Mat mu1_mu2 = mu1.mul(mu2);
      cv::Mat sigma1_2, sigam2_2, sigam12;
      cv::GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
      sigma1_2 -= mu1_2;
      
      cv::GaussianBlur(I2_2, sigam2_2, Size(11, 11), 1.5);
      sigam2_2 -= mu2_2;
      
      cv::GaussianBlur(I1_I2, sigam12, Size(11, 11), 1.5);
      sigam12 -= mu1_mu2;
      cv::Mat t1, t2, t3;
      t1 = 2 * mu1_mu2 + C1;
      t2 = 2 * sigam12 + C2;
      t3 = t1.mul(t2);
      
      t1 = mu1_2 + mu2_2 + C1;
      t2 = sigma1_2 + sigam2_2 + C2;
      t1 = t1.mul(t2);
      
      cv::Mat ssim_map;
      cv::divide(t3, t1, ssim_map);
      cv::Scalar mssim = mean(ssim_map);
      
      double ssim = (mssim.val[0] + mssim.val[1] + mssim.val[2]) / 3;
      return ssim;
    }
    /* ========================================================================== *
     * LaBGen_P                                                                   *
     * ========================================================================== */
    LaBGen_P::LaBGen_P(size_t height, size_t width, int32_t s, int32_t n) :
    height(height),
    width(width),
    s(s),
    n(n),
    motion_map(height, width, CV_32SC1),
    filter((min(height, width) / n) | 1),
    history(Utils::getROIs(height, width), s),
    first_frame(true) {
      quantities_of_motion = cv::Mat(height, width, filter.getOpenCVEncoding());
    }
    
    /******************************************************************************/
    void LaBGen_P::reset_buffer(int buffer_size)
    {
    	history.clear_history(buffer_size);
    	motion_map.setTo(0);
    	quantities_of_motion.setTo(0);
    	first_frame = true;
    }
    
    /******************************************************************************/
    
    void LaBGen_P::insert(const cv::Mat& current_frame) {
      /* Motion map computation by frame difference. */
      f_diff.compute(current_frame, motion_map);
    
      /* Initialization of background subtraction. */
      if (first_frame) {
    	first_frame = false;
    	return;
      }
    
      /* Filtering motion map to produce quantities of motion. */
      filter.compute(motion_map, quantities_of_motion);
    
      /* Insert the current frame along with the quantities of motion into the
       * history.
       */
      history.insert(quantities_of_motion, current_frame);
    }
    
    /******************************************************************************/
    
    void LaBGen_P::generate_background(cv::Mat& background) const {
      if (history.empty()) {
    	throw runtime_error(
    	  "Cannot generate the background with less than two inserted frames"
    	);
      }
      history.median(background, s);
    }
    
    /******************************************************************************/
    size_t LaBGen_P::get_height() const {
      return height;
    }
    
    /******************************************************************************/
    size_t LaBGen_P::get_width() const {
      return width;
    }
    
    /******************************************************************************/
    int32_t LaBGen_P::get_s() const {
      return s;
    }
    
    /******************************************************************************/
    int32_t LaBGen_P::get_n() const {
      return n;
    }
    
    /******************************************************************************/
    const cv::Mat& LaBGen_P::get_motion_map() const {
      return motion_map;
    }
    
    /******************************************************************************/
    const cv::Mat& LaBGen_P::get_quantities_of_motion() const {
      return quantities_of_motion;
    }
    
    /* ========================================================================== *
     * FrameDifferenceC1L1                                                        *
     * ========================================================================== */
    
    void FrameDifferenceC1L1::compute(const cv::Mat& current_frame, cv::Mat& motion_map) {
      if(current_frame.empty())
        return;
    
      cv::Mat converted_input;
    
      if (current_frame.channels() != 1)
        cv::cvtColor(current_frame, converted_input, CV_BGR2GRAY);
      else
        current_frame.copyTo(converted_input);
    
      if (previous_frame.empty()) {
        converted_input.copyTo(previous_frame);
        return;
      }
    
      const unsigned char* current_buffer  = converted_input.data;
      const unsigned char* previous_buffer = previous_frame.data;
    
      int32_t* motion_map_buffer =
        reinterpret_cast<int32_t*>(motion_map.data);
    
      for (size_t i = 0; i < current_frame.total(); ++i) {
        *(motion_map_buffer++) = abs(
          static_cast<int32_t>(*(current_buffer++)) - *(previous_buffer++)
        );
      }
    
      converted_input.copyTo(previous_frame);
    }
    
    /* ========================================================================== *
     * HistoryMat                                                                 *
     * ========================================================================== */
    
    HistoryMat::HistoryMat(const unsigned char* mat, const uint32_t positives) :
    positives(positives) {
      this->mat[0] = mat[0];
      this->mat[1] = mat[1];
      this->mat[2] = mat[2];
    }
    
    /******************************************************************************/
    HistoryMat::HistoryMat(const HistoryMat& copy) :
    positives(copy.positives) {
      mat[0] = copy.mat[0];
      mat[1] = copy.mat[1];
      mat[2] = copy.mat[2];
    }
    
    /******************************************************************************/
    HistoryMat& HistoryMat::operator=(const HistoryMat& copy) {
      if (this != &copy) {
    	mat[0] = copy.mat[0];
    	mat[1] = copy.mat[1];
    	mat[2] = copy.mat[2];
    
    	positives = copy.positives;
      }
    
      return *this;
    }
    
    /******************************************************************************/
    uint8_t HistoryMat::operator[](size_t i) const {
      return mat[i];
    }
	
    inline bool operator<(const HistoryMat& lhs, const HistoryMat& rhs) {
      return lhs.positives < rhs.positives;
    }

    /******************************************************************************/
    inline bool operator<=(const HistoryMat& lhs, const HistoryMat& rhs) {
      return lhs.positives <= rhs.positives;
    }

    /******************************************************************************/
    inline bool operator==(const HistoryMat& lhs, const HistoryMat& rhs) {
      return lhs.positives == rhs.positives;
    }

    /******************************************************************************/
    inline bool operator<(const HistoryMat& lhs, const uint32_t& rhs) {
      return lhs.positives < rhs;
    }

    /******************************************************************************/
    inline bool operator<=(const HistoryMat& lhs, const uint32_t& rhs) {
      return lhs.positives <= rhs;
    }

    /******************************************************************************/
    inline bool operator==(const HistoryMat& lhs, const uint32_t& rhs) {
      return lhs.positives == rhs;
    }

    /******************************************************************************/
    inline bool operator<(const uint32_t& lhs, const HistoryMat& rhs) {
      return lhs < rhs.positives;
    }

    /******************************************************************************/
    inline bool operator<=(const uint32_t& lhs, const HistoryMat& rhs) {
      return lhs <= rhs.positives;
    }

    /******************************************************************************/
    inline bool operator==(const uint32_t& lhs, const HistoryMat& rhs) {
      return lhs == rhs.positives;
    }
    
    /* ========================================================================== *
     * History                                                                    *
     * ========================================================================== */
    
    History::History(size_t buffer_size) : history(), buffer_size(buffer_size) {
      history.reserve(buffer_size + 1);
    }
    
    /******************************************************************************/
    History::HistoryVec& History::operator*() {
      return history;
    }
    
    /******************************************************************************/
    const History::HistoryVec& History::operator*() const {
      return history;
    }
    
    /******************************************************************************/
    void History::insert(
      const int32_t* quantities_of_motion, const unsigned char* current_frame
    ) {
      const int32_t* qt_buffer = quantities_of_motion;
      int32_t positives = *(qt_buffer);
    
      if (history.empty())
    	history.push_back(HistoryMat(current_frame, positives));
      else {
    	bool inserted = false;
    
    	for (
    	  HistoryVec::iterator it = history.begin(), end = history.end();
    	  it != end;
    	  ++it
    	) {
    	  if (positives <= (*it)) {
    		history.insert(it, HistoryMat(current_frame, positives));
    		inserted = true;
    
    		if (history.size() > buffer_size)
    		  history.erase(history.end() - 1);
    
    		break;
    	  }
    	}
    
    	if ((history.size() < buffer_size) && !inserted)
    	  history.push_back(HistoryMat(current_frame, positives));
      }
    }
    
    /******************************************************************************/
    void History::median(unsigned char* result, size_t size) const {
      if (history.size() == 1 || size == 1) {
    	result[0] = history[0][0];
    	result[1] = history[0][1];
    	result[2] = history[0][2];
      }
    
      vector<unsigned char> buffer_r(buffer_size);
      vector<unsigned char> buffer_g(buffer_size);
      vector<unsigned char> buffer_b(buffer_size);
    
      size_t _size = min(history.size(), size);
    
      for (size_t num = 0; num < _size; ++num) {
    	buffer_r[num] = history[num][0];
    	buffer_g[num] = history[num][1];
    	buffer_b[num] = history[num][2];
      }
    
      size_t middle = _size / 2;
    
      if (_size & 1) {
    	nth_element(buffer_r.begin(), buffer_r.begin() + middle, buffer_r.begin() + _size);
    	nth_element(buffer_g.begin(), buffer_g.begin() + middle, buffer_g.begin() + _size);
    	nth_element(buffer_b.begin(), buffer_b.begin() + middle, buffer_b.begin() + _size);
    
    	result[0] = buffer_r[middle];
    	result[1] = buffer_g[middle];
    	result[2] = buffer_b[middle];
      }
      else {
    	nth_element(buffer_r.begin(), buffer_r.begin() + (middle - 1), buffer_r.begin() + _size);
    	nth_element(buffer_g.begin(), buffer_g.begin() + (middle - 1), buffer_g.begin() + _size);
    	nth_element(buffer_b.begin(), buffer_b.begin() + (middle - 1), buffer_b.begin() + _size);
    
    	nth_element(buffer_r.begin() + middle, buffer_r.begin() + middle, buffer_r.begin() + _size);
    	nth_element(buffer_g.begin() + middle, buffer_g.begin() + middle, buffer_g.begin() + _size);
    	nth_element(buffer_b.begin() + middle, buffer_b.begin() + middle, buffer_b.begin() + _size);
    
    	result[0] = ((static_cast<int32_t>(buffer_r[middle - 1])) + (buffer_r[middle])) / 2;
    	result[1] = ((static_cast<int32_t>(buffer_g[middle - 1])) + (buffer_g[middle])) / 2;
    	result[2] = ((static_cast<int32_t>(buffer_b[middle - 1])) + (buffer_b[middle])) / 2;
      }
    }
    
    /******************************************************************************/
    bool History::empty() const {
      return history.empty();
    }
    
    /* ========================================================================== *
     * PatchesHistory                                                             *
     * ========================================================================== */
    
    PatchesHistory::PatchesHistory(const Utils::ROIs& rois, size_t buffer_size) :
    p_history(), rois(rois) {
      p_history.reserve(rois.size());
    
      for (size_t i = 0; i < rois.size(); ++i)
    	p_history.push_back(History(buffer_size));
    }
    
    /******************************************************************************/
    void PatchesHistory::insert(
      const cv::Mat& quantities_of_motion, const cv::Mat& current_frame
    ) {
      int32_t* qt_buffer = reinterpret_cast<int32_t*>(quantities_of_motion.data);
      unsigned char* current_buffer = current_frame.data;
    
      for (size_t i = 0, j = 0; i < current_frame.total(); ++i, j += 3)
    	p_history[i].insert(qt_buffer + i, current_buffer + j);
    }
    
    /******************************************************************************/
    void PatchesHistory::median(cv::Mat& result, size_t size) const {
      unsigned char* result_buffer = result.data;
    
      for (size_t i = 0, j = 0; i < rois.size(); ++i, j += 3){
    	p_history[i].median(result_buffer + j, size);
      }
    }
    
    /******************************************************************************/
    void PatchesHistory::clear_history(int buffer_size)
    {
    	p_history.clear();
    	p_history.reserve(rois.size());
    	for (size_t i = 0; i < rois.size(); ++i)
    		p_history.push_back(History(buffer_size));
    }

    /******************************************************************************/
    bool PatchesHistory::empty() const {
     for (History h : p_history) {
       if (h.empty())
    	 return true;
     }
    
     return false;
    }
    
    /* ========================================================================== *
     * QuantitiesMotion                                                           *
     * ========================================================================== */
    QuantitiesMotion::QuantitiesMotion(int size) : size(size) {}
    
    /******************************************************************************/
    void QuantitiesMotion::compute(
      cv::Mat& motion_map,
      cv::Mat& quantities_of_motion
    ) const {
      int half = size / 2;
      if (half == 0)
    	throw logic_error("Size divided by 2 is zero!");
   
      QuantitiesMotionEncoding* output_buffer =
    	reinterpret_cast<QuantitiesMotionEncoding*>(quantities_of_motion.data);
    
      SummedAreaTables<QuantitiesMotionEncoding> sums(motion_map);
    
      for (int y = 0; y < motion_map.rows; ++y) {
    	for (int x = 0; x < motion_map.cols; ++x, ++output_buffer) {
    	  /* Computing kernel ROI. */
    	  int x_roi = max(x - half, 0);
    	  int y_roi = max(y - half, 0);
    	  int width_roi =
    		min(x + half, (motion_map.cols - 1)) - max(x - half, 0) + 1;
    	  int height_roi =
    		min(y + half, (motion_map.rows - 1)) - max(y - half, 0) + 1;
    
    	  *output_buffer = sums.getIntegral(
    		y_roi, y_roi + height_roi - 1, x_roi, x_roi + width_roi - 1
    	  );
    	}
      }
    }
    
    /******************************************************************************/
    int QuantitiesMotion::getOpenCVEncoding() const {
      return CV_32SC1;
    }
    
    /* ========================================================================== *
     * Utils                                                                      *
     * ========================================================================== */
    string Utils::getMethod(string method) {
      size_t pos = method.rfind("/");
      pos = (pos == string::npos) ? 0 : pos + 1;
    
      return method.substr(pos, method.rfind(".") - pos);
    }
    
    /******************************************************************************/
    Utils::ROIs Utils::getROIs(size_t height, size_t width, size_t segments) {
      if (segments == 0)
    	return getROIs(height, width);
    
      ROIs rois;
      rois.reserve(segments * segments);
    
      size_t patch_height = height / segments;
      size_t h_reminder = height % segments;
    
      size_t patch_width = width / segments;
      size_t w_reminder = width % segments;
    
      /* Algorithm. */
      size_t h_reminder_tmp = h_reminder;
      size_t previous_y = 0;
      size_t previous_height = 0;
    
      /*
       * If the modulo X of a dimension divided by the number of segments is
       * superior to 0, then it is distributed among the X first patches.
       */
      for (size_t i = 0; i < segments; ++i) {
    	size_t w_reminder_tmp = w_reminder;
    	size_t previous_x = 0;
    	size_t previous_width = 0;
    
    	for (size_t j = 0; j < segments; ++j) {
    	  rois.push_back(
    		Rect(
    		  (j == 0) ? 0 : (previous_x + previous_width),
    		  (i == 0) ? 0 : (previous_y + previous_height),
    		  patch_width  + ((w_reminder_tmp > 0) ? 1 : 0),
    		  patch_height + ((h_reminder_tmp > 0) ? 1 : 0)
    		)
    	  );
    
    	  previous_x = rois.back().x;
    	  previous_width = rois.back().width;
    
    	  if (w_reminder_tmp > 0)
    		--w_reminder_tmp;
    	}
    
    	previous_y = rois.back().y;
    	previous_height = rois.back().height;
    
    	if (h_reminder_tmp > 0)
    	  --h_reminder_tmp;
      }
    
      return rois;
    }
    
    /******************************************************************************/
    Utils::ROIs Utils::getROIs(size_t height, size_t width) {
      ROIs rois;
      rois.reserve(height * width);
    
      for (size_t i = 0; i < height; ++i) {
    	for (size_t j = 0; j < width; ++j)
    	  rois.push_back(Rect(j, i, 1, 1));
      }
    
      return rois;
    }
    
    /****************************************************************************/
    void Utils::normalize_mat(cv::Mat& input, double max) {
      normalize_mat(input, input, max);
    }
    
    /****************************************************************************/
    void Utils::normalize_mat(const cv::Mat& input, cv::Mat& output, double max) {
      double max_value;
      minMaxLoc(input, 0, &max_value);
      input.convertTo(output, output.empty() ? -1 : output.type(), max / max_value);
    }
};

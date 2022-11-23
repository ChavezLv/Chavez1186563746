//
// Created by Admin on 2021/6/18.
//

#include "TrtMultiDetectCenterImpl.h"
#include "src/common/common.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
#include "src/common/detectUtil.h"
namespace arctern {

TrtMultiDetectCenterImpl::TrtMultiDetectCenterImpl() {
  cls_project.clear();
  vec_cls_project.clear();
}

TrtMultiDetectCenterImpl::~TrtMultiDetectCenterImpl() {

}

ErrorType
TrtMultiDetectCenterImpl::GetModelsConfigInfo(const MxnetModels *models) {
  version_ = models->version()->str();
  auto model = models->models()->Get(0);
  bool return_res = try_get_value(*model, "INPUT_DIM_W", input_dim_w);
  return_res &= try_get_value(*model, "INPUT_DIM_H", input_dim_h);
  return_res &= try_get_value(*model, "OUTPUT_DIM_W", output_dim_w);
  if(version_ == "1.0.0"){
    input_dim_w = 512;
    input_dim_h = 288;
    output_dim_w = 128;
  }
  return_res &= try_get_value(*model, "OUTPUT_DIM_H", output_dim_h);
  if (m_initParam.thresh_ <= 0) {
    return_res &= try_get_value(*model, "CONF_THRESH", m_initParam.thresh_);
  }
  if (!try_get_value(*model, "FLAG_QUALITY", flag_quality)) {
    flag_quality = false;
  }
  if (!try_get_value(*model, "FLAG_IND_WH", flag_ind_wh)) {
    flag_ind_wh = false;
  }

  if (!try_get_value(*model, "CLS_PROJECT", vec_cls_project)){
    vec_cls_project = std::vector<int>{0, 0, 1, 1, 2, 4, 3, 2, 4, 3}; /// default value, solve the compatibility of old model
  }
  if(flag_quality) {
    //cls project must be a multiple of 2;
    assert(!(vec_cls_project.size() & 1));
    for (size_t i = 0; i < vec_cls_project.size(); i+=2){
      cls_project[vec_cls_project[i]] = vec_cls_project[i+1];
    }
  }
  m_modelsInputWidth = input_dim_w;
  m_modelsInputHeight = input_dim_h;
  hm_ratio_h_ = input_dim_h * 1.0 / output_dim_h;
  hm_ratio_w_ = input_dim_w * 1.0 / output_dim_w;
  return ErrorType ::ERR_SUCCESS;
}

void TrtMultiDetectCenterImpl::initPostProcessParam(int img_width, int img_height, const cv::Size& osize,
                                     std::vector<int> &pad_w, std::vector<int> &pad_h, std::vector<float> &img_ratio) {
  const int w = osize.width;
  const int h = osize.height;
  int new_w = img_width;
  int new_h = img_height;

  if ((static_cast<float>(w) / img_width) <
      (static_cast<float>(h) / img_height)) {
    new_w = w;
    new_h = (img_height * w) / img_width;
  } else {
    new_h = h;
    new_w = (img_width * h) / img_height;
  }

  const int offset_w = (w - new_w) >> 1;
  const int offset_h = (h - new_h) >> 1;
  pad_h.push_back(offset_h);
  pad_w.push_back(offset_w);
  img_ratio.push_back(float(img_width) / float(new_w));
}

ErrorType TrtMultiDetectCenterImpl::LocalPostProcess(const RunParameter *p,
                                                     int startId,
                                                     int endId,
                                                     vector<arctern::OutputTensor<float>> &outputs) {
  auto r = dynamic_cast<MultiDetectCenterResult *>(m_pResult);

  int total = endId - startId;
  std::vector<int> pad_w;
  std::vector<int> pad_h;
  std::vector<float> img_ratio;
  std::vector<int> imgcols(total);
  std::vector<int> imgrows(total);
  std::vector<float> img_ratio_w;
  std::vector<float> img_ratio_h;
  for (int subId = 0; subId < total; subId++) {
    imgcols[subId] = p->imageV_[startId+subId].width;
    imgrows[subId] = p->imageV_[startId+subId].height;
    if(version_ == "1.0.0"){
      pad_h.push_back(0);
      pad_w.push_back(0);
      img_ratio_w.push_back(imgcols[subId]*1.0/m_modelsInputWidth);
      img_ratio_h.push_back(imgrows[subId]*1.0/m_modelsInputHeight);
      hm_ratio_w_ = 4;
      hm_ratio_h_ = 4;
    }
    else{
      initPostProcessParam(p->imageV_[startId + subId].width,
                           p->imageV_[startId + subId].height,
                           cv::Size(m_modelsInputWidth, m_modelsInputHeight),
                           pad_w, pad_h, img_ratio);
    }

  }


  float* qm = 0;
  int stride_qm = 0;
  int offset_qm;
  int stride_wh = 0;
  int offset_wh;

  float* top_inds   = outputs[1].data();
  float* top_scores = outputs[0].data();
  float* top_cls    = outputs[2].data();
  float* top_x      = outputs[3].data();
  float* top_y      = outputs[4].data();
  float* wh         = outputs[5].data();
  float* offset     = outputs[6].data();
  if(flag_quality){
    qm       = outputs[7].data();
    stride_qm = outputs[7].shape().size() / outputs[7].shape()[0];
    offset_qm = 0;
  }
  if(flag_ind_wh){
    stride_wh    = outputs[5].shape().size() / outputs[5].shape()[0];
    offset_wh    = 0;
  }
  int stride_ind     = outputs[0].shape().size() / outputs[0].shape()[0];
  int offset_ind     = 0;
  int stride_offset  = outputs[6].shape().size() / outputs[6].shape()[0];
  int offset_offset  = 0;

  cv::Mat tl;
  cv::Mat br;
  float x;
  float y;
  float w;
  float h;
  int tl_x;
  int tl_y;
  int br_x;
  int br_y;
  int wh_ind;
  int off_ind;
  for(int subId = 0; subId < total; subId++) {
    int i = subId;
    Vbbox boxes;
    for(int j = 0; j < stride_ind; j++){
      if (flag_ind_wh) {
        wh_ind = int(top_inds[j + offset_ind]) + offset_wh;
        off_ind = int(top_inds[j + offset_ind]) + offset_offset;
      }else{
        off_ind = int(top_inds[j + offset_ind]) + offset_offset;
      }
      if(top_scores[j + offset_ind] > m_initParam.thresh_){
        if (flag_ind_wh) {
          int cls = int(top_cls[j + offset_ind]);
          x = float(top_x[j + offset_ind]) + offset[off_ind];
          y = float(top_y[j + offset_ind]) + offset[off_ind + output_dim_w * output_dim_h];
          w = wh[wh_ind + 2 * cls * output_dim_h * output_dim_w];
          h = wh[wh_ind + (2 * cls + 1) * output_dim_w * output_dim_h];
        }else{
          x = float(top_x[j + offset_ind]) + offset[off_ind];
          y = float(top_y[j + offset_ind]) + offset[off_ind + output_dim_w * output_dim_h];
          w = wh[off_ind];
          h = wh[off_ind +  output_dim_w * output_dim_h];
        }

        float x1 = x - w / 2.0f;
        float y1 = y - h / 2.0f;
        float x2 = x + w / 2.0f;
        float y2 = y + h / 2.0f;

        tl = cv::Mat(cv::Point3f( x1, y1, 1.0 ));
        br = cv::Mat(cv::Point3f( x2, y2, 1.0 ));

        if(version_ == "1.0.0"){
          tl_x = tl.at<float>(0,0) * hm_ratio_w_ - pad_w[i] < 0 ? 0 : int((tl.at<float>(0,0) * hm_ratio_w_ - pad_w[i]) * img_ratio_w[i]);
          tl_x = tl_x > imgcols[i] - 1 ? imgcols[i] - 1 : tl_x;
          tl_y = tl.at<float>(0,1) * hm_ratio_h_ - pad_h[i] < 0 ? 0 : int((tl.at<float>(0,1) * hm_ratio_h_ - pad_h[i]) * img_ratio_h[i]);
          tl_y = tl_y > imgrows[i] - 1 ? imgrows[i] - 1 : tl_y;
          br_x = br.at<float>(0,0) * hm_ratio_w_ - pad_w[i] < 0 ? 0 : int((br.at<float>(0,0) * hm_ratio_w_ - pad_w[i]) * img_ratio_w[i]);
          br_x = br_x > imgcols[i] - 1 ? imgcols[i] - 1 : br_x;
          br_y = br.at<float>(0,1) * hm_ratio_h_ - pad_h[i] < 0 ? 0: int((br.at<float>(0,1) * hm_ratio_h_ - pad_h[i]) * img_ratio_h[i]);
          br_y = br_y > imgrows[i] - 1 ? imgrows[i] - 1 : br_y;
        }
        else{
          tl_x = tl.at<float>(0,0) * hm_ratio_w_ - pad_w[i] < 0 ? 0 : int((tl.at<float>(0,0) * hm_ratio_w_ - pad_w[i]) * img_ratio[i]);
          tl_x = tl_x > imgcols[i] - 1 ? imgcols[i] - 1 : tl_x;
          tl_y = tl.at<float>(0,1) * hm_ratio_h_ - pad_h[i] < 0 ? 0 : int((tl.at<float>(0,1) * hm_ratio_h_ - pad_h[i]) * img_ratio[i]);
          tl_y = tl_y > imgrows[i] - 1 ? imgrows[i] - 1 : tl_y;
          br_x = br.at<float>(0,0) * hm_ratio_w_ - pad_w[i] < 0 ? 0 : int((br.at<float>(0,0) * hm_ratio_w_ - pad_w[i]) * img_ratio[i]);
          br_x = br_x > imgcols[i] - 1 ? imgcols[i] - 1 : br_x;
          br_y = br.at<float>(0,1) * hm_ratio_h_ - pad_h[i] < 0 ? 0: int((br.at<float>(0,1) * hm_ratio_h_ - pad_h[i]) * img_ratio[i]);
          br_y = br_y > imgrows[i] - 1 ? imgrows[i] - 1 : br_y;
        }

        Bbox b(tl_x, tl_y, br_x, br_y, float(top_scores[j + offset_ind]));
        if(flag_quality) {
          int quality_ind = int(top_cls[j + offset_ind]) * output_dim_w * output_dim_h +
              int(top_inds[j + offset_ind]) + offset_qm;
          float quality = qm[quality_ind];
          b.quality = quality < 0 ? 0 : quality;
          b.label = cls_project[int(top_cls[j + offset_ind])];
        }else{
          b.label = top_cls[j + offset_ind];
          b.quality = -1.0;
        }
        boxes.push_back(b);
      }
    }
    if (square_rect_) {
      ConvertSquare(boxes, exth_scale_rect_, extw_scale_rect_);
    }
    for (auto &box : boxes) {
      MULTI_DETECT_CENTER_INFO info;
      info.rect.x = box.x1;
      info.rect.y = box.y1;
      info.rect.width  = box.x2 - box.x1;
      info.rect.height = box.y2 - box.y1;
      info.score = box.score;
      info.quality = box.quality;
      info.type = static_cast<MultiCatType>(box.label);
      r->detInfos_[startId + i].push_back(info);
    }
    if(flag_quality){
      offset_qm += stride_qm;
    }
    if(flag_ind_wh){
      offset_wh += stride_wh;
    }
    offset_offset  += stride_offset;
    offset_ind += stride_ind;
  }
  return ErrorType::ERR_SUCCESS;
}

void TrtMultiDetectCenterImpl::InitPreprocessorParam(){
  m_pStream = m_vEngines[0]->getContext()->getCudaStream();
  m_pPreprocessor.reset(new NppPreprocessor(m_pStream));
  NppPreprocessorInitPt pPreprocessorInitParam ;

  pPreprocessorInitParam.netH_ = m_modelsInputHeight;
  pPreprocessorInitParam.netW_ = m_modelsInputWidth;
  pPreprocessorInitParam.mean_ = 127.5;
  pPreprocessorInitParam.std_ = 128;
  pPreprocessorInitParam.borderValue_ = 0;
  assert(m_pPreprocessor);
  m_lastErrorCode = m_pPreprocessor->Init(&pPreprocessorInitParam);
  assert(m_lastErrorCode == ErrorType::ERR_SUCCESS);
}
void TrtMultiDetectCenterImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  if(version_ == "1.0.0"){
    runParm->sequence_.push_back(OperatorType::resized);
  }
  else {
    runParm->sequence_.push_back(OperatorType::multiCat);
  }
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}


}

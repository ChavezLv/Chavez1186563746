
#include "personScarfImpl.h"
#include "common/algUtil.h"
#include "personScarf.h"

using namespace arctern;

PersonScarfImpl::PersonScarfImpl() {
  input_h_ = 256;
  input_w_ = 128;
}

PersonScarfImpl::~PersonScarfImpl() { PersonScarfImpl::Release(); }

ErrorType
PersonScarfImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                std::vector<arctern::Tensor<float>> &outputs) {
  PersonScarfResult *p_res = dynamic_cast<PersonScarfResult *>(result_);

  int i_total = endId - startId;
  int i_stride = outputs[0].size() / outputs[0].shape()[0];
  for (int subId = 0; subId < i_total; subId++) {
    const float *p_data = outputs[0].data() + subId * i_stride;
    auto type_res = calcType(p_data, i_stride);
    p_res->m_vClassifyResults[startId + subId].first = type_res.first;
    p_res->m_vClassifyResults[startId + subId].second = type_res.second;
  }

  return ErrorType::ERR_SUCCESS;
}

std::pair<int, float> PersonScarfImpl::calcType(const float *p_data,
                                              const size_t class_num) {
  if (nullptr == p_data || class_num <= 0) {
    return std::make_pair(-1, 0.f);
  }

  std::vector<float> v_prob(class_num);
  memcpy(v_prob.data(), p_data, class_num * sizeof(float));

  float f_max_val = 0.f;
  int i_max_idx = softmax(v_prob, f_max_val);

  return std::make_pair(i_max_idx, f_max_val);
}

cv::Rect PersonScarfImpl::check_rect(const int width, const int height,
                                   const cv::Rect bbox) {
  float x1 = bbox.tl().x;
  float y1 = bbox.tl().y;
  float x2 = bbox.br().x + bbox.width;
  float y2 = bbox.br().y + bbox.height;
  auto sx1 = (int) (x1 < 0 ? 0 : x1);
  auto sy1 = (int) (y1 < 0 ? 0 : y1);
  auto sx2 = (int) (x2 <= width ? x2 : width);
  auto sy2 = (int) (y2 <= height ? y2 : height);
  auto ew  = (int) (sx2 - sx1);
  auto eh  = (int) (sy2 - sy1);
  if (ew > 0 && eh > 0) {
    cv::Rect body_rect = cv::Rect(sx1, sy1, ew, eh);
    return body_rect;
  }
  else {
    cv::Rect body_rect = cv::Rect(0, 0, width, height);
    return body_rect;
  }
}

ArcternRect PersonScarfImpl::LocalGetRoi(const RunParameter *p, int idx) {
//  const auto p_run_param = dynamic_cast<const FaceTypeRunParameter *>(p);
  auto &arct_rect = p->rectsV_[idx];
  cv::Rect ori_rect(arct_rect.x, arct_rect.y, arct_rect.width,
                    arct_rect.height);
  cv::Rect bbox = check_rect(p->imageV_[idx].width, p->imageV_[idx].height, ori_rect);
  ArcternRect result;
  result.x = bbox.x;
  result.y = bbox.y;
  result.width = bbox.width;
  result.height = bbox.height;
  return result;
}

void PersonScarfImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
}

#include "personUmbrella.h"
#include "personUmbrellaImpl.h"
#include "common/algUtil.h"
#include "common/file_buffer.h"

using namespace arctern;

PersonUmbrellaImpl::PersonUmbrellaImpl() {
  input_h_ = 256;
  input_w_ = 128;
}

PersonUmbrellaImpl::~PersonUmbrellaImpl() { PersonUmbrellaImpl::Release(); }

ErrorType
PersonUmbrellaImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                std::vector<arctern::Tensor<float>> &outputs) {
  PersonUmbrellaResult *p_res = dynamic_cast<PersonUmbrellaResult *>(result_);

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

std::pair<int, float> PersonUmbrellaImpl::calcType(const float *p_data,
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

cv::Rect PersonUmbrellaImpl::check_rect(const int width, const int height,
                                   const cv::Rect bbox) {
  float x1 = bbox.tl().x;
  float y1 = bbox.tl().y;
  float x2 = bbox.br().x;
  float y2 = bbox.br().y;
  auto sx1 = (int) (x1 < 0 ? 0 : x1);
  auto sy1 = (int) (y1 < 0 ? 0 : y1);
  auto sx2 = (int) (x2 <= width ? x2 : width);
  auto sy2 = (int) (y2 <= height ? y2 : height);
  y1 = (int) (sy1 - 0.15 * (sy2 - sy1));
  sy1 = (int) (y1 < 0 ? 0 : y1);
  x1 = (int) (sx1 - 0.10 * (sx2 - sx1));
  sx1 = (int) (x1 < 0 ? 0 : x1);
  x2 = (int) (sx2 + 0.10 * (sx2 - sx1));
  sx2 = (int) (x2 <= width ? x2 : width);
  auto ew  = (int) (sx2 - sx1);
  auto eh  = (int) (sy2 - sy1);
  if (ew > 0 && eh > 0) {
    cv::Rect vehicle_rect = cv::Rect(sx1, sy1, ew, eh);
    return vehicle_rect;
  }
  else {
    cv::Rect vehicle_rect = cv::Rect(0, 0, width, height);
    return vehicle_rect;
  }
}

ArcternRect PersonUmbrellaImpl::LocalGetRoi(const RunParameter *p, int idx) {
  const auto p_run_param = dynamic_cast<const PersonUmbrellaRunParameter *>(p);
  auto &arct_rect = p_run_param->rectsV_[idx];
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
void PersonUmbrellaImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
}

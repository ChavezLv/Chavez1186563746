// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.01
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#include "personBagImpl.h"
#include "common/algUtil.h"
#include "common/file_buffer.h"
#include "common/imageConvert.h"
#include "log/logger.h"
#include "personBag.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#include "src/Processors/Algorithms/attrpreprocess.h"
#include <string.h>

using namespace arctern;

personBagImpl::personBagImpl() {
  input_h_ = 256;
  input_w_ = 128;
}

personBagImpl::~personBagImpl() { personBagImpl::Release(); }

ErrorType personBagImpl::Init(const InitParameter *p) {
  auto ret = ArcternAlgorithm::Init(p);
  if (ret != ErrorType::ERR_SUCCESS) {
    personBagImpl::Release();
    return ret;
  }

  preProcessor_.reset(new AttrPreProcesser);

  arctern::AttrPreProInitParameter init_param;
  init_param.netW_ = input_w_;
  init_param.netH_ = input_h_;
  init_param.mean_ = 127.5f;
  init_param.std_ = 128.0f;
  init_param.alignFace_ = AlignFace::NoAlignFace;

  assert(preProcessor_);

  auto code = preProcessor_->Init(&init_param);
  assert(code == ErrorType::ERR_SUCCESS);

  return ErrorType::ERR_SUCCESS;
}

void personBagImpl::Release() {

  ArcternAlgorithm::Release();
}

cv::Mat personBagImpl::LocalPreProcess(const cv::Mat &image,
                                       const RunParameter *p, int idx) {
  if (nullptr == p || image.empty()) {
    return cv::Mat();
  }
  const auto p_run_param = dynamic_cast<const PersonBagRunParameter *>(p);
  auto &arct_rect = p_run_param->rectsV_[idx];
  cv::Rect ori_rect(arct_rect.x, arct_rect.y, arct_rect.width,
                    arct_rect.height);
  cv::Rect bbox = check_rect(image.cols, image.rows, ori_rect);

  PreProRunParameter pre_run_param;
  pre_run_param.iMat_ = image(bbox).clone();

  auto result = preProcessor_->Process(&pre_run_param);

  if (ErrorType::ERR_SUCCESS != result.error_type_) {
    assert(result.error_type_ == ErrorType::ERR_SUCCESS);
    return cv::Mat();
  }

  return result.oMat_;
}

ErrorType
personBagImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                std::vector<arctern::Tensor<float>> &outputs) {
  PersonBagResult *p_res = dynamic_cast<PersonBagResult *>(result_);

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

std::pair<int, float> personBagImpl::calcType(const float *p_data,
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

cv::Rect personBagImpl::check_rect(const int width, const int height,
                                   const cv::Rect bbox) {
  float x1 = bbox.tl().x;
  float y1 = bbox.tl().y;
  float x2 = bbox.br().x;
  float y2 = bbox.br().y;
  auto sx1 = (int)(x1 < 0 ? 0 : x1);
  auto sy1 = (int)(y1 < 0 ? 0 : y1);
  auto sx2 = (int)(x2 <= width ? x2 : width);
  auto sy2 = (int)(y2 <= height ? y2 : height);
  x1 = (int)(sx1 - 0.10 * (sx2 - sx1));
  sx1 = (int)(x1 < 0 ? 0 : x1);
  x2 = (int)(sx2 + 0.10 * (sx2 - sx1));
  sx2 = (int)(x2 <= width ? x2 : width);
  auto ew = (int)(sx2 - sx1);
  auto eh = (int)(sy2 - sy1);
  if (ew > 0 && eh > 0) {
    cv::Rect person_rect = cv::Rect(sx1, sy1, ew, eh);
    return person_rect;
  } else {
    cv::Rect person_rect = cv::Rect(0, 0, width, height);
    return person_rect;
  }
}

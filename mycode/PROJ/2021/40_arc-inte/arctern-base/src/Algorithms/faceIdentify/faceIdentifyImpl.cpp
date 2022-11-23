// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.01
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#include "faceIdentifyImpl.h"
#include "common/algUtil.h"
#include "common/file_buffer.h"
#include "common/imageConvert.h"
#include "faceIdentify.h"
#include "log/logger.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#include "src/Processors/Algorithms/attrpreprocess.h"
#include <string.h>

using namespace arctern;

FaceIdentifyImpl::FaceIdentifyImpl() : m_aExtScale{0.f, 0.5f, 0.f, 0.1f} {
  input_h_ = 255;
  input_w_ = 255;
}

FaceIdentifyImpl::~FaceIdentifyImpl() { FaceIdentifyImpl::Release(); }

ErrorType FaceIdentifyImpl::Init(const InitParameter *p) {
  auto ret = ArcternAlgorithm::Init(p);
  if (ret != ErrorType::ERR_SUCCESS) {
    FaceIdentifyImpl::Release();
    return ret;
  }

  fg FaceIdentifyResult;

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

void FaceIdentifyImpl::Release() {

  ArcternAlgorithm::Release();
}


cv::Mat FaceIdentifyImpl::LocalPreProcess(const cv::Mat &image,
                                          const RunParameter *p, int idx) {
  if (nullptr == p || image.empty()) {
    return cv::Mat();
  }
  const auto p_run_param = dynamic_cast<const FaceIdentifyRunParameter *>(p);
  auto &arct_rect = p_run_param->rectsV_[idx];
  cv::Rect ori_rect(arct_rect.x, arct_rect.y, arct_rect.width,
                    arct_rect.height);
  cv::Rect bbox = getFace(image.cols, image.rows, ori_rect);

  PreProRunParameter pre_run_param;
  pre_run_param.iMat_ = image(bbox).clone();

  auto result = preProcessor_->Process(&pre_run_param);

  if (ErrorType::ERR_SUCCESS != result.error_type_) {
    assert(result.error_type_ == ErrorType::ERR_SUCCESS);
    return cv::Mat();
  }

  return result.oMat_;
}

ErrorType FaceIdentifyImpl::LocalPostProcess(
    const RunParameter *p, int startId, int endId,
    std::vector<arctern::Tensor<float>> &outputs) {
  FaceIdentifyResult *p_res = dynamic_cast<FaceIdentifyResult *>(result_);

  int i_total = endId - startId;
  int i_stride = outputs[0].size() / outputs[0].shape()[0];
  for (int subId = 0; subId < i_total; subId++) {
    const float *p_data = outputs[0].data() + subId * i_stride;
    auto type_res = calcType(p_data, i_stride);
    p_res->m_vTypeResults[startId + subId].first = type_res.first;
    p_res->m_vTypeResults[startId + subId].second = type_res.second;
  }

  return ErrorType::ERR_SUCCESS;
}

std::pair<int, float> FaceIdentifyImpl::calcType(const float *p_data,
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

cv::Rect FaceIdentifyImpl::getFace(const int width, const int height,
                                   const cv::Rect bbox) {
  float x1 = bbox.tl().x;
  float y1 = bbox.tl().y;
  float x2 = bbox.br().x;
  float y2 = bbox.br().y;
  float w = x2 - x1 + 1;
  float h = y2 - y1 + 1;

  y1 = y1 - h * m_aExtScale.at(1);
  y2 = y2 + h * m_aExtScale.at(3);

  float by1 = std::min(std::max(0, static_cast<int>(y1)), height);
  float by2 = std::min(std::max(0, static_cast<int>(y2)), height);

  float hh = by2 - by1 + 1;
  float ww = hh;

  if (ww > w) {
    int wx = (int)(ww - w) / 2;
    x1 = x1 - wx;
    x2 = x2 + wx;
  }

  float bx1 = std::min(std::max(0, static_cast<int>(x1)), width);
  float bx2 = std::min(std::max(0, static_cast<int>(x2)), width);

  return cv::Rect(cv::Point(bx1, by1), cv::Point(bx2, by2));
}

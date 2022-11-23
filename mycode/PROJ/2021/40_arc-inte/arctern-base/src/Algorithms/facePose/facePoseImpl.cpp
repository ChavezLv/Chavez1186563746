/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.27
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include <algorithm>
#include <vector>
#include "facePoseImpl.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "common/file_buffer.h"
#include "common/algUtil.h"
#include "log/logger.h"

using arctern::FacePoseImpl;
using arctern::ErrorType;
using arctern::Result;

FacePoseImpl::FacePoseImpl() {
  input_h_ = 64;
  input_w_ = 64;
}

FacePoseImpl::~FacePoseImpl() noexcept {
  FacePoseImpl::Release();
}

ErrorType FacePoseImpl::CheckRunParam(const RunParameter *p) {
  auto rp = dynamic_cast<const FacePoseRunParameter *>(p);
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(rp);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) return arcternBaseCheck;
  return ErrorType::ERR_SUCCESS;
}

ErrorType FacePoseImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                         std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<FacePoseResult *>(result_);
  int total = endId - startId;

  /// stride is same. so just calculate the first one
  int stride = outputs[0].size() / outputs[0].shape()[0];
  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride;
    const float *data1 = outputs[1].data() + subId * stride;
    const float *data2 = outputs[2].data() + subId * stride;
    r->facePose_[startId + subId].pitch = calcResult(data0, stride);
    r->facePose_[startId + subId].yaw = calcResult(data1, stride);
    r->facePose_[startId + subId].roll = calcResult(data2, stride);
  }

  return ErrorType::ERR_SUCCESS;
}

cv::Rect FacePoseImpl::getFace(int width, int height, const cv::Rect &bbox) {
  float scale = 1.1f;
  int w = bbox.width;
  int h = bbox.height;
  float x1 = bbox.tl().x;
  float y1 = bbox.tl().y;

  int scaled_w = static_cast<int>(w * scale + 0.5);
  int scaled_h = static_cast<int>(h * scale + 0.5);

  x1 = x1 - (scaled_w - w) / 2;
  y1 = y1 - (scaled_h - h) / 2;

  int roundingX1 = static_cast<int>(x1 + 0.5);
  int roundingY1 = static_cast<int>(y1 + 0.5);

  x1 = std::min(width - 1, std::max(0, roundingX1));
  y1 = std::min(height - 1, std::max(0, roundingY1));

  float x2 = std::min(width - 1, std::max(0, roundingX1 + scaled_w));
  float y2 = std::min(height - 1, std::max(0, roundingY1 + scaled_h));

  return cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2));
}

arctern::FacePoseLevel FacePoseImpl::calcResult(const float *data, int num) {
  int distance = std::distance(data, std::max_element(data, data + num));
  return arctern::FacePoseLevel(distance);
}

int FacePoseImpl::GetConfigInfo(arctern::MxnetModel *model) {
  return 0;
}


arctern::ArcternRect FacePoseImpl::LocalGetRoi(const RunParameter *p, int idx)  {
  const auto implRunP = dynamic_cast<const FacePoseRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  auto rect = check_shrink_box(oriRect, p->imageV_[idx].width, p->imageV_[idx].height);
  rect = getFace(p->imageV_[idx].width, p->imageV_[idx].height, rect);
  ArcternRect result;
  result.x = rect.x;
  result.y = rect.y;
  result.width = rect.width;
  result.height = rect.height;
  return result;
}
void FacePoseImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::normalized);
}



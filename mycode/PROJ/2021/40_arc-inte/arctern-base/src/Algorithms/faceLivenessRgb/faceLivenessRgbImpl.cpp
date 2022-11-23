/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.22
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include <algorithm>
#include <vector>
#include "faceLivenessRgbImpl.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "common/file_buffer.h"
#include "common/algUtil.h"

#include "log/logger.h"

using arctern::FaceLivenessRgbImpl;
using arctern::ErrorType;
using arctern::Result;

FaceLivenessRgbImpl::FaceLivenessRgbImpl() {
  input_h_ = 128;
  input_w_ = 128;
}

FaceLivenessRgbImpl::~FaceLivenessRgbImpl() noexcept {
  FaceLivenessRgbImpl::Release();
}

ErrorType FaceLivenessRgbImpl::CheckRunParam(const RunParameter *p) {
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(p);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) return arcternBaseCheck;
  return ErrorType::ERR_SUCCESS;
}

ErrorType FaceLivenessRgbImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                                std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<FaceLivenessRgbResult *>(result_);
  int total = endId - startId;
  int stride = outputs[1].size() / outputs[1].shape()[0];
  for (int subId = 0; subId < total; subId++) {
    const float *data = outputs[1].data() + subId * stride;
    float cls = std::exp(data[1]) / (std::exp(data[0]) + std::exp(data[1]));

    /// spdlog::info("data0 = {}, data1 = {}, cls = {}", data[0], data[1], cls);
    r->faceLiveness_[startId + subId] = cls;
  }

  return ErrorType::ERR_SUCCESS;
}

arctern::ArcternRect FaceLivenessRgbImpl::getFace(int width, int height, const cv::Rect &bbox) {
  int x1 = bbox.tl().x;
  int y1 = bbox.tl().y;
  int x2 = bbox.br().x - 1;  // python implementation lacks 1 elem
  int y2 = bbox.br().y - 1;

  int w = x2 - x1;  // aligned with python
  int h = y2 - y1;  // aligend with python
  x1 = std::max(0, x1 - static_cast<int>(0.075f * w));
  x2 = std::min(width, x2 + static_cast<int>(0.075f * w));
  y1 = std::max(0, y1 - static_cast<int>(0.075f * h));
  y2 = std::min(height, y2 + static_cast<int>(0.075f * h));

  ArcternRect rect;
  rect.x = x1;
  rect.y = y1;
  rect.width = x2 - x1;
  rect.height = y2 - y1;
  return rect;
}

int FaceLivenessRgbImpl::GetConfigInfo(arctern::MxnetModel *model) {
    return 0;
}

arctern::ArcternRect FaceLivenessRgbImpl::LocalGetRoi(const RunParameter *p, int idx)  {
    const auto implRunP = dynamic_cast<const FaceLivenessRgbRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  auto rect = check_shrink_box(oriRect, p->imageV_[idx].width, p->imageV_[idx].height);
  return getFace(p->imageV_[idx].width, p->imageV_[idx].height, rect);
}
void FaceLivenessRgbImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
}


/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.21
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include <algorithm>
#include <vector>
#include "faceLivenessIrImpl.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "common/file_buffer.h"
#include "common/algUtil.h"
#include "log/logger.h"

using arctern::FaceLivenessIrImpl;
using arctern::ErrorType;
using arctern::Result;

FaceLivenessIrImpl::FaceLivenessIrImpl() {
  input_h_ = 128;
  input_w_ = 128;
  isNormaliized_ = true;
}

FaceLivenessIrImpl::~FaceLivenessIrImpl() noexcept {
  FaceLivenessIrImpl::Release();
}

ErrorType FaceLivenessIrImpl::CheckRunParam(const RunParameter *p) {
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(p);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) return arcternBaseCheck;
  return ErrorType::ERR_SUCCESS;
}

int FaceLivenessIrImpl::GetConfigInfo(arctern::MxnetModel *model) {
  return 0;
}

ErrorType FaceLivenessIrImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                                std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<FaceLivenessIrResult *>(result_);
  int total = endId - startId;
  int stride = outputs[0].size() / outputs[0].shape()[0];
  for (int subId = 0; subId < total; subId++) {
    const float *data = outputs[0].data() + subId * stride;
    r->faceLiveness_[startId + subId] = data[1];
  }

  return ErrorType::ERR_SUCCESS;
}

arctern::ArcternRect FaceLivenessIrImpl::getFace(int width, int height, const cv::Rect &bbox) {
  int x1 = bbox.tl().x;
  int y1 = bbox.tl().y;
  int x2 = bbox.br().x - 1;  // python implementation lacks 1 elem
  int y2 = bbox.br().y - 1;

  x1 = std::max(0, x1);
  x2 = std::min(width, x2);
  y1 = std::max(0, y1);
  y2 = std::min(height, y2);

  ArcternRect rect;
  rect.x = x1;
  rect.y = y1;
  rect.width = x2 - x1;
  rect.height = y2 - y1;
  return rect;
}


arctern::ArcternRect FaceLivenessIrImpl::LocalGetRoi(const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const FaceLivenessIrRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  auto rect = check_shrink_box(oriRect, p->imageV_[idx].width, p->imageV_[idx].height);
  return  getFace( p->imageV_[idx].width, p->imageV_[idx].height, rect);
}

void FaceLivenessIrImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm)  {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);

  if (isNormaliized_) {
    runParm->sequence_.push_back(OperatorType::normalized);
  }
  runParm->sequence_.push_back(OperatorType::resized);
}



/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.21
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include <algorithm>
#include <vector>
#include "faceQualityPoseImpl.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "common/file_buffer.h"
#include "common/algUtil.h"
#include "log/logger.h"

using arctern::FaceQualityPoseImpl;
using arctern::ErrorType;
using arctern::Result;

FaceQualityPoseImpl::FaceQualityPoseImpl() {
  input_h_ = 112;
  input_w_ = 96;
}

FaceQualityPoseImpl::~FaceQualityPoseImpl() noexcept {
  FaceQualityPoseImpl::Release();
}

ErrorType FaceQualityPoseImpl::CheckRunParam(const RunParameter *p) {
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(p);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) return arcternBaseCheck;
  return ErrorType::ERR_SUCCESS;
}

ErrorType FaceQualityPoseImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                                std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<FaceQualityPoseResult *>(result_);
  int total = endId - startId;
  int stride = outputs[0].size() / outputs[0].shape()[0];
  for (int subId = 0; subId < total; subId++) {
    const float *data = outputs[0].data() + subId * stride;
    r->faceQualityPose_[startId + subId].quality = 1.0f - data[0];
    r->faceQualityPose_[startId + subId].pitch = data[1] * 100.f;
    r->faceQualityPose_[startId + subId].yaw = data[2] * 100.f;
    r->faceQualityPose_[startId + subId].roll = data[3] * 100.0f;
  }

  return ErrorType::ERR_SUCCESS;
}

cv::Rect FaceQualityPoseImpl::getFace(int width, int height, const cv::Rect &bbox) {
  float ext_scale[] = {-1.0f, 0.083f, -1.0f, 0.083f};
  ///  int w = bbox.width;
  int h = bbox.height;
  float x1 = bbox.tl().x;
  float y1 = bbox.tl().y;
  float x2 = bbox.br().x;
  float y2 = bbox.br().y;

  y1 = y1 - h * ext_scale[1];
  y2 = y2 + h * ext_scale[3];
  float by1 = std::min(std::max(0, static_cast<int>(round(y1))), height - 1);
  float by2 = std::min(std::max(0, static_cast<int>(round(y2))), height - 1);
  float bx1 = std::min(std::max(0, static_cast<int>(round(x1))), width - 1);
  float bx2 = std::min(std::max(0, static_cast<int>(round(x2))), width - 1);

  return cv::Rect(cv::Point(bx1, by1), cv::Point(bx2 - 1, by2 - 1));
}

int FaceQualityPoseImpl::GetConfigInfo(arctern::MxnetModel *model) {
  //initPreProcessor();
  return 0;
}

arctern::ArcternRect FaceQualityPoseImpl::LocalGetRoi(const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const FaceQualityPoseRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  auto rect = check_shrink_box(oriRect, p->imageV_[idx].width, p->imageV_[idx].height);
  rect = getFace( p->imageV_[idx].width, p->imageV_[idx].height, rect);
  rect.width -= 1;
  rect.height -= 1;
  ArcternRect ret;
  ret.x = rect.x;
  ret.y = rect.y;
  ret.width = rect.width;
  ret.height = rect.height;
  return ret;
}
void FaceQualityPoseImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
}

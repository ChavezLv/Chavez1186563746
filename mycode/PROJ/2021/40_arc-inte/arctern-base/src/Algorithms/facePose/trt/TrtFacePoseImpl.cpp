//
// Created by Admin on 2021/6/18.
//

#include "TrtFacePoseImpl.h"
#include "src/common/common.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
namespace arctern {

TrtFacePoseImpl::TrtFacePoseImpl() {
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
}

TrtFacePoseImpl::~TrtFacePoseImpl() {

}

ErrorType
TrtFacePoseImpl::GetModelsConfigInfo(const MxnetModels *models) {
  ErrorType vResult;
  m_modelsInputHeight = 64;
  m_modelsInputWidth = 64;
  return vResult;
}
ArcternRect TrtFacePoseImpl::GetFace(int width, int height, const ArcternRect &bbox) {
  float scale = 1.1f;
  int w = bbox.width;
  int h = bbox.height;
  float x1 = bbox.x;
  float y1 = bbox.y;

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

  ArcternRect ret;
  ret.x = x1;
  ret.y = y1;
  ret.width = x2-x1;
  ret.height = y2-y1;
  return ret;
}
ErrorType TrtFacePoseImpl::LocalPostProcess(const RunParameter *p,
                                                     int startId,
                                                     int endId,
                                                     vector<arctern::OutputTensor<float>> &outputs) {
  auto r = dynamic_cast<FacePoseResult *>(m_pResult);

  int total = endId - startId;

  int stride = outputs[0].size() / outputs[0].shape()[0];
  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride;
    const float *data1 = outputs[1].data() + subId * stride;
    const float *data2 = outputs[2].data() + subId * stride;
    r->facePose_[startId + subId].pitch = CalculateResult(data0, stride);
    r->facePose_[startId + subId].yaw = CalculateResult(data1, stride);
    r->facePose_[startId + subId].roll = CalculateResult(data2, stride);
  }

  return ErrorType::ERR_SUCCESS;
}

ArcternRect TrtFacePoseImpl::LocalGetROI(const RunParameter *p, int idx) {
  const auto pRunParam = dynamic_cast<const RunParameter *>(p);
  auto &faceRect = pRunParam->rectsV_[idx];

  auto rect = CheckShrinkBox(faceRect, p->imageV_[idx].width, p->imageV_[idx].height);
  rect = TrtFacePoseImpl::GetFace(p->imageV_[idx].width, p->imageV_[idx].height, rect);
  return rect;
}

void TrtFacePoseImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}

FacePoseLevel TrtFacePoseImpl::CalculateResult(const float *data, int step) {
  int distance = std::distance(data, std::max_element(data, data + step));
  return arctern::FacePoseLevel(distance);
}

}

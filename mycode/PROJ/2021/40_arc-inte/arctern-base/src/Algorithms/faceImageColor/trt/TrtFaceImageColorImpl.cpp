//
// Created by Admin on 2021/6/18.
//

#include "TrtFaceImageColorImpl.h"
#include "src/common/common.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
namespace arctern {

TrtFaceImageColorImpl::TrtFaceImageColorImpl() : m_extScale{0,0.2,0,0} {
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
}

TrtFaceImageColorImpl::~TrtFaceImageColorImpl() {

}

ErrorType
TrtFaceImageColorImpl::GetModelsConfigInfo(const MxnetModels *models) {
  ErrorType vResult;
  m_modelsInputHeight = 64;
  m_modelsInputWidth = 64;
  return vResult;
}

ErrorType TrtFaceImageColorImpl::LocalPostProcess(const RunParameter *p,
                                                     int startId,
                                                     int endId,
                                                     vector<arctern::OutputTensor<float>> &outputs) {
  auto r = dynamic_cast<FaceImageColorResult *>(m_pResult);

  int total = endId - startId;

  int stride0 = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    r->faceImageColors_[startId + subId] = data0[1];
  }

  return ErrorType::ERR_SUCCESS;
}
ArcternRect TrtFaceImageColorImpl::GetFace(const int width, const int height, const ArcternRect &rect) {
  int x1 = rect.x;
  int y1 = rect.y;
  int x2 = x1 + width - 1;  // python implementation lacks 1 elem
  int y2 = y1 + height - 1;
  x1 = std::max(0, x1);
  x2 = std::min(width, x2);
  y1 = std::max(0, y1);
  y2 = std::min(height, y2);
  ArcternRect ret;
  ret.x = x1;
  ret.y = y1;
  ret.width = x2-x1;
  ret.height = y2-y1;
  return ret;
}
ArcternRect TrtFaceImageColorImpl::LocalGetROI(const RunParameter *p, int idx) {
  const auto pRunParam = dynamic_cast<const RunParameter *>(p);
  auto &faceRect = pRunParam->rectsV_[idx];
  auto rect = CheckShrinkBox(faceRect, p->imageV_[idx].width, p->imageV_[idx].height);
  rect = TrtFaceImageColorImpl::GetFace(p->imageV_[idx].width, p->imageV_[idx].height, rect);
  return rect;
}

void TrtFaceImageColorImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}


}

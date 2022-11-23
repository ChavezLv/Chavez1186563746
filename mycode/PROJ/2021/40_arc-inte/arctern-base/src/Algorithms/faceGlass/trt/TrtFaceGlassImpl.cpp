//
// Created by Admin on 2021/6/18.
//

#include "TrtFaceGlassImpl.h"
#include "src/common/common.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
namespace arctern {

TrtFaceGlassImpl::TrtFaceGlassImpl() :m_extScale{0,0.2,0,0}{
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
}

TrtFaceGlassImpl::~TrtFaceGlassImpl() {

}

ErrorType
TrtFaceGlassImpl::GetModelsConfigInfo(const MxnetModels *models) {
  ErrorType vResult;


  m_modelsInputHeight = 128;
  m_modelsInputWidth = 128;


  return vResult;
}

ErrorType TrtFaceGlassImpl::LocalPostProcess(const RunParameter *p,
                                                     int startId,
                                                     int endId,
                                                     vector<arctern::OutputTensor<float>> &outputs) {
  auto r = dynamic_cast<FaceGlassResult *>(m_pResult);

  int total = endId - startId;
  int stride0 = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    auto glasses = CalculateResult(data0, stride0);

    r->FaceGlasss_[startId + subId].glassesStatus = GlassesStatus(glasses.first);
    r->FaceGlasss_[startId + subId].statusScore = glasses.second;
  }

  return ErrorType::ERR_SUCCESS;
}

ArcternRect TrtFaceGlassImpl::LocalGetROI(const RunParameter *p, int idx) {
  const auto pRunParam = dynamic_cast<const RunParameter *>(p);
  auto &faceRect = pRunParam->rectsV_[idx];
  auto rect = CheckShrinkBox(faceRect, p->imageV_[idx].width, p->imageV_[idx].height);
  rect = GetFace(p->imageV_[idx].width, p->imageV_[idx].height, rect, m_extScale);
  rect.width -= 1;
  rect.height -= 1;
  return rect;
}

void TrtFaceGlassImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}

pair<int, float> TrtFaceGlassImpl::CalculateResult(const float *data, int step) {
  std::vector<float> prob;
  for (size_t j = 0; j < step; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}

}

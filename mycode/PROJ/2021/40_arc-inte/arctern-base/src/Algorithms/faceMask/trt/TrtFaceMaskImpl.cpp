//
// Created by Admin on 2021/5/19.
//
#include "TrtFaceMaskImpl.h"
#include "src/common/common.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
namespace arctern {

TrtFaceMaskImpl::TrtFaceMaskImpl() : m_extScale{0.0, 0.1, 0.0, 0.1}{
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
}

TrtFaceMaskImpl::~TrtFaceMaskImpl() {

}

ErrorType
TrtFaceMaskImpl::GetModelsConfigInfo(const MxnetModels *models) {
  ErrorType vResult;
  m_modelsInputHeight = m_modelsInputWidth = 128;
  return vResult;
}

ErrorType TrtFaceMaskImpl::LocalPostProcess(const RunParameter *p,
                                                     int startId,
                                                     int endId,
                                                     vector<arctern::OutputTensor<float>> &outputs) {
  auto r = dynamic_cast<FaceMaskResult *>(m_pResult);

  int total = endId - startId;

  int stride0 = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    auto gender = CalculateResult(data0, stride0);

    r->FaceMasks_[startId + subId].maskStatus = gender.first == 0 ? MASK : NOMASK;
    r->FaceMasks_[startId + subId].statusScore = gender.second;
  }

  return ErrorType::ERR_SUCCESS;
}
void TrtFaceMaskImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}
ArcternRect TrtFaceMaskImpl::LocalGetROI(const RunParameter *p, int idx) {
  const auto pRunParam = dynamic_cast<const RunParameter *>(p);
  auto &faceRect = pRunParam->rectsV_[idx];
  auto roiRect = CheckShrinkBox(faceRect, p->imageV_[idx].width, p->imageV_[idx].height);
  auto rect = GetFace(pRunParam->imageV_[idx].width, pRunParam->imageV_[idx].height, roiRect, m_extScale);
  rect.width -= 1;
  rect.height -= 1;
  return rect;
}

pair<int, float> TrtFaceMaskImpl::CalculateResult(const float *data, int step) {
  std::vector<float> prob;
  for (size_t j = 0; j < step; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(1 - max_idx, max_score);
}

}

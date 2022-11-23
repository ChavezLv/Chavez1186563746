//
// Created by Admin on 2021/6/21.
//

#include "TrtFaceHatImpl.h"
#include "TrtFaceHatImpl.h"
#include "src/common/common.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
namespace arctern {
TrtFaceHatImpl::TrtFaceHatImpl() : m_extScale{0.0,1.5,0.0,1.5} {
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
}

TrtFaceHatImpl::~TrtFaceHatImpl() {

}

ErrorType TrtFaceHatImpl::GetModelsConfigInfo(const MxnetModels *models) {
  ErrorType vResult;
  m_modelsInputWidth = m_modelsInputHeight = 128;
  return vResult;
}

void TrtFaceHatImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}

ArcternRect TrtFaceHatImpl::LocalGetROI(const RunParameter *p, int idx) {
  const auto pRunParam = dynamic_cast<const RunParameter *>(p);
  auto &faceRect = pRunParam->rectsV_[idx];
  auto roiRect = CheckShrinkBox(faceRect,pRunParam->imageV_[idx].width,pRunParam->imageV_[idx].height);
  auto rect = GetFace(p->imageV_[idx].width, p->imageV_[idx].height, roiRect, m_extScale);
  return rect;
}

ErrorType TrtFaceHatImpl::LocalPostProcess(const RunParameter *p,
                                             int startId,
                                             int endId,
                                             vector<arctern::OutputTensor<float>> &outputs) {
  assert(p != nullptr);
  auto r = dynamic_cast<FaceHatResult *>(m_pResult);

  int total = endId - startId;

  int stride0 = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    auto hat = CalculateResult(data0, stride0);

    r->FaceHats_[startId + subId].hatStatus = HatStatus(hat.first);
    r->FaceHats_[startId + subId].statusScore = hat.second;
  }

  return ErrorType::ERR_SUCCESS;
}

std::pair<int, float> TrtFaceHatImpl::CalculateResult(const float *data, int step) {
  std::vector<float> prob;
  for (size_t j = 0; j < step; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}
}
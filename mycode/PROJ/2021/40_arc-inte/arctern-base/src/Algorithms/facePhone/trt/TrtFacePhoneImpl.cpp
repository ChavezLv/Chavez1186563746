//
// Created by Admin on 2021/5/19.
//
#include "TrtFacePhoneImpl.h"
#include "src/common/common.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
namespace arctern {

TrtFacePhoneImpl::TrtFacePhoneImpl() :m_extScale{0.0,0.5,0.0,0.1} {
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
}

TrtFacePhoneImpl::~TrtFacePhoneImpl() {

}

ErrorType
TrtFacePhoneImpl::GetModelsConfigInfo(const MxnetModels *models) {
  ErrorType vResult;
  m_modelsInputWidth = m_modelsInputHeight = 255;
  return vResult;
}

ErrorType TrtFacePhoneImpl::LocalPostProcess(const RunParameter *p,
                                             int startId,
                                             int endId,
                                             vector<arctern::OutputTensor<float>> &outputs) {
  auto r = dynamic_cast<FacePhoneResult *>(m_pResult);

  int total = endId - startId;

  int stride0 = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    auto phone = CalculateResult(data0, stride0);

    r->facePhones_[startId + subId].phoneStatus = PhoneStatus(phone.first);
    r->facePhones_[startId + subId].statusScore = phone.second;
  }

  return ErrorType::ERR_SUCCESS;
}
void TrtFacePhoneImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}
ArcternRect TrtFacePhoneImpl::LocalGetROI(const RunParameter *p, int idx) {
  const auto pRunParam = dynamic_cast<const RunParameter *>(p);
  auto &faceRect = pRunParam->rectsV_[idx];
  return GetFace(pRunParam->imageV_[idx].width, pRunParam->imageV_[idx].height, faceRect, m_extScale, Float2Int::TRUNCATION);;
}


pair<int, float> TrtFacePhoneImpl::CalculateResult(const float *data, int step) {
  std::vector<float> prob;
  for (size_t j = 0; j < step; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}

}

//
// Created by Admin on 2021/5/19.
//
#include "TrtFaceQualityImpl.h"
#include "src/common/common.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
namespace arctern {

TrtFaceQualityImpl::TrtFaceQualityImpl() {

}

TrtFaceQualityImpl::~TrtFaceQualityImpl() {

}

ErrorType
TrtFaceQualityImpl::GetModelsConfigInfo(const MxnetModels *models) {
  ErrorType vResult;
  auto model = models->models()->Get(0);

  bool ret = try_get_value(*model, "GRAY_IMG_INPUT", m_bGrayInput);
  assert(ret == true);

  ret = try_get_value(*model, "BRANCH_NUM", m_outputNum);
  assert(ret == true);

  /// netFormat_ = isGrayInput_ ? NetworkFormat::GRAY : NetworkFormat::C_RGB;
  m_dstImageFormatForForward = m_bGrayInput ? ARCTERN_IMAGE_FORMAT_GRAY : ARCTERN_IMAGE_FORMAT_RGB888;

  m_modelsInputWidth = m_modelsInputHeight = 50;
  return vResult;
}

ErrorType TrtFaceQualityImpl::LocalPostProcess(const RunParameter *p,
                                             int startId,
                                             int endId,
                                             vector<arctern::OutputTensor<float>> &outputs) {
  auto r = dynamic_cast<FaceQualityResult *>(m_pResult);

  int total = endId - startId;
  int stride0 = outputs[0].size() / outputs[0].shape()[0];

  bool secondOutput = outputs.size() > 1;
  int stride1 = secondOutput ? outputs[1].size() / outputs[1].shape()[0] : 0;

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    const float *data1 = secondOutput ? outputs[1].data() + subId * stride1 : nullptr;
    r->faceQuality_[startId + subId] = CalculateResult(data0, data1);
  }

  return ErrorType::ERR_SUCCESS;
}
void TrtFaceQualityImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);

  //runParm->sequence_.push_back(OperatorType::extractChannel);
}
ArcternRect TrtFaceQualityImpl::LocalGetROI(const RunParameter *p, int idx) {
  const auto pRunParam = dynamic_cast<const RunParameter *>(p);
  auto &faceRect = pRunParam->rectsV_[idx];
  return CheckShrinkBox(faceRect, pRunParam->imageV_[idx].width, pRunParam->imageV_[idx].height);
}


float TrtFaceQualityImpl::CalculateResult(const float *data0, const float *data1) {
  if (m_outputNum > 1) {
    return data1[0] > data1[1] ? 0 : data0[0];
  } else {
    return data0[0];
  }
}

}

//
// Created by Admin on 2021/6/21.
//

#include "TrtFaceExpressionImpl.h"
#include "src/common/common.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
namespace arctern {
TrtFaceExpressionImpl::TrtFaceExpressionImpl() :m_extScale{0.0,0.5,0.0,0.1} {
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_GRAY;
}

TrtFaceExpressionImpl::~TrtFaceExpressionImpl() {

}

ErrorType TrtFaceExpressionImpl::GetModelsConfigInfo(const MxnetModels *models) {
  ErrorType vResult;
  m_modelsInputWidth = m_modelsInputHeight = 96;
  return vResult;
}

ArcternRect TrtFaceExpressionImpl::LocalGetROI(const RunParameter *p, int idx) {
  const auto pRunParam = dynamic_cast<const RunParameter *>(p);
  auto &faceRect = pRunParam->rectsV_[idx];
  auto rect =  CheckShrinkBox(faceRect, p->imageV_[idx].width, p->imageV_[idx].height);
  return rect;
}

ErrorType TrtFaceExpressionImpl::LocalPostProcess(const RunParameter *p,
                                             int startId,
                                             int endId,
                                             vector<arctern::OutputTensor<float>> &outputs) {
  auto r = dynamic_cast<FaceExpressionResult *>(m_pResult);

  int total = endId - startId;
  int stride0 = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    auto result = CalculateResult(data0, stride0);
    r->faceExpressions_[startId + subId].type = arctern::ExpressionType(result.first);
    r->faceExpressions_[startId + subId].score = result.second;
  }

  return ErrorType::ERR_SUCCESS;
}

std::pair<int, float> TrtFaceExpressionImpl::CalculateResult(const float *data, int step) {
  std::vector<float> prob;
  for (size_t j = 0; j < step; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}
void TrtFaceExpressionImpl::InitPreprocessorParam(){
  m_pStream = m_vEngines[0]->getContext()->getCudaStream();
  m_pPreprocessor.reset(new NppPreprocessor(m_pStream));

  NppPreprocessorInitPt pPreprocessorInitParam ;
  pPreprocessorInitParam.netH_ = m_modelsInputHeight;
  pPreprocessorInitParam.netW_ = m_modelsInputWidth;
  pPreprocessorInitParam.mean_ = 0;
  pPreprocessorInitParam.std_ = 255;
  assert(m_pPreprocessor);
  m_lastErrorCode = m_pPreprocessor->Init(&pPreprocessorInitParam);
  assert(m_lastErrorCode == ErrorType::ERR_SUCCESS);
}
void TrtFaceExpressionImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){

  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);

  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::faceExp);

  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::merge);
}

}

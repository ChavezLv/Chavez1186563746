//
// Created by Admin on 2021/6/21.
//

#include "TrtFaceBangsImpl.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
namespace arctern {
TrtFaceBangsImpl::TrtFaceBangsImpl() :  m_extScale {0.0,0.5,0.0,0.1} {
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
}

TrtFaceBangsImpl::~TrtFaceBangsImpl() {

}

ErrorType TrtFaceBangsImpl::GetModelsConfigInfo(const MxnetModels *models) {
  m_modelsInputWidth = m_modelsInputHeight = 128;
  return ERR_SUCCESS;
}
void TrtFaceBangsImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}
ArcternRect TrtFaceBangsImpl::LocalGetROI(const RunParameter *p, int idx) {
  const auto pRunParam = dynamic_cast<const RunParameter *>(p);
  auto &faceRect = pRunParam->rectsV_[idx];
  auto rect = GetFace(p->imageV_[idx].width, p->imageV_[idx].height, faceRect, m_extScale, Float2Int::TRUNCATION);
  return rect;
}

ErrorType TrtFaceBangsImpl::LocalPostProcess(const RunParameter *p,
                                             int startId,
                                             int endId,
                                             vector<arctern::OutputTensor<float>> &outputs) {
  assert(p != nullptr);
  auto r = dynamic_cast<FaceBangsResult*>(m_pResult) ;
  if(r == nullptr) return ERR_NULL_POINT;

  int total = endId - startId;

  int stride0 = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    auto result = CalculateResult(data0, stride0);
    r->faceBangss_[startId + subId].status = arctern::BangsStatus(result.first);
    r->faceBangss_[startId + subId].statusScore = result.second;
  }

  return ErrorType::ERR_SUCCESS;
}

std::pair<int, float> TrtFaceBangsImpl::CalculateResult(const float *data, int step) {
  std::vector<float> prob;
  for (size_t j = 0; j < step; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}
}
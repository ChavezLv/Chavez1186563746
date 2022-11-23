//
// Created by Admin on 2021/6/18.
//

#include "TrtFaceHeadwearImpl.h"
#include "src/common/common.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
namespace arctern {

TrtFaceHeadwearImpl::TrtFaceHeadwearImpl() :m_extScale{0.0, 0.7, 0.0, 0.2} {
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
}

TrtFaceHeadwearImpl::~TrtFaceHeadwearImpl() {

}

ErrorType
TrtFaceHeadwearImpl::GetModelsConfigInfo(const MxnetModels *models) {
  ErrorType vResult;
  m_modelsInputHeight = 128;
  m_modelsInputWidth = 128;
  return vResult;
}

ErrorType TrtFaceHeadwearImpl::LocalPostProcess(const RunParameter *p,
                                                int startId,
                                                int endId,
                                                vector<arctern::OutputTensor<float>> &outputs) {
  auto r = dynamic_cast<FaceHeadwearResult *>(m_pResult);

  int total = endId - startId;

  int stride0 = outputs[0].size() / outputs[0].shape()[0];
  int stride1 = outputs[1].size() / outputs[1].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    auto result = CalculateResult(data0, stride0);
    r->faceHeadwears_[startId + subId].color = arctern::HeadwearColor(result.first);
    r->faceHeadwears_[startId + subId].colorScore = result.second;

    const float *data1 = outputs[1].data() + subId * stride1;
    auto result1 = CalculateResult(data1, stride1);
    r->faceHeadwears_[startId + subId].type = arctern::HeadwearType(result1.first);
    r->faceHeadwears_[startId + subId].typeScore = result1.second;
  }

  return ErrorType::ERR_SUCCESS;
}

ArcternRect TrtFaceHeadwearImpl::LocalGetROI(const RunParameter *p, int idx) {
  const auto pRunParam = dynamic_cast<const RunParameter *>(p);
  auto &faceRect = pRunParam->rectsV_[idx];
  auto rect = GetFace(p->imageV_[idx].width, p->imageV_[idx].height, faceRect, m_extScale);
  return rect;
}

void TrtFaceHeadwearImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}

pair<int, float> TrtFaceHeadwearImpl::CalculateResult(const float *data, int step) {
  std::vector<float> prob;
  prob.assign(data, data + step);
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}

}

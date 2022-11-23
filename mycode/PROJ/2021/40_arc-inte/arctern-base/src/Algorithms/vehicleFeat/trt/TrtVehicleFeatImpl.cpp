//
// Created by Admin on 2021/6/18.
//

#include "TrtVehicleFeatImpl.h"
#include "src/common/common.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
#include "src/common/featureUtil.h"
namespace arctern {

TrtVehicleFeatImpl::TrtVehicleFeatImpl() {

}

TrtVehicleFeatImpl::~TrtVehicleFeatImpl() {

}

void TrtVehicleFeatImpl::InitPreprocessorParam(){
  m_pStream = m_vEngines[0]->getContext()->getCudaStream();
  m_pPreprocessor.reset(new NppPreprocessor(m_pStream));
  NppPreprocessorInitPt pPreprocessorInitParam ;

  pPreprocessorInitParam.netH_ = m_modelsInputHeight;
  pPreprocessorInitParam.netW_ = m_modelsInputWidth;
  pPreprocessorInitParam.mean_ = 127.5;
  pPreprocessorInitParam.std_ = 1.0;
  assert(m_pPreprocessor);
  m_lastErrorCode = m_pPreprocessor->Init(&pPreprocessorInitParam);
  assert(m_lastErrorCode == ErrorType::ERR_SUCCESS);
}

ErrorType
TrtVehicleFeatImpl::GetModelsConfigInfo(const MxnetModels *models) {
  
  m_modelsInputHeight = 224;
  m_modelsInputWidth = 224;

  return ErrorType::ERR_SUCCESS;
}

ErrorType TrtVehicleFeatImpl::LocalPostProcess(const RunParameter *p,
                                                     int startId,
                                                     int endId,
                                                     vector<arctern::OutputTensor<float>> &outputs) {
  auto res = dynamic_cast<vehicleFeatResult *>(m_pResult);
  int total = endId - startId;
  int stride = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data = outputs[0].data() + subId * stride;
    std::vector<float> data_cls(stride);
    memcpy(data_cls.data(), data, stride * sizeof(float));

    l2norm(data_cls);

    res->m_vFeatResults[startId + subId] = encrypt_feature(data_cls);
  }

  return ErrorType::ERR_SUCCESS;
}

ArcternRect TrtVehicleFeatImpl::LocalGetROI(const RunParameter *p, int idx) {
  const auto pRunParam = dynamic_cast<const RunParameter *>(p);
  auto &faceRect = pRunParam->rectsV_[idx];
  auto rect = CheckShrinkBox(faceRect, p->imageV_[idx].width, p->imageV_[idx].height);
  return rect;
}

void TrtVehicleFeatImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}


}

//
// Created by Admin on 2021/6/18.
//

#include "TrtLicenseLandmarkImpl.h"
#include "src/common/common.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
namespace arctern {

TrtLicenseLandmarkImpl::TrtLicenseLandmarkImpl() {
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
}

TrtLicenseLandmarkImpl::~TrtLicenseLandmarkImpl() {

}

void TrtLicenseLandmarkImpl::InitPreprocessorParam() {
  m_pStream = m_vEngines[0]->getContext()->getCudaStream();
  m_pPreprocessor.reset(new NppPreprocessor(m_pStream));

  NppPreprocessorInitPt pPreprocessorInitParam ;
  pPreprocessorInitParam.netH_ = m_modelsInputHeight;
  pPreprocessorInitParam.netW_ = m_modelsInputWidth;
  pPreprocessorInitParam.meanV_ = {0.485, 0.456, 0.406};
  pPreprocessorInitParam.stdV_ = {0.229, 0.224, 0.225};
  pPreprocessorInitParam.borderValue_ = 0;
  assert(m_pPreprocessor);
  m_lastErrorCode = m_pPreprocessor->Init(&pPreprocessorInitParam);
  assert(m_lastErrorCode == ErrorType::ERR_SUCCESS);
}

ErrorType
TrtLicenseLandmarkImpl::GetModelsConfigInfo(const MxnetModels *models) {
  m_modelsInputHeight = 128;
  m_modelsInputWidth = 256;
  return ErrorType::ERR_SUCCESS;
}

ErrorType TrtLicenseLandmarkImpl::LocalPostProcess(const RunParameter *p,
                                                int startId,
                                                int endId,
                                                vector<arctern::OutputTensor<float>> &outputs) {
  auto r = dynamic_cast<LicenseLandmarkResult *>(m_pResult);
  int total = endId - startId;
  int stride = outputs[0].size() / outputs[0].shape()[0];
  for (int subId = 0; subId < total; subId++) {
    auto landmark = outputs[0].data() + subId * stride;

    float scale_x = float(p->imageV_[startId+subId].width) / float(m_modelsInputWidth);
    float scale_y = float(p->imageV_[startId+subId].height) / float(m_modelsInputHeight);
    cout << __func__ << ": " << __LINE__ << ": " << p->imageV_[startId+subId].width << ", and input_w_ = " << m_modelsInputWidth<< endl;
    cout << __func__ << ": " << __LINE__ << ": " << p->imageV_[startId+subId].height << ", and input_w_ = " << m_modelsInputHeight << endl;
    cout << __func__ << ": " << __LINE__ << ": " << scale_x << ", and scale_y = " << scale_y << endl;

    std::vector<int> result;
    for(int i=0; i < 8; i=i+2)
    {
      arctern::ArcternPoint point;
      point.x = int(landmark[i] * scale_x + 0.5);
      point.y = int(landmark[i+1] * scale_y + 0.5);
      cout << __func__ << ": " << __LINE__ << ": landmark: " << landmark[i] << ", "\
           << landmark[i + 1] << ", (" << point.x << ", " << point.y << "). " << endl;
      r->LicenseLandmarks_[startId + subId].emplace_back(point);
    }
  }
  return ErrorType::ERR_SUCCESS;
}


void TrtLicenseLandmarkImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){

  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  /* runParm->sequence_.push_back(OperatorType::splitNormalized); */
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}


}

//
// Created by Admin on 2021/5/19.
//
#include "TrtCrowdHeatMapImpl.h"
#include "src/common/common.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
namespace arctern {

TrtCrowdHeatMapImpl::TrtCrowdHeatMapImpl() : m_extScale{0.0, 0.1, 0.0, 0.1}{
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_BGR888;
}

TrtCrowdHeatMapImpl::~TrtCrowdHeatMapImpl() {

}

ErrorType
TrtCrowdHeatMapImpl::GetModelsConfigInfo(const MxnetModels *models) {
  m_modelsInputHeight = 432;
  m_modelsInputWidth = 768;
  return ERR_SUCCESS;
}

void TrtCrowdHeatMapImpl::InitPreprocessorParam() {
  m_pStream = m_vEngines[0]->getContext()->getCudaStream();
  m_pPreprocessor.reset(new NppPreprocessor(m_pStream));

  NppPreprocessorInitPt pPreprocessorInitParam ;
  pPreprocessorInitParam.netH_ = m_modelsInputHeight;
  pPreprocessorInitParam.netW_ = m_modelsInputWidth;
  pPreprocessorInitParam.meanV_ = {0.431981861591, 0.447249650955, 0.452016860247};//bgr
  pPreprocessorInitParam.stdV_ = {0.221840232611, 0.224925786257, 0.23242045939};
  pPreprocessorInitParam.borderValue_ = 0;
  assert(m_pPreprocessor);
  m_lastErrorCode = m_pPreprocessor->Init(&pPreprocessorInitParam);
  assert(m_lastErrorCode == ErrorType::ERR_SUCCESS);
}

ErrorType TrtCrowdHeatMapImpl::LocalPostProcess(const RunParameter *p,
                                                     int startId,
                                                     int endId,
                                                     vector<arctern::OutputTensor<float>> &outputs) {
  auto r = dynamic_cast<CrowdHeatmapResult *>(m_pResult);
  int total = endId - startId;
  int res_h = 54, res_w = 96;
  int stride0 = res_w * res_h;

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    cv::Mat fimage(res_h, res_w, CV_32FC1);
    std::memcpy(fimage.data, data0, sizeof(float)*stride0);
    cv::resize(fimage, fimage, cv::Size(m_modelsInputWidth, m_modelsInputHeight), CV_INTER_NN);
    r->crowdHeatmaps_[startId + subId].headCount = cv::sum(fimage)[0]/100;
    double maxv=0.0;
    cv::minMaxIdx(fimage, 0, &maxv);
    fimage = fimage/(maxv + 1e-20)*255;
    cv::imencode(".jpg", fimage, r->crowdHeatmaps_[startId + subId].heatmap);
  }

  return ErrorType::ERR_SUCCESS;
}
void TrtCrowdHeatMapImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){


  runParm->sequence_.push_back(OperatorType::splitNormalized);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}

}

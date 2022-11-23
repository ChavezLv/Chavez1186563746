//
// Created by chengaohua on 2021/6/24.
//

#include "TrtFaceSmokeImpl.h"
#include "src/Algorithms/faceSmoke/faceSmoke.h"
#include "../utils/faceSmokeUtil.h"

namespace arctern {
TrtFaceSmokeImpl::TrtFaceSmokeImpl() {
  m_modelsInputWidth = 255;
  m_modelsInputHeight = 255;
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
}

TrtFaceSmokeImpl::~TrtFaceSmokeImpl() throw() {

}

ErrorType TrtFaceSmokeImpl::GetModelsConfigInfo(const MxnetModels *models) {
  ErrorType result;
  return result;
}

ErrorType TrtFaceSmokeImpl::LocalPostProcess(const RunParameter *p,
                                             int startId,
                                             int endId,
                                             vector<arctern::OutputTensor<float>> &outputs) {

  auto r = dynamic_cast<FaceSmokeResult *>(m_pResult);

  int total = endId - startId;

  // const auto batchNum = initParam_.batchNum_;
  int stride0 = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    auto phone = smoke::calcSmoke(data0, stride0);

    r->faceSmokes_[startId + subId].smokeStatus = SmokeStatus(phone.first);
    r->faceSmokes_[startId + subId].statusScore = phone.second;
  }

  return ErrorType::ERR_SUCCESS;
}

ArcternRect TrtFaceSmokeImpl::LocalGetROI(const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const FaceSmokeRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  auto rect = smoke::getFace(p->imageV_[idx].width, p->imageV_[idx].height, oriRect);
  ArcternRect result;
  result.x = rect.x;
  result.y = rect.y;
  result.width = rect.width;
  result.height = rect.height;
  return result;
}

void TrtFaceSmokeImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}

}
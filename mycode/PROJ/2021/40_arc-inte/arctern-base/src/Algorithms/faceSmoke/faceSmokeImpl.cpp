/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.28
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include <vector>
#include "faceSmokeImpl.h"
#include "common/imageConvert.h"
#include "common/algUtil.h"
#include "utils/faceSmokeUtil.h"

using arctern::FaceSmokeImpl;
using arctern::ErrorType;
using arctern::Result;

FaceSmokeImpl::FaceSmokeImpl() {
  input_h_ = 255;
  input_w_ = 255;
}

FaceSmokeImpl::~FaceSmokeImpl() noexcept {
  FaceSmokeImpl::Release();
}

ErrorType FaceSmokeImpl::CheckRunParam(const RunParameter *p) {
  auto rp = dynamic_cast<const FaceSmokeRunParameter *>(p);
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(rp);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) return arcternBaseCheck;
  return ErrorType::ERR_SUCCESS;
}

ErrorType FaceSmokeImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                          std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<FaceSmokeResult *>(result_);

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

arctern::ArcternRect FaceSmokeImpl::LocalGetRoi(const RunParameter *p, int idx) {
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
void FaceSmokeImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
}





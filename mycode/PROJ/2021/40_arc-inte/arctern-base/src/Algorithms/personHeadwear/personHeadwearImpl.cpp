/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.01
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include <vector>
#include "personHeadwearImpl.h"
#include "common/imageConvert.h"
#include "common/algUtil.h"
#include "utils/personHeadwearUtil.h"

using arctern::PersonHeadwearImpl;
using arctern::ErrorType;
using arctern::Result;
using arctern::ArcternRect;

PersonHeadwearImpl::PersonHeadwearImpl() : extScale_{0, 0.15, 0, 0} {
  input_h_ = 320;
  input_w_ = 128;
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_BGR888;
}

PersonHeadwearImpl::~PersonHeadwearImpl() noexcept {
  PersonHeadwearImpl::Release();
}

ErrorType PersonHeadwearImpl::CheckRunParam(const RunParameter *p) {
  return ArcternAlgorithm::CheckRunParam(p);
}

ErrorType PersonHeadwearImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                        std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<PersonHeadwearResult *>(result_);

  int total = endId - startId;

  // const auto batchNum = initParam_.batchNum_;
  int stride0 = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    auto headwear = personHeadwear::calcHeadwear(data0, stride0);

    r->PersonHeadwears_[startId + subId].headwearType = PersonHeadwearType(headwear.first);
    r->PersonHeadwears_[startId + subId].typeScore = headwear.second;
  }

  return ErrorType::ERR_SUCCESS;
}


ArcternRect PersonHeadwearImpl::LocalGetRoi(const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const PersonHeadwearRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  auto rect = check_shrink_box(oriRect, p->imageV_[idx].width, p->imageV_[idx].height);
  rect = get_face(p->imageV_[idx].width, p->imageV_[idx].height, rect, extScale_, Float2Int::TRUNCATION, false);
  ArcternRect result;
  result.x = rect.x;
  result.y = rect.y;
  result.width = rect.width;
  result.height = rect.height;
  return result;
}
void PersonHeadwearImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::normalized);
}


/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include <utility>
#include <vector>
#include "faceMaskImpl.h"
#include "common/algUtil.h"
#include "faceMask.h"
using arctern::FaceMaskImpl;
using arctern::ErrorType;
using arctern::Result;
using arctern::ArcternRect;

FaceMaskImpl::FaceMaskImpl() : extScale_{0.0, 0.1, 0.0, 0.1} {
  input_h_ = 128;
  input_w_ = 128;
  isNormaliized_ = true;
}

FaceMaskImpl::~FaceMaskImpl()  {
  FaceMaskImpl::Release();
}

ErrorType FaceMaskImpl::CheckRunParam(const RunParameter *p) {
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(p);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) return arcternBaseCheck;
  return ErrorType::ERR_SUCCESS;
}


ErrorType FaceMaskImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                        std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<FaceMaskResult *>(result_);

  int total = endId - startId;

  // const auto batchNum = initParam_.batchNum_;
  int stride0 = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    std::cout<<"-------------------------->"<<data0[0]<<" -------------->"<<data0[1]<<std::endl;

    auto gender = calc_mask(data0, stride0);

    r->FaceMasks_[startId + subId].maskStatus = gender.first == 0 ? MASK : NOMASK;
    r->FaceMasks_[startId + subId].statusScore = gender.second;
  }

  return ErrorType::ERR_SUCCESS;
}

/// \brief calculate mask label and corresponding score
std::pair<int, float> FaceMaskImpl::calc_mask(const float *data, size_t num_class) {
  std::vector<float> prob;
  for (size_t j = 0; j < num_class; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(1 - max_idx, max_score);
}

int FaceMaskImpl::GetConfigInfo(arctern::MxnetModel *model) {
  return 0;
}

ArcternRect FaceMaskImpl::LocalGetRoi(const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const FaceMaskRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);
  auto rect = check_shrink_box(oriRect, p->imageV_[idx].width,p->imageV_[idx].height);
  rect = get_face(p->imageV_[idx].width,p->imageV_[idx].height, rect, extScale_);
  rect.width -= 1;
  rect.height -= 1;
  ArcternRect result;
  result.x = rect.x;
  result.y = rect.y;
  result.width = rect.width;
  result.height = rect.height;
  return result;
}

void FaceMaskImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  if(isNormaliized_) {
    runParm->sequence_.push_back(OperatorType::normalized);
  }
  runParm->sequence_.push_back(OperatorType::resized);
}

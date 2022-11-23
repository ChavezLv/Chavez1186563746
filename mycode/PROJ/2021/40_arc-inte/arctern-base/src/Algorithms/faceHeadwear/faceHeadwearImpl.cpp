/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.25
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include <vector>
#include <utility>
#include "src/Algorithms/faceHeadwear/faceHeadwearImpl.h"
#include "common/algUtil.h"

#include "log/logger.h"

using arctern::FaceHeadwearImpl;
using arctern::ErrorType;
using arctern::Result;

FaceHeadwearImpl::FaceHeadwearImpl() : extScale_{0.0, 0.7, 0.0, 0.2} {
  input_h_ = 128;
  input_w_ = 128;
}

FaceHeadwearImpl::~FaceHeadwearImpl() noexcept {
  FaceHeadwearImpl::Release();
}

ErrorType FaceHeadwearImpl::CheckRunParam(const RunParameter *p) {
  auto rp = dynamic_cast<const FaceHeadwearRunParameter *>(p);
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(rp);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) return arcternBaseCheck;
  return ErrorType::ERR_SUCCESS;
}

ErrorType FaceHeadwearImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                         std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<FaceHeadwearResult *>(result_);

  int total = endId - startId;

  // const auto batchNum = initParam_.batchNum_;
  int stride0 = outputs[0].size() / outputs[0].shape()[0];
  int stride1 = outputs[1].size() / outputs[1].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    auto result = calcResult(data0, stride0);
    r->faceHeadwears_[startId + subId].color = arctern::HeadwearColor(result.first);
    r->faceHeadwears_[startId + subId].colorScore = result.second;

    const float *data1 = outputs[1].data() + subId * stride1;
    auto result1 = calcResult(data1, stride1);
    r->faceHeadwears_[startId + subId].type = arctern::HeadwearType(result1.first);
    r->faceHeadwears_[startId + subId].typeScore = result1.second;
  }

  return ErrorType::ERR_SUCCESS;
}

/// \brief calculate mask label and corresponding score
std::pair<int, float> FaceHeadwearImpl::calcResult(const float *data, size_t num_class) {
  std::vector<float> prob;
  prob.assign(data, data + num_class);
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}

arctern::ArcternRect FaceHeadwearImpl::LocalGetRoi(const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const FaceHeadwearRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  auto rect = get_face(p->imageV_[idx].width, p->imageV_[idx].height,oriRect, extScale_, Float2Int::TRUNCATION);
  ArcternRect result;
  result.x = rect.x;
  result.y = rect.y;
  result.width = rect.width;
  result.height = rect.height;
  return result;
}
void FaceHeadwearImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::normalized);
}



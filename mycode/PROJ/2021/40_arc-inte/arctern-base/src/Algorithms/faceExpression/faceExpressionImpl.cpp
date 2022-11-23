/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.24
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include <vector>
#include <utility>
#include "src/Algorithms/faceExpression/faceExpressionImpl.h"
#include "common/algUtil.h"
#include "log/logger.h"
#include "src/common/testUtil.h"

using arctern::FaceExpressionImpl;
using arctern::ErrorType;
using arctern::Result;

FaceExpressionImpl::FaceExpressionImpl() : extScale_{0.0, 0.5, 0.0, 0.1} {
  input_h_ = 96;
  input_w_ = 96;
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_GRAY;
}

FaceExpressionImpl::~FaceExpressionImpl() noexcept {
  FaceExpressionImpl::Release();
}

ErrorType FaceExpressionImpl::CheckRunParam(const RunParameter *p) {
  auto rp = dynamic_cast<const FaceExpressionRunParameter *>(p);
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(rp);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) return arcternBaseCheck;
  return ErrorType::ERR_SUCCESS;
}

ErrorType FaceExpressionImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                               std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<FaceExpressionResult *>(result_);

  int total = endId - startId;

  // const auto batchNum = initParam_.batchNum_;
  int stride0 = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    auto result = calcResult(data0, stride0);
    r->faceExpressions_[startId + subId].type = arctern::ExpressionType(result.first);
    r->faceExpressions_[startId + subId].score = result.second;
  }

  return ErrorType::ERR_SUCCESS;
}

/// \brief calculate mask label and corresponding score
std::pair<int, float> FaceExpressionImpl::calcResult(const float *data, size_t num_class) {
  std::vector<float> prob;
  prob.assign(data, data + num_class);

  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}

arctern::ArcternRect FaceExpressionImpl::LocalGetRoi(const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const FaceExpressionRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);
  auto rect = check_shrink_box(oriRect, p->imageV_[idx].width, p->imageV_[idx].height);
  ArcternRect result;
  result.x = rect.x;
  result.y = rect.y;
  result.width = rect.width;
  result.height = rect.height;
  return result;
}

void FaceExpressionImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::faceExp);
  runParm->sequence_.push_back(OperatorType::normalized);
}

void FaceExpressionImpl::InitPreprocessorParam() {
  preProcessor_.reset(new OpencvPreprocessor);
  OpencvPreprocessorInitPt init_pt;
  init_pt.net_h_ = input_h_;
  init_pt.net_w_ = input_w_;
  init_pt.mean_ = 0;
  init_pt.std_ = 255;
  init_pt.border_value = 0;
  auto code = preProcessor_->Init(&init_pt);
  assert(code == ErrorType::ERR_SUCCESS);
}
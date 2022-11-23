/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.11
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include <utility>
#include <vector>
#include "faceGlassImpl.h"
#include "common/file_buffer.h"
#include "common/imageConvert.h"
#include "common/algUtil.h"
#include "log/logger.h"

using arctern::FaceGlassImpl;
using arctern::ErrorType;
using arctern::Result;

FaceGlassImpl::FaceGlassImpl() : extScale_{0.0, 0.2, 0.0, 0.0} {
  input_h_ = 128;
  input_w_ = 128;
}

FaceGlassImpl::~FaceGlassImpl() noexcept {
  FaceGlassImpl::Release();
}



ErrorType FaceGlassImpl::CheckRunParam(const RunParameter *p) {
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(p);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) return arcternBaseCheck;
  return ErrorType::ERR_SUCCESS;
}

ErrorType FaceGlassImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                         std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<FaceGlassResult *>(result_);

  int total = endId - startId;

  // const auto batchNum = initParam_.batchNum_;
  int stride0 = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    auto glasses = calc_mask(data0, stride0);

    r->FaceGlasss_[startId + subId].glassesStatus = GlassesStatus(glasses.first);
    r->FaceGlasss_[startId + subId].statusScore = glasses.second;
  }

  return ErrorType::ERR_SUCCESS;
}

/// \brief calculate mask label and corresponding score
std::pair<int, float> FaceGlassImpl::calc_mask(const float *data, size_t num_class) {
  std::vector<float> prob;
  for (size_t j = 0; j < num_class; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}

arctern::ArcternRect FaceGlassImpl::LocalGetRoi(const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const FaceGlassRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  auto rect = check_shrink_box(oriRect, p->imageV_[idx].width, p->imageV_[idx].height);
  rect = get_face(p->imageV_[idx].width, p->imageV_[idx].height, rect, extScale_);
  rect.width -= 1;
  rect.height -= 1;
  ArcternRect result;
  result.x = rect.x;
  result.y = rect.y;
  result.width = rect.width;
  result.height = rect.height;
  return result;
}
void FaceGlassImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm)  {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
}


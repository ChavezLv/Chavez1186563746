/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.28
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include <vector>
#include <utility>
#include "facePhoneImpl.h"
#include "common/file_buffer.h"
#include "common/imageConvert.h"
#include "common/algUtil.h"
#include "log/logger.h"

using arctern::FacePhoneImpl;
using arctern::ErrorType;
using arctern::Result;

FacePhoneImpl::FacePhoneImpl() : extScale_{0.0, 0.5, 0.0, 0.1} {
  input_h_ = 255;
  input_w_ = 255;
}

FacePhoneImpl::~FacePhoneImpl() noexcept {
  FacePhoneImpl::Release();
}

ErrorType FacePhoneImpl::CheckRunParam(const RunParameter *p) {
  auto rp = dynamic_cast<const FacePhoneRunParameter *>(p);
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(rp);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) return arcternBaseCheck;
  return ErrorType::ERR_SUCCESS;
}

ErrorType FacePhoneImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                          std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<FacePhoneResult *>(result_);

  int total = endId - startId;

  // const auto batchNum = initParam_.batchNum_;
  int stride0 = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    auto phone = calcPhone(data0, stride0);

    r->facePhones_[startId + subId].phoneStatus = PhoneStatus(phone.first);
    r->facePhones_[startId + subId].statusScore = phone.second;
  }

  return ErrorType::ERR_SUCCESS;
}

/// \brief calculate phone label and corresponding score
std::pair<int, float> FacePhoneImpl::calcPhone(const float *data, size_t num_class) {
  std::vector<float> prob;
  for (size_t j = 0; j < num_class; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}

arctern::ArcternRect FacePhoneImpl::LocalGetRoi(const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const FacePhoneRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  // auto rect = check_shrink_box(oriRect, image.cols, image.rows);
  auto rect = get_face(p->imageV_[idx].width, p->imageV_[idx].height, oriRect, extScale_, Float2Int::TRUNCATION);
  ArcternRect result;
  result.x = rect.x;
  result.y = rect.y;
  result.width = rect.width;
  result.height = rect.height;
  return result;
}

void FacePhoneImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::normalized);
}



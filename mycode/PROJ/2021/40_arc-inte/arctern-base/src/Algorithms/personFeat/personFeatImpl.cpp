/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.11.30
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#include <utility>
#include <vector>
#include "personFeatImpl.h"
#include "common/file_buffer.h"
#include "common/imageConvert.h"
#include "common/algUtil.h"
#include "common/featureUtil.h"
#include "log/logger.h"

using arctern::PersonFeatImpl;
using arctern::ErrorType;
using arctern::Result;

PersonFeatImpl::PersonFeatImpl() {
  input_h_ = 384;
  input_w_ = 128;
  isNormaliized_ = true;
}

PersonFeatImpl::~PersonFeatImpl() noexcept {
  PersonFeatImpl::Release();
}

ErrorType PersonFeatImpl::CheckRunParam(const RunParameter *p) {
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(p);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) return arcternBaseCheck;
  return ErrorType::ERR_SUCCESS;
}

ErrorType PersonFeatImpl::LocalPostProcess(const RunParameter *p,
                                         int startId,
                                         int endId,
                                         std::vector<arctern::Tensor<float>> &outputs) {
  assert(p != nullptr);
  assert(startId >= 0);
  auto r = dynamic_cast<PersonFeatResult *>(result_);

  int total = endId - startId;

  int stride = outputs[0].size() / outputs[0].shape()[0];
  int offset = 0;

  for (int subId = 0; subId < total; subId++) {
    float *data = outputs[0].data() + stride * subId;
    std::vector<float> data_cls(stride);
    for (size_t i = 0; i < data_cls.size(); ++i) {
      data_cls[i] = data[i];
    }

    arctern::l2norm(data_cls);

    r->features_[startId + subId] = encryptFeature(data_cls);

    offset += stride;
  }

  return ErrorType::ERR_SUCCESS;
}

std::vector<uint8> PersonFeatImpl::encryptFeature(const std::vector<float> &feat) {
  return encrypt_feature(feat);
}

int PersonFeatImpl::GetConfigInfo(arctern::MxnetModel *model) {
  int input_dim_w, input_dim_h;
  bool return_res = try_get_value(*model, "w", input_dim_w);
  return_res &= try_get_value(*model, "h", input_dim_h);
  if (return_res) {
    input_w_ = input_dim_w;
    input_h_ = input_dim_h;
  }
  return 0;
}

arctern::ArcternRect PersonFeatImpl::LocalGetRoi(const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const PersonFeatRunParameter *>(p);
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
void PersonFeatImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  if(isNormaliized_) {
    runParm->sequence_.push_back(OperatorType::normalized);
  }
}


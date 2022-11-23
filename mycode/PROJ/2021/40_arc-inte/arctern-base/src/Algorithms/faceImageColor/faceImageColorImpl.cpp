/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.28
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include <utility>
#include <vector>
#include "faceImageColorImpl.h"
#include "common/file_buffer.h"
#include "common/imageConvert.h"
#include "common/algUtil.h"

#include "log/logger.h"

using arctern::FaceImageColorImpl;
using arctern::ErrorType;
using arctern::Result;
using arctern::ArcternRect;

FaceImageColorImpl::FaceImageColorImpl() {
  input_h_ = 64;
  input_w_ = 64;
  isNormaliized_ = true;
}

FaceImageColorImpl::~FaceImageColorImpl() noexcept {
  FaceImageColorImpl::Release();
}

//void FaceImageColorImpl::Release() {
//
//  ArcternAlgorithm::Release();
//}
//
//ErrorType FaceImageColorImpl::Init(const InitParameter *p) {
//  auto ret = ArcternAlgorithm::Init(p);
//  if (ret != ErrorType::ERR_SUCCESS) {
//    FaceImageColorImpl::Release();
//    return ret;
//  }
//
//  fg FaceImageColorResult();
//
//  // use uniqur_ptr to manager memory
//  preProcessor_.reset(new AttrPreProcesser(isNormaliized_));
//
//  arctern::AttrPreProInitParameter initParam;
//  initParam.netH_ = input_h_;
//  initParam.netW_ = input_w_;
//  initParam.mean_ = 127.5;
//  initParam.std_ = 128.0;
//  initParam.alignFace_ = AlignFace::NoAlignFace;
//
//  assert(preProcessor_);
//
//  preProcessor_->Init(&initParam);
//
//  return ErrorType::ERR_SUCCESS;
//}
//
//ErrorType FaceImageColorImpl::SetParameter(ConfigParameter *p) {
//  if (nullptr != p) {
//    return ErrorType::ERR_SUCCESS;
//  }
//
//  return ErrorType::ERR_INPUT_TYPE_INCORRECT;
//}
//
//const StatusParameter *FaceImageColorImpl::GetParameter(StatusParameter *) {
//  return nullptr;
//}

ErrorType FaceImageColorImpl::CheckRunParam(const RunParameter *p) {
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(p);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) return arcternBaseCheck;
  return ErrorType::ERR_SUCCESS;
}

namespace {
cv::Rect getFace(const int width, const int height, const cv::Rect &rect) {
  int x1 = rect.tl().x;
  int y1 = rect.tl().y;
  int x2 = rect.br().x - 1;  // python implementation lacks 1 elem
  int y2 = rect.br().y - 1;
  x1 = std::max(0, x1);
  x2 = std::min(width, x2);
  y1 = std::max(0, y1);
  y2 = std::min(height, y2);

  return cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2));

}
}

//cv::Mat FaceImageColorImpl::LocalGetFace(const cv::Mat &image, const RunParameter *p, int idx) {
//  const auto implRunP = dynamic_cast<const FaceImageColorRunParameter *>(p);
//  auto &arctRect = implRunP->rectsV_[idx];
//  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);
//  auto rect = check_shrink_box(oriRect, image.cols, image.rows);
//  rect = getFace(image.cols, image.rows, rect);
//  cv::Mat face = image(rect).clone();
//  return face;
//}

ErrorType FaceImageColorImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                               std::vector<arctern::Tensor<float>> &outputs) {
  auto r = dynamic_cast<FaceImageColorResult *>(result_);
  int total = endId - startId;

  // const auto batchNum = initParam_.batchNum_;
  int stride0 = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    r->faceImageColors_[startId + subId] = data0[1];
  }

  return ErrorType::ERR_SUCCESS;
}


int FaceImageColorImpl::GetConfigInfo(arctern::MxnetModel *model) {
  return 0;
}

ArcternRect FaceImageColorImpl::LocalGetRoi(const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const FaceImageColorRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);
  auto rect = check_shrink_box(oriRect, p->imageV_[idx].width, p->imageV_[idx].height);
  rect = getFace(p->imageV_[idx].width, p->imageV_[idx].height, rect);
  ArcternRect result;
  result.x = rect.x;
  result.y = rect.y;
  result.width = rect.width;
  result.height = rect.height;
  return result;
}

void FaceImageColorImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  if(isNormaliized_) {
    runParm->sequence_.push_back(OperatorType::normalized);
  }
  runParm->sequence_.push_back(OperatorType::resized);
}

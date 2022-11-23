/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.11
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include <utility>
#include <vector>
#include "src/Algorithms/faceBangs/faceBangsImpl.h"
#include "common/algUtil.h"
#include "log/logger.h"
#include "src/common/testUtil.h"
#include "src/common/algUtil.h"

using arctern::FaceBangsImpl;
using arctern::ErrorType;
using arctern::Result;

FaceBangsImpl::FaceBangsImpl() : extScale_{0.0, 0.5, 0.0, 0.1} {
  input_h_ = 128;
  input_w_ = 128;
}

FaceBangsImpl::~FaceBangsImpl() noexcept {
  FaceBangsImpl::Release();
}

//void FaceBangsImpl::Release() {
//
//  ArcternAlgorithm::Release();
//}
//
//ErrorType FaceBangsImpl::Init(const InitParameter *p) {
//  auto ret = ArcternAlgorithm::Init(p);
//  if (ret != ErrorType::ERR_SUCCESS) {
//    FaceBangsImpl::Release();
//    return ret;
//  }
//
//  preProcessor_.reset(new AttrPreProcesser);
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
//  auto code = preProcessor_->Init(&initParam);
//  assert(code == ErrorType::ERR_SUCCESS);
//
//  return ErrorType::ERR_SUCCESS;
//}

ErrorType FaceBangsImpl::CheckRunParam(const RunParameter *p) {
  auto rp = dynamic_cast<const FaceBangsRunParameter *>(p);
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(rp);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) return arcternBaseCheck;
  return ErrorType::ERR_SUCCESS;
}

//cv::Mat FaceBangsImpl::LocalPreProcess(const cv::Mat &image, const RunParameter *p, int idx) {
//  const auto implRunP = dynamic_cast<const FaceBangsRunParameter *>(p);
//  auto &arctRect = implRunP->rectsV_[idx];
//  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);
//
//  auto rect = get_face(image.cols, image.rows, oriRect, extScale_, Float2Int::TRUNCATION);
//
//  PreProRunParameter runParameter;
//  cv::Mat face_image = image(rect);
//  printfImg<uchar>(face_image);
//  runParameter.iMat_ = face_image;
//
//  auto result = preProcessor_->Process(&runParameter);
//  assert(result.error_type_ == ErrorType::ERR_SUCCESS);
//  return result.oMat_;
//}

ErrorType FaceBangsImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                         std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<FaceBangsResult *>(result_);

  int total = endId - startId;

  // const auto batchNum = initParam_.batchNum_;
  int stride0 = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    auto result = calcResult(data0, stride0);
    r->faceBangss_[startId + subId].status = arctern::BangsStatus(result.first);
    r->faceBangss_[startId + subId].statusScore = result.second;
  }

  return ErrorType::ERR_SUCCESS;
}

/// \brief calculate mask label and corresponding score
std::pair<int, float> FaceBangsImpl::calcResult(const float *data, size_t num_class) {
  std::vector<float> prob;
  prob.assign(data, data + num_class);
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}

arctern::ArcternRect FaceBangsImpl::LocalGetRoi(const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const FaceBangsRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  auto rect = arctern::get_face(p->imageV_[idx].width, p->imageV_[idx].height,oriRect, extScale_, Float2Int::TRUNCATION);
  ArcternRect result;
  result.x = rect.x;
  result.y = rect.y;
  result.width = rect.width;
  result.height = rect.height;
  return result;
}

void FaceBangsImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::normalized);
}


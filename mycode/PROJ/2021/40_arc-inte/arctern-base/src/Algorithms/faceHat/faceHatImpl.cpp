/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.11
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include <utility>
#include <vector>
#include "faceHatImpl.h"
#include "common/file_buffer.h"
#include "common/imageConvert.h"
#include "common/algUtil.h"
#include "log/logger.h"

using arctern::FaceHatImpl;
using arctern::ErrorType;
using arctern::Result;

FaceHatImpl::FaceHatImpl() : extScale_{0.0, 1.5, 0.0, 1.5} {
  input_h_ = 128;
  input_w_ = 128;
}

FaceHatImpl::~FaceHatImpl() noexcept {
  FaceHatImpl::Release();
}

//void FaceHatImpl::Release() {
//
//  ArcternAlgorithm::Release();
//}
//
//ErrorType FaceHatImpl::Init(const InitParameter *p) {
//  auto ret = ArcternAlgorithm::Init(p);
//  if (ret != ErrorType::ERR_SUCCESS) {
//    FaceHatImpl::Release();
//    return ret;
//  }
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



ErrorType FaceHatImpl::CheckRunParam(const RunParameter *p) {
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(p);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) return arcternBaseCheck;
  return ErrorType::ERR_SUCCESS;
}


//cv::Mat FaceHatImpl::LocalPreProcess(const cv::Mat &image, const RunParameter *p, int idx) {
//  const auto implRunP = dynamic_cast<const FaceHatRunParameter *>(p);
//  auto &arctRect = implRunP->rectsV_[idx];
//  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);
//
//  auto rect = check_shrink_box(oriRect, image.cols, image.rows);
//  rect = get_face(image.cols, image.rows, rect, extScale_);
//
//  PreProRunParameter runParameter;
//  cv::Mat face_image = image(rect).clone();
//
//  runParameter.iMat_ = face_image;
//
//  auto result = preProcessor_->Process(&runParameter);
//  assert(result.error_type_ == ErrorType::ERR_SUCCESS);
//
//  return result.oMat_;
//}

ErrorType FaceHatImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                        std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<FaceHatResult *>(result_);

  int total = endId - startId;

  // const auto batchNum = initParam_.batchNum_;
  int stride0 = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    auto hat = calcHat(data0, stride0);

    r->FaceHats_[startId + subId].hatStatus = HatStatus(hat.first);
    r->FaceHats_[startId + subId].statusScore = hat.second;
  }

  return ErrorType::ERR_SUCCESS;
}

/// \brief calculate mask label and corresponding score
std::pair<int, float> FaceHatImpl::calcHat(const float *data, size_t num_class) {
  std::vector<float> prob;
  for (size_t j = 0; j < num_class; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}

arctern::ArcternRect FaceHatImpl::LocalGetRoi(const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const FaceHatRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  auto rect = check_shrink_box(oriRect, p->imageV_[idx].width, p->imageV_[idx].height);
  rect = get_face( p->imageV_[idx].width, p->imageV_[idx].height, rect, extScale_);
  ArcternRect result;
  result.x = rect.x;
  result.y = rect.y;
  result.width = rect.width;
  result.height = rect.height;
  return result;

}
void FaceHatImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm)  {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::normalized);
}


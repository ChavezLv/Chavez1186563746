/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.11.24
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#include <vector>
#include <utility>
#include "src/Algorithms/faceBasicMultiAttr/faceBasicMultiAttrImpl.h"
#include "common/algUtil.h"
#include "log/logger.h"
#include "src/common/testUtil.h"
using arctern::FaceBasicMultiAttrImpl;
using arctern::ErrorType;
using arctern::Result;

FaceBasicMultiAttrImpl::FaceBasicMultiAttrImpl() : extScale_{0.0, 0.7, 0.0, 0.5} {
  input_h_ = 112;
  input_w_ = 128;
}

FaceBasicMultiAttrImpl::~FaceBasicMultiAttrImpl() noexcept {
  FaceBasicMultiAttrImpl::Release();
}

//void FaceBasicMultiAttrImpl::Release() {
//
//  ArcternAlgorithm::Release();
//}
//
//ErrorType FaceBasicMultiAttrImpl::Init(const InitParameter *p) {
//  auto ret = ArcternAlgorithm::Init(p);
//  if (ret != ErrorType::ERR_SUCCESS) {
//    FaceBasicMultiAttrImpl::Release();
//    return ret;
//  }
//
//  return ErrorType::ERR_SUCCESS;
//}

ErrorType FaceBasicMultiAttrImpl::CheckRunParam(const RunParameter *p) {
  auto rp = dynamic_cast<const FaceBasicMultiAttrRunParameter *>(p);
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(rp);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) return arcternBaseCheck;
  return ErrorType::ERR_SUCCESS;
}

ErrorType FaceBasicMultiAttrImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                                   std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<FaceBasicMultiAttrResult *>(result_);

  int total = endId - startId;

  // const auto batchNum = initParam_.batchNum_;
  int stride0 = outputs[0].size() / outputs[0].shape()[0];
  int stride1 = outputs[1].size() / outputs[1].shape()[0];
  int stride2 = outputs[2].size() / outputs[2].shape()[0];
  int stride3 = outputs[3].size() / outputs[3].shape()[0];
  int stride4 = outputs[4].size() / outputs[4].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    auto result = calcResult(data0, stride0);
    r->faceBasicMultiAttrs_[startId + subId].gender = arctern::BasicGender(result.first);
    r->faceBasicMultiAttrs_[startId + subId].genderScore = result.second;

    const float *data1 = outputs[1].data() + subId * stride1;
    auto result1 = calc_age(data1, stride1);
    r->faceBasicMultiAttrs_[startId + subId].age = result1.first;
    r->faceBasicMultiAttrs_[startId + subId].ageScore = result1.second;

    const float *data2 = outputs[2].data() + subId * stride2;
    auto result2 = calcResult(data2, stride2);
    r->faceBasicMultiAttrs_[startId + subId].glasses = arctern::BasicGlasses(result2.first);
    r->faceBasicMultiAttrs_[startId + subId].glassesScore = result2.second;

    const float *data3 = outputs[3].data() + subId * stride3;
    auto result3 = calcResult(data3, stride3);
    r->faceBasicMultiAttrs_[startId + subId].mask = arctern::BasicMask(result3.first);
    r->faceBasicMultiAttrs_[startId + subId].maskScore = result3.second;

    const float *data4 = outputs[4].data() + subId * stride4;
    auto result4 = calcResult(data4, stride4);
    r->faceBasicMultiAttrs_[startId + subId].headwear = arctern::BasicHeadwear(result4.first);
    r->faceBasicMultiAttrs_[startId + subId].headwearScore = result4.second;

  }

  return ErrorType::ERR_SUCCESS;
}

namespace {
cv::Rect get_face1(int width, int height,
                  const cv::Rect &bbox) {
  float x1 = bbox.tl().x;
  float y1 = bbox.tl().y;
  float x2 = bbox.br().x;
  float y2 = bbox.br().y;
  float w = x2 - x1 + 1;
  float h = y2 - y1 + 1;

  y1 = y1 - h * 0.7;
  y2 = y2 + h * 0.1;
  float by1 = std::min(std::max(0, static_cast<int>(y1)), height);
  float by2 = std::min(std::max(0, static_cast<int>(y2)), height);
  float hh = by2 - by1 + 1;
  float ww = hh;
  if (ww > w) {
    int wx = static_cast<int>((ww - w) / 2);
    x1 = x1 - wx;
    x2 = x2 + wx;
  }

  float bx1 = std::min(std::max(0, static_cast<int>(x1)), width);
  float bx2 = std::min(std::max(0, static_cast<int>(x2)), width);

  return cv::Rect(cv::Point(bx1, by1), cv::Point(bx2, by2));
}
}


/// \brief calculate mask label and corresponding score
std::pair<int, float> FaceBasicMultiAttrImpl::calcResult(const float *data, size_t num_class) {
  std::vector<float> prob;
  prob.assign(data, data + num_class);
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}

/// \brief calculate age and corresponding score
std::pair<int, float> FaceBasicMultiAttrImpl::calc_age(const float *data, size_t num_class) {
  float age = 0, score = 0;
  for (size_t i = 0; i < num_class; ++i) {
    age += data[i] * i;
    if (score < data[i]) score = data[i];
  }
  //return std::make_pair(python_round(age), score);
  return std::make_pair(python_round(age), score);
}

//cv::Mat FaceBasicMultiAttrImpl::LocalGetFace(const cv::Mat &image, const arctern::RunParameter *p, int idx) {
//  const auto implRunP = dynamic_cast<const FaceBasicMultiAttrRunParameter *>(p);
//  auto &arctRect = implRunP->rectsV_[idx];
//  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);
//
//  auto rect = get_face1(image.cols, image.rows, oriRect);
//  cv::Mat face_image = image(rect).clone();
//  printfImg<uchar>(face_image);
//  return face_image;
//}

int FaceBasicMultiAttrImpl::GetConfigInfo(arctern::MxnetModel *model) {
  bool return_res = try_get_value(*model, "INPUT_HEIGHT", input_h_);
  return_res &= try_get_value(*model, "INPUT_WIDTH", input_w_);
  return_res &= try_get_value(*model, "EXT_SCALE0", extScale_[0]);
  return_res &= try_get_value(*model, "EXT_SCALE1", extScale_[1]);
  return_res &= try_get_value(*model, "EXT_SCALE2", extScale_[2]);
  return_res &= try_get_value(*model, "EXT_SCALE3", extScale_[3]);
  return_res &= try_get_value(*model, "TASK_NUM", task_num_);
  return_res &= try_get_value(*model, "AGE_IDX", age_idx_);
  return_res &= try_get_value(*model, "AGE_RANGE", age_range_);
  assert(return_res == true);
 // initPreProcessor();
  return 0;
}

//void FaceBasicMultiAttrImpl::initPreProcessor() {
//  preProcessor_.reset(new AttrPreProcesser);
//  arctern::AttrPreProInitParameter initParam;
//  initParam.netH_ = input_h_;
//  initParam.netW_ = input_w_;
//  initParam.mean_ = 127.5;
//  initParam.std_ = 128.0;
//  initParam.alignFace_ = AlignFace::NoAlignFace;
//  assert(preProcessor_);
//  auto code = preProcessor_->Init(&initParam);
//  assert(code == ErrorType::ERR_SUCCESS);
//}

arctern::ArcternRect FaceBasicMultiAttrImpl::LocalGetRoi(const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const FaceBasicMultiAttrRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  auto rect = get_face1(p->imageV_[idx].width, p->imageV_[idx].height, oriRect);
  ArcternRect result;
  result.x = rect.x;
  result.y = rect.y;
  result.width = rect.width;
  result.height = rect.height;
  return result;
}

void FaceBasicMultiAttrImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::normalized);
}
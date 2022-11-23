/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.31
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include <utility>
#include <algorithm>
#include <vector>
#include <string>
#include "ageGenderImpl.h"
#include "common/algUtil.h"
#include "utils/AgeGenderUtil.h"

using arctern::AgeGenderImpl;
using arctern::ErrorType;
using arctern::Result;

AgeGenderImpl::AgeGenderImpl() {
  input_h_ = 128;
  input_w_ = 112;
}

AgeGenderImpl::~AgeGenderImpl() noexcept {
  AgeGenderImpl::Release();
}

arctern::ArcternRect AgeGenderImpl::LocalGetRoi(const RunParameter *p, int idx) {
  const int width = p->imageV_[idx].width;
  const int height = p->imageV_[idx].height;
  auto &arctRect = p->rectsV_[idx];
  cv::Rect bbox(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  float x1 = bbox.tl().x;
  float y1 = bbox.tl().y;
  float x2 = bbox.br().x;
  float y2 = bbox.br().y;
  float w = x2 - x1 + 1;
  float h = y2 - y1 + 1;

  y1 = y1 - h * extScale_[1];
  y2 = y2 + h * extScale_[3];
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

  ArcternRect result;
  result.x = bx1;
  result.y = by1;
  result.width = bx2 - bx1;
  result.height = by2 - by1;
  return result;

}


ErrorType AgeGenderImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                          std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<AgeGenderResult *>(result_);

  int total = endId - startId;

  // const auto batchNum = initParam_.batchNum_;
  int stride0 = outputs[0].size() / outputs[0].shape()[0];
  int stride1 = outputs[1].size() / outputs[1].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    const float *data1 = outputs[1].data() + subId * stride1;

    auto gender = calc_gender(data0, stride0);
    auto age = calc_age(data1, stride1);

    r->ageGenders_[startId + subId].age = age.first;
    r->ageGenders_[startId + subId].ageScore = age.second;
    r->ageGenders_[startId + subId].gender = gender.first == 0 ? MALE : FEMALE;
    r->ageGenders_[startId + subId].genderScore = gender.second;
  }

  return ErrorType::ERR_SUCCESS;
}

ErrorType AgeGenderImpl::CheckRunParam(const RunParameter *p) {
  auto rp = dynamic_cast<const AgeGenderRunParameter *>(p);
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(rp);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) return arcternBaseCheck;
  return ErrorType::ERR_SUCCESS;
}



int AgeGenderImpl::GetConfigInfo(arctern::MxnetModel *model) {
  extScale_[0] = 0.0;
  extScale_[2] = 0.0;

  if(model->version()->str() == "0.1.0") {
    extScale_[1] = 0.3;
    extScale_[3] = 0.3;
  } else {
    extScale_[1] = 0.5;
    extScale_[3] = 0.1;
  }
  return 0;
}


void AgeGenderImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm)  {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
}

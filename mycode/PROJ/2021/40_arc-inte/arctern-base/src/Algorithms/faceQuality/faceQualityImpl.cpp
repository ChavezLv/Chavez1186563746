/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.17
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include <vector>
#include "faceQualityImpl.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "common/file_buffer.h"
#include "common/algUtil.h"
#include "log/logger.h"

using arctern::FaceQualityImpl;
using arctern::ErrorType;
using arctern::Result;

FaceQualityImpl::FaceQualityImpl() {
  input_h_ = 50;
  input_w_ = 50;
  isNormaliized_ = true;
}

FaceQualityImpl::~FaceQualityImpl()  {
  FaceQualityImpl::Release();
}

ErrorType FaceQualityImpl::CheckRunParam(const RunParameter *p) {
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(p);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) return arcternBaseCheck;
  return ErrorType::ERR_SUCCESS;
}

ErrorType FaceQualityImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                            std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<FaceQualityResult *>(result_);
  int total = endId - startId;
  int stride0 = outputs[0].size() / outputs[0].shape()[0];

  bool secondOutput = outputs.size() > 1;
  int stride1 = secondOutput ? outputs[1].size() / outputs[1].shape()[0] : 0;

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    const float *data1 = secondOutput ? outputs[1].data() + subId * stride1 : nullptr;
    r->faceQuality_[startId + subId] = calcResult(data0, data1);
  }

  return ErrorType::ERR_SUCCESS;
}

float FaceQualityImpl::calcResult(const float *data0, const float *data1) {
  if (outputNum_ > 1) {
    return data1[0] > data1[1] ? 0 : data0[0];
  } else {
    return data0[0];
  }
}

int FaceQualityImpl::GetConfigInfo(arctern::MxnetModel *model) {
  bool res = try_get_value(*model, "GRAY_IMG_INPUT", isGrayInput_);
  res &= try_get_value(*model, "BRANCH_NUM", outputNum_);
  res &= try_get_value(*model, "CLS_FIRST_BR", CLS_FIRST_BR_);
 // assert(res);

  /// netFormat_ = isGrayInput_ ? NetworkFormat::GRAY : NetworkFormat::C_RGB;
  dstImageFormatForForward_ = isGrayInput_ ? ARCTERN_IMAGE_FORMAT_GRAY : ARCTERN_IMAGE_FORMAT_RGB888;
//  initPreProcessor();
  return 0;
}

arctern::ArcternRect FaceQualityImpl::LocalGetRoi(const RunParameter *p, int idx)  {
  const auto implRunP = dynamic_cast<const FaceQualityRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  auto rect = check_shrink_box(oriRect,p->imageV_[idx].width, p->imageV_[idx].height);
  ArcternRect result;
  result.x = rect.x;
  result.y = rect.y;
  result.width = rect.width;
  result.height = rect.height;
  return result;
}
void FaceQualityImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  if(isNormaliized_) {
    runParm->sequence_.push_back(OperatorType::normalized);
  }
}



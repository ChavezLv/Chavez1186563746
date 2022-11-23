/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.18
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include <utility>
#include <algorithm>
#include <vector>
#include "faceOcclusionImpl.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "common/file_buffer.h"
#include "common/algUtil.h"
#include "log/logger.h"

using arctern::FaceOcclusionImpl;
using arctern::ErrorType;
using arctern::Result;

FaceOcclusionImpl::FaceOcclusionImpl() {
  ///  netFormat_ = NetworkFormat::C_BGR;
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_BGR888;
}

FaceOcclusionImpl::~FaceOcclusionImpl() noexcept {
  FaceOcclusionImpl::Release();
}

ErrorType FaceOcclusionImpl::CheckRunParam(const RunParameter *p) {
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(p);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) return arcternBaseCheck;
  return ErrorType::ERR_SUCCESS;
}

ErrorType FaceOcclusionImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                              std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<FaceOcclusionResult *>(result_);
  int total = endId - startId;
  const int numTask = outputs.size();
  spdlog::info("numTask = {}", numTask);
  assert(9 == numTask);

  std::array<int, 9> stride;
  for (int i = 0; i < 9; i++) {
    stride[i] = outputs[i].size() / outputs[i].shape()[0];
  }

  for (int subId = 0; subId < total; subId++) {
    float *data = outputs[0].data() + subId * stride[0];
    r->faceOcclusion_[startId + subId].leftEyeOpen = calcResult(data, stride[0]);

    data = outputs[1].data() + subId * stride[1];
    r->faceOcclusion_[startId + subId].rightEyeOpen = calcResult(data, stride[1]);

    data = outputs[2].data() + subId * stride[2];
    r->faceOcclusion_[startId + subId].leftEyeOcc = calcResult(data, stride[2]);

    data = outputs[3].data() + subId * stride[3];
    r->faceOcclusion_[startId + subId].rightEyeOcc = calcResult(data, stride[3]);

    data = outputs[4].data() + subId * stride[4];
    r->faceOcclusion_[startId + subId].leftCheekOcc = calcResult(data, stride[4]);

    data = outputs[5].data() + subId * stride[5];
    r->faceOcclusion_[startId + subId].rightCheekOcc = calcResult(data, stride[5]);

    data = outputs[6].data() + subId * stride[6];
    r->faceOcclusion_[startId + subId].mouthOcc = calcResult(data, stride[6]);

    data = outputs[7].data() + subId * stride[7];
    r->faceOcclusion_[startId + subId].noseOcc = calcResult(data, stride[7]);

    data = outputs[8].data() + subId * stride[8];
    r->faceOcclusion_[startId + subId].chincontourOcc = calcResult(data, stride[8]);
  }

  return ErrorType::ERR_SUCCESS;
}

std::pair<int, float> FaceOcclusionImpl::calcResult(const float *data, size_t num_class) {
  std::vector<float> prob;
  for (size_t j = 0; j < num_class; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}

int FaceOcclusionImpl::GetConfigInfo(arctern::MxnetModel *model) {
  int input_dim = 0;
  bool res = try_get_value(*model, "INPUT_DIM", input_dim);
  assert(res);

  input_h_ = input_dim;
  input_w_ = input_dim;

  extScale_ = {0.0, 0.5, 0.0, 0.1};
 // initPreProcessor();
  return 0;
}

arctern::ArcternRect FaceOcclusionImpl::LocalGetRoi(const RunParameter *p, int idx) {
    const auto implRunP = dynamic_cast<const FaceOcclusionRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  auto rect = check_shrink_box(oriRect, p->imageV_[idx].width, p->imageV_[idx].height);
  rect = get_face(p->imageV_[idx].width, p->imageV_[idx].height, rect, extScale_);
  ArcternRect ret;
  ret.x = rect.x;
  ret.y = rect.y;
  ret.width = rect.width;
  ret.height = rect.height;
  return ret;
//
}
void FaceOcclusionImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::normalized);
}



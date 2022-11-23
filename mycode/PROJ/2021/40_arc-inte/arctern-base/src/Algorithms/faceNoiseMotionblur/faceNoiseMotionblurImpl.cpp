/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.15
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include <utility>
#include <vector>
#include <algorithm>
#include "faceNoiseMotionblurImpl.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "common/file_buffer.h"
#include "common/imageConvert.h"
#include "common/algUtil.h"
#include "log/logger.h"

using arctern::FaceNoiseMotionblurImpl;
using arctern::ErrorType;
using arctern::Result;

FaceNoiseMotionblurImpl::FaceNoiseMotionblurImpl() {
  isNormaliized_ = true;
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_BGR888;
}

FaceNoiseMotionblurImpl::~FaceNoiseMotionblurImpl() noexcept {
  FaceNoiseMotionblurImpl::Release();
}

ErrorType FaceNoiseMotionblurImpl::CheckRunParam(const RunParameter *p) {
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(p);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) return arcternBaseCheck;
  return ErrorType::ERR_SUCCESS;
}

namespace {
cv::Rect getFace(
    int img_width, int img_height,  // NOTICE: param :image&scale isn't be used ,need clean!
    const cv::Rect &face) {
  float x1 = face.x;
  float y1 = face.y;
  int width = face.width;
  int height = face.height;

  const float old_size = (width + height) / 2.0f;
  const float center_x = x1 + width / 2.0f;
  const float center_y = y1 + height / 2.0f;

  int size = (width < 64 || height < 64) ? static_cast<int>(old_size * 1.5f) : static_cast<int>(old_size * 0.9f);

  float halfsize = size / 2.0f;
  int sx = arctern::python_round(center_x - 0.9f * halfsize);
  int sy = arctern::python_round(center_y - halfsize);
  int ex = arctern::python_round(center_x + 0.9f * halfsize);
  int ey = arctern::python_round(center_y + halfsize);

  sx = std::max(0, sx);
  sy = std::max(0, sy);
  ex = std::min(ex, img_width - 1);
  ey = std::min(ey, img_height - 1);

  return cv::Rect(cv::Point(sx, sy), cv::Point(ex, ey));
}
}  // namespace

ErrorType FaceNoiseMotionblurImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                                    std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<FaceNoiseMotionblurResult *>(result_);
  int total = endId - startId;
  int stride0 = outputs[USE_POS_].size() / outputs[USE_POS_].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[USE_POS_].data() + subId * stride0;
    auto ALR = calcALResult(data0, stride0);

    r->faceNoiseMotionblurs_[startId + subId].type = NoiseType(ALR.first);
    r->faceNoiseMotionblurs_[startId + subId].score = ALR.second;
  }

  return ErrorType::ERR_SUCCESS;
}

/// \brief calculate mask label and corresponding score
std::pair<int, float> FaceNoiseMotionblurImpl::calcALResult(const float *data, size_t num_class) {
  auto biggest = std::max_element(data, data + num_class);
  auto prob = *biggest;
  int classId = std::distance(data, biggest);
  return std::make_pair(classId, prob);
}

int FaceNoiseMotionblurImpl::GetConfigInfo(arctern::MxnetModel *model) {
  int input_dim = 0;
  bool return_res = try_get_value(*model, "INPUT_DIM", input_dim);
  try_get_value(*model, "USEFUL_POS", USE_POS_);
  assert(return_res == true);
  input_w_ = input_dim;
  input_h_ = input_dim;
  return return_res;
}

arctern::ArcternRect FaceNoiseMotionblurImpl::LocalGetRoi(const RunParameter *p, int idx)  {
  const auto implRunP = dynamic_cast<const FaceNoiseMotionblurRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  auto rect = getFace(p->imageV_[idx].width, p->imageV_[idx].height, oriRect);
  ArcternRect result;
  result.x = rect.x;
  result.y = rect.y;
  result.width = rect.width;
  result.height = rect.height;
  return result;
}
void FaceNoiseMotionblurImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm)  {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::twiceResized);
  if(isNormaliized_) {
    runParm->sequence_.push_back(OperatorType::normalized);
  }

}


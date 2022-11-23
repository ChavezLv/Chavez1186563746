/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.14
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include <src/common/arcterndefs_config_analyzer.h>
#include <utility>
#include <vector>
#include "faceAbnomalLightImpl.h"
#include "common/file_buffer.h"
#include "common/algUtil.h"


using arctern::FaceAbnomalLightImpl;
using arctern::ErrorType;
using arctern::Result;

FaceAbnomalLightImpl::FaceAbnomalLightImpl() {
  /// netFormat_ = NetworkFormat::C_BGR;
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_BGR888;
}

FaceAbnomalLightImpl::~FaceAbnomalLightImpl() noexcept {
  FaceAbnomalLightImpl::Release();
}

ErrorType FaceAbnomalLightImpl::CheckRunParam(const RunParameter *p) {
  const auto rp = dynamic_cast<const FaceAbnomalLightRunParameter *>(p);
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(rp);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) return arcternBaseCheck;
  return ErrorType::ERR_SUCCESS;
}

ErrorType FaceAbnomalLightImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                                 std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<FaceAbnomalLightResult *>(result_);
  int total = endId - startId;
  int stride0 = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    auto ALR = calcALResult(data0, stride0);

    r->faceAbnomalLights_[startId + subId].type = AbnomalLightType(ALR.first);
    r->faceAbnomalLights_[startId + subId].score = ALR.second;
  }

  return ErrorType::ERR_SUCCESS;
}

/// \brief calculate mask label and corresponding score
std::pair<int, float> FaceAbnomalLightImpl::calcALResult(const float *data, size_t num_class) {
  auto biggest = std::max_element(data, data + num_class);
  auto prob = *biggest;
  int classId = std::distance(data, biggest);
  return std::make_pair(classId, prob);
}

int FaceAbnomalLightImpl::GetConfigInfo(arctern::MxnetModel *model) {
  int input_dim = 0;
  std::string key = "INPUT_DIM";
  bool return_res = try_get_value<int>(*model, key, input_dim);
  assert(return_res == true);
  input_w_ = input_dim;
  input_h_ = input_dim;
  return 0;
}

arctern::ArcternRect FaceAbnomalLightImpl::LocalGetRoi(const RunParameter *p, int idx)  {
  const auto implRunP = dynamic_cast<const FaceAbnomalLightRunParameter *>(p);
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
void FaceAbnomalLightImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm)  {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::normalized);

}


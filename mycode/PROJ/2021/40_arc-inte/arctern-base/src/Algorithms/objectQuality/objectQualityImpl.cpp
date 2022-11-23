/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
*  author:         chenbin
 *  Last modified:  2020.12.16
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#include <vector>
#include "objectQualityImpl.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "common/file_buffer.h"
#include "common/algUtil.h"
#include "log/logger.h"

using arctern::ObjectQualityImpl;
using arctern::ErrorType;
using arctern::Result;
using arctern::ArcternRect;

ObjectQualityImpl::ObjectQualityImpl() {
  input_h_ = 50;
  input_w_ = 50;
}

ObjectQualityImpl::~ObjectQualityImpl() noexcept {
  ObjectQualityImpl::Release();
}

ErrorType ObjectQualityImpl::CheckRunParam(const RunParameter *p) {
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(p);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) return arcternBaseCheck;
  return ErrorType::ERR_SUCCESS;
}

ErrorType ObjectQualityImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                            std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<ObjectQualityResult *>(result_);
  int total = endId - startId;
  const float* data_res_arr = outputs[0].data();
  const float* data_cls_arr = outputs[1].data();
  int stride0 = outputs[0].shape().size() / outputs[0].shape()[0];
  int start_res = 0;
  int start_cls = 0;

  for (int subId = 0; subId < total; subId++) {
    float data_res = data_res_arr[start_res];
    start_res += stride0;
    if (branchNum_ == 1) {
      r->objectQuality_[startId + subId] = data_res;
    } else {
      float cls_none = data_cls_arr[start_cls++];
      float cls_obj  = data_cls_arr[start_cls++];
      r->objectQuality_[startId + subId] = (cls_none > cls_obj) ? 0 : data_res;
    }
  }
  return ErrorType::ERR_SUCCESS;
}

int ObjectQualityImpl::GetConfigInfo(arctern::MxnetModel *model) {
  bool res = try_get_value(*model, "INPUT_W", input_w_);
  res &= try_get_value(*model, "INPUT_H", input_h_);
  res &= try_get_value(*model, "BRANCH_NUM", branchNum_);
  //assert(res);
  //initPreProcessor();
  return 0;
}

ArcternRect ObjectQualityImpl::LocalGetRoi(const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const ObjectQualityRunParameter *>(p);
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
void ObjectQualityImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);

}
// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  create:         2021.06.05
// *  email:          kuang.hongshen@intellif.com
// *  modified:
// ****************************************************************************

#include "TrtObjectQualityImpl.h"
#include "common/algUtil.h"
#include "src/common/common.h"
#include <vector>
#include "src/common/trt/TrtAlgorithmUtil.h"
namespace arctern {

TrtObjectQualityImpl::TrtObjectQualityImpl() : m_iBranchNum(1) {
  m_modelsInputHeight = 50;
  m_modelsInputWidth = 50;
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
}

TrtObjectQualityImpl::~TrtObjectQualityImpl() {}

ErrorType
TrtObjectQualityImpl::GetModelsConfigInfo(const MxnetModels *models) {
  auto model = models->models()->Get(0);

  bool b_ret = try_get_value(*model, "INPUT_W", m_modelsInputWidth);

  b_ret &= try_get_value(*model, "INPUT_H", m_modelsInputHeight);

  b_ret &= try_get_value(*model, "BRANCH_NUM", m_iBranchNum);

  return ErrorType::ERR_SUCCESS;
}
void TrtObjectQualityImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}


ArcternRect TrtObjectQualityImpl::LocalGetROI(const RunParameter *p, int idx) {
  return CheckShrinkBox(p->rectsV_[idx], p->imageV_[idx].width, p->imageV_[idx].height);
}


ErrorType TrtObjectQualityImpl::LocalPostProcess(
    const RunParameter *p, int startId, int endId,
    std::vector<arctern::OutputTensor<float>> &outputs) {
  auto r = dynamic_cast<ObjectQualityResult *>(m_pResult);
  int total = endId - startId;
  const float *data_res_arr = outputs[0].data();
  const float *data_cls_arr = outputs[1].data();
  int stride0 = outputs[0].shape().size() / outputs[0].shape()[0];
  int start_res = 0;
  int start_cls = 0;

  for (int subId = 0; subId < total; subId++) {
    float data_res = data_res_arr[start_res];
    start_res += stride0;
    if (m_iBranchNum == 1) {
      r->objectQuality_[startId + subId] = data_res;
    } else {
      float cls_none = data_cls_arr[start_cls++];
      float cls_obj = data_cls_arr[start_cls++];
      r->objectQuality_[startId + subId] = (cls_none > cls_obj) ? 0 : data_res;
    }
  }
  return ErrorType::ERR_SUCCESS;
}

} // namespace arctern

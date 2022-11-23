// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.02
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#include "vehicleCycleFeatImpl.h"
#include "common/file_buffer.h"
#include "log/logger.h"
#include "src/Processors/Algorithms/attrpreprocess.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "vehicleCycleFeat.h"
#include "utils/utils.h"

using namespace arctern;

vehicleCycleFeatImpl::vehicleCycleFeatImpl() {
  input_h_ = 192;
  input_w_ = 192;
}

vehicleCycleFeatImpl::~vehicleCycleFeatImpl() {
  vehicleCycleFeatImpl::Release();
}

ErrorType vehicleCycleFeatImpl::Init(const InitParameter *p) {
  auto ret = ArcternAlgorithm::Init(p);
  if (ret != ErrorType::ERR_SUCCESS) {
    vehicleCycleFeatImpl::Release();
    return ret;
  }

  fg vehicleCycleFeatResult;

  preProcessor_.reset(new AttrPreProcesser);

  arctern::AttrPreProInitParameter initParam;
  initParam.netH_ = input_h_;
  initParam.netW_ = input_w_;
  initParam.mean_ = 127.5f;
  initParam.std_ = 128.f;
  initParam.alignFace_ = AlignFace::NoAlignFace;

  assert(preProcessor_);

  auto code = preProcessor_->Init(&initParam);
  assert(code == ErrorType::ERR_SUCCESS);

  return ErrorType::ERR_SUCCESS;
}

void vehicleCycleFeatImpl::Release() {}

ErrorType vehicleCycleFeatImpl::SetParameter(ConfigParameter *p) {
  if (nullptr != p) {
    return ErrorType::ERR_SUCCESS;
  }

  return ErrorType::ERR_INPUT_TYPE_INCORRECT;
}

const StatusParameter *vehicleCycleFeatImpl::GetParameter(StatusParameter *) {
  return nullptr;
}

cv::Mat vehicleCycleFeatImpl::LocalPreProcess(const cv::Mat &image,
                                              const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const vehicleCycleFeatRunParameter *>(p);

  auto &arct_rect = implRunP->rectsV_[idx];
  cv::Rect ori_rect(arct_rect.x, arct_rect.y, arct_rect.width,
                    arct_rect.height);
  cv::Rect bbox_rect = check_shrink_box(ori_rect, image.cols, image.rows);

  PreProRunParameter runParameter;
  runParameter.iMat_ = image(bbox_rect).clone();

  auto result = preProcessor_->Process(&runParameter);
  assert(result.error_type_ == ErrorType::ERR_SUCCESS);

  return result.oMat_;
}

ErrorType vehicleCycleFeatImpl::LocalPostProcess(
    const RunParameter *p, int startId, int endId,
    std::vector<arctern::Tensor<float>> &outputs) {
  auto res = dynamic_cast<vehicleCycleFeatResult *>(result_);
  int total = endId - startId;
  int stride = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data = outputs[0].data() + subId * stride;
    std::vector<float> data_cls(stride);
    memcpy(data_cls.data(), data, stride * sizeof(float));

    l2norm(data_cls);

    res->m_vFeatResults[startId + subId] = encrypt_feature(data_cls);
  }

  return ErrorType::ERR_SUCCESS;
}

ErrorType vehicleCycleFeatImpl::CheckRunParam(const RunParameter *p) {
  vehicleCycleFeatRunParameter *rp =
      dynamic_cast<vehicleCycleFeatRunParameter *>(p);
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(rp);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) {
    return arcternBaseCheck;
  }

  return ErrorType::ERR_SUCCESS;
}
int vehicleCycleFeatImpl::GetConfigInfo(arctern::MxnetModel *model) {
  bool b_ret = false;
  auto &mxnet_modle = *model;
  b_ret = try_get_value(mxnet_modle, "h", input_h_);
  b_ret &= try_get_value(mxnet_modle, "w", input_w_);

  if (!b_ret)
  {
    input_h_ = 192;
    input_w_ = 192;
  }

  return 0;
}

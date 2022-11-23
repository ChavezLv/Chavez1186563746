// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.01
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#include "policeSuitImpl.h"
#include "common/algUtil.h"
#include "common/file_buffer.h"
#include "common/imageConvert.h"
#include "log/logger.h"
#include "policeSuit.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#include "src/Processors/Algorithms/attrpreprocess.h"
#include <iostream>
#include <string.h>

using namespace arctern;

PoliceSuitImpl::PoliceSuitImpl()
    : m_iImageW(100), m_iImageH(50), m_ConfigParam{} {}

PoliceSuitImpl::~PoliceSuitImpl() { PoliceSuitImpl::Release(); }

ErrorType PoliceSuitImpl::Init(const InitParameter *p) {
  auto ret = ArcternAlgorithm::Init(p);
  if (ret != ErrorType::ERR_SUCCESS) {
    PoliceSuitImpl::Release();
    return ret;
  }

  fg PoliceSuitResult;

  preProcessor_.reset(new AttrPreProcesser);

  arctern::AttrPreProInitParameter init_param;
  init_param.netW_ = m_iImageW;
  init_param.netH_ = m_iImageH;
  init_param.mean_ = 127.5f;
  init_param.std_ = 128.0f;
  init_param.alignFace_ = AlignFace::NoAlignFace;

  assert(preProcessor_);

  auto code = preProcessor_->Init(&init_param);
  assert(code == ErrorType::ERR_SUCCESS);

  return ErrorType::ERR_SUCCESS;
}

void PoliceSuitImpl::Release() {

  ArcternAlgorithm::Release();
}

ErrorType PoliceSuitImpl::SetParameter(ConfigParameter *p) {
  if (nullptr != p) {
    return ErrorType::ERR_SUCCESS;
  }

  return ErrorType::ERR_INPUT_TYPE_INCORRECT;
}

const StatusParameter *PoliceSuitImpl::GetParameter(StatusParameter *p) {
  return nullptr;
}

cv::Mat PoliceSuitImpl::LocalPreProcess(const cv::Mat &image,
                                        const RunParameter *p, int idx) {
  if (nullptr == p || image.empty()) {
    return cv::Mat();
  }
  const auto p_run_param = dynamic_cast<const PoliceSuitRunParameter *>(p);
  auto &arct_rect = p_run_param->rectsV_[idx];
  cv::Rect ori_rect(arct_rect.x, arct_rect.y, arct_rect.width,
                    arct_rect.height);
  cv::Rect chest_bbox = get_chest_roi(ori_rect, image.cols, image.rows);

  if ((chest_bbox.width < 20) || (chest_bbox.height < 20)) {
    return cv::Mat();
  }

  PreProRunParameter pre_run_param;
  pre_run_param.iMat_ = image(chest_bbox).clone();

  auto result = preProcessor_->Process(&pre_run_param);

  if (ErrorType::ERR_SUCCESS != result.error_type_) {
    assert(result.error_type_ == ErrorType::ERR_SUCCESS);
    return cv::Mat();
  }

  return result.oMat_;
}

ErrorType
PoliceSuitImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                 std::vector<arctern::Tensor<float>> &outputs) {
  PoliceSuitResult *p_res = dynamic_cast<PoliceSuitResult *>(result_);

  int i_total = endId - startId;
  int i_stride = outputs[0].size() / outputs[0].shape()[0];
  for (int subId = 0; subId < i_total; subId++) {
    const float *p_data = outputs[0].data() + subId * i_stride;
    auto suit_res = calcSuit(p_data, i_stride);
    p_res->m_vSuitResults[startId + subId].first = suit_res.first;
    p_res->m_vSuitResults[startId + subId].second = suit_res.second;
  }

  return ErrorType::ERR_SUCCESS;
}

cv::Rect PoliceSuitImpl::get_chest_roi(cv::Rect roi, int img_w, int img_h) {
  if (img_w <= 0 || img_h <= 0) {
    return roi;
  }

  cv::Rect bbox = check_shrink_box(roi, img_w, img_h);
  int crop_xmin = std::max(0, bbox.x - bbox.width);
  int crop_ymin = std::max(0, bbox.y + bbox.height);
  int crop_xmax = std::min(img_w, bbox.x + bbox.width * 2);
  int crop_ymax = std::min(img_h, bbox.y + bbox.height * 2);
  int crop_width = crop_xmax - crop_xmin;
  int crop_height = crop_ymax - crop_ymin;

  if (crop_width < 2) {
    crop_xmin = bbox.x;
    crop_width = 2;
  }

  if (crop_height < 2) {
    crop_ymin = bbox.y;
    crop_height = 2;
  }

  cv::Rect crop_rect({crop_xmin, crop_ymin, crop_width, crop_height});

  return crop_rect;
}

std::pair<PoliceSuitClassify, float>
PoliceSuitImpl::calcSuit(const float *p_data, const size_t class_num) {
  if (nullptr == p_data || class_num <= 0) {
    return std::make_pair(INVALID_RESULT, 0.f);
  }

  std::vector<float> v_prob(class_num);
  memcpy(v_prob.data(), p_data, class_num * sizeof(float));

  float f_max_val = 0.f;
  int i_max_idx = softmax(v_prob, f_max_val);

  return std::make_pair(PoliceSuitClassify(i_max_idx), f_max_val);
}

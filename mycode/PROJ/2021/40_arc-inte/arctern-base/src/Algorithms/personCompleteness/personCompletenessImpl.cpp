/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
*  author:         chenbin
 *  Last modified:  2020.12.04
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#include <vector>
#include "personCompletenessImpl.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "common/file_buffer.h"
#include "common/algUtil.h"
#include "log/logger.h"
#include "utils/personCompletenessUtil.h"

using arctern::PersonCompletenessImpl;
using arctern::ErrorType;
using arctern::Result;
using arctern::ArcternRect;

PersonCompletenessImpl::PersonCompletenessImpl() : extScale_{0, 0, 0, 0} {
  input_h_ = 256;
  input_w_ = 128;
}

PersonCompletenessImpl::~PersonCompletenessImpl() noexcept {
  PersonCompletenessImpl::Release();
}

ErrorType PersonCompletenessImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                            std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<PersonCompletenessResult *>(result_);
  int total = endId - startId;
  int stride0 = outputs[0].size() / outputs[0].shape()[0]; // 2
  int stride1 = outputs[1].size() / outputs[1].shape()[0]; // 5
  //int stride2 = outputs[2].size() / outputs[2].shape()[0]; // 128
  int stride2 = 128;

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    const float *data1 = outputs[1].data() + subId * stride1;
    const float *data2 = outputs[2].data() + subId * stride2;
    parse_truncation(data0, stride0, r->personCompleteness_[startId + subId].truncation);
    parse_visible_parts(data1, stride1,
            r->personCompleteness_[startId + subId].visibleParts,
            r->personCompleteness_[startId + subId].visiblePartsScore);
    parse_soft_mask(data2, stride2,
            r->personCompleteness_[startId + subId].softMasks,
            r->personCompleteness_[startId + subId].softMasksScore);
  }

  return ErrorType::ERR_SUCCESS;
}

 cv::Rect PersonCompletenessImpl::get_roi(
          const int &img_width,
          const int &img_height,
          const cv::Rect &bbox,
          const std::array<float, 4> ext_scale) const {
    int height = img_height;
    int width = img_width;
    int w = bbox.width;
    int h = bbox.height;
    float x1 = bbox.tl().x;
    float y1 = bbox.tl().y;
    float x2 = bbox.br().x;
    float y2 = bbox.br().y;

    x1 -= w * ext_scale[0];
    y1 -= h * ext_scale[1];
    x2 += w * ext_scale[2];
    y2 += h * ext_scale[3];
    float bx1 = std::min(std::max(0, static_cast<int>(round(x1))), width - 1);
    float by1 = std::min(std::max(0, static_cast<int>(round(y1))), height - 1);
    float bx2 = std::min(std::max(0, static_cast<int>(round(x2))), width - 1);
    float by2 = std::min(std::max(0, static_cast<int>(round(y2))), height - 1);
    return cv::Rect(cv::Point(bx1, by1), cv::Point(bx2 + 1, by2 + 1));
  }


ArcternRect PersonCompletenessImpl::LocalGetRoi(const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const PersonCompletenessRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  auto rect = check_shrink_box(oriRect, p->imageV_[idx].width, p->imageV_[idx].height);
  rect = get_roi(p->imageV_[idx].width, p->imageV_[idx].height, rect, extScale_);
  ArcternRect result;
  result.x = rect.x;
  result.y = rect.y;
  result.width = rect.width;
  result.height = rect.height;
  return result;
}
void PersonCompletenessImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::splitNormalized);
}

void PersonCompletenessImpl::InitPreprocessorParam() {
  preProcessor_.reset(new OpencvPreprocessor);
  OpencvPreprocessorInitPt init_pt;
  init_pt.net_h_ = input_h_;
  init_pt.net_w_ = input_w_;
  init_pt.meanV_ = {0.4914, 0.4822,0.4465};  // rgb
  init_pt.stdV_ = {0.2023, 0.1994, 0.2010};  // rgb
  init_pt.border_value = 0;
  auto code = preProcessor_->Init(&init_pt);
  assert(code == ErrorType::ERR_SUCCESS);
}
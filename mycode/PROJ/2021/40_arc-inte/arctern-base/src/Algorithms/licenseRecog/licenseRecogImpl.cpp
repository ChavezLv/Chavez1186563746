/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.01
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#include <utility>
#include <vector>
#include "licenseRecogImpl.h"
#include "common/file_buffer.h"
#include "common/imageConvert.h"
#include "common/algUtil.h"
#include "log/logger.h"

using arctern::LicenseRecogImpl;
using arctern::ErrorType;
using arctern::Result;

LicenseRecogImpl::LicenseRecogImpl() {
  input_h_ = 48;
  input_w_ = 192;
}

LicenseRecogImpl::~LicenseRecogImpl() noexcept {
  LicenseRecogImpl::Release();
}

ErrorType LicenseRecogImpl::CheckRunParam(const RunParameter *p) {
  return ArcternAlgorithm::CheckRunParam(p);
}

ErrorType LicenseRecogImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                        std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<LicenseRecogResult *>(result_);

  int total = endId - startId;

  // const auto batchNum = initParam_.batchNum_;
  int stride0 = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    std::vector<float> res;
    const float *data0 = outputs[0].data() + subId * stride0;
    if (*(data0) != INDEXFORSPACE) {
      res.push_back(*(data0));
    }

    for (int i = 1; i < stride0; i++) {
      if(*(data0 + i) == *(data0 + i - 1) || 
            *(data0 + i) == INDEXFORSPACE) {
        continue;
      }
      res.push_back(*(data0 + i));
    }
    r->LicenseRecogs_[startId + subId] = calc_license(res);
  }
  return ErrorType::ERR_SUCCESS;
}

std::string LicenseRecogImpl::calc_license(const std::vector<float> &output) {
  std::string license = "";
  for (size_t i = 0; i < output.size(); i++) {
    int index = static_cast<int>(output[i]) < 0 ? 0 : static_cast<int>(output[i]);
    index = index > INDEXFORSPACE ? INDEXFORSPACE : index;
    if (LicenseCharacters[index] == "^") {
      break;
    }
    license = license + LicenseCharacters[index];
  }
  return license;
}

arctern::ArcternRect LicenseRecogImpl::LocalGetRoi(const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const LicenseRecogRunParameter *>(p);
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
void LicenseRecogImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::normalized);
}
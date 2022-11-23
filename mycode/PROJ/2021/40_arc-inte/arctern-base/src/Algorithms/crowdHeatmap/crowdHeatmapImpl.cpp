/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
*  author:         chenbin
 *  Last modified:  2020.12.18
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#include <vector>
#include "crowdHeatmapImpl.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "common/file_buffer.h"
#include "common/algUtil.h"
#include "log/logger.h"

using arctern::CrowdHeatmapImpl;
using arctern::ErrorType;
using arctern::Result;

CrowdHeatmapImpl::CrowdHeatmapImpl() {
  input_h_ = 432;
  input_w_ = 768;
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_BGR888;
}

CrowdHeatmapImpl::~CrowdHeatmapImpl() noexcept {
  CrowdHeatmapImpl::Release();
}


ErrorType CrowdHeatmapImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                            std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<CrowdHeatmapResult *>(result_);
  int total = endId - startId;
  int res_h = 54, res_w = 96;
  int stride0 = res_w * res_h;

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    cv::Mat fimage(res_h, res_w, CV_32FC1);
    std::memcpy(fimage.data, data0, sizeof(float)*stride0);
    cv::resize(fimage, fimage, cv::Size(input_w_, input_h_), CV_INTER_NN);
    r->crowdHeatmaps_[startId + subId].headCount = cv::sum(fimage)[0]/100;
    double maxv=0.0;
    cv::minMaxIdx(fimage, 0, &maxv);
    fimage = fimage/(maxv + 1e-20)*255;
    cv::imencode(".jpg", fimage, r->crowdHeatmaps_[startId + subId].heatmap);
  }

  return ErrorType::ERR_SUCCESS;
}


void CrowdHeatmapImpl::InitPreprocessorParam() {
  preProcessor_.reset(new OpencvPreprocessor);
  OpencvPreprocessorInitPt init_pt;
  init_pt.net_h_ = input_h_;
  init_pt.net_w_ = input_w_;
  init_pt.meanV_ = {0.431981861591, 0.447249650955, 0.452016860247};//bgr
  init_pt.stdV_ = {0.221840232611, 0.224925786257, 0.23242045939};
  init_pt.border_value = 0;
  auto code = preProcessor_->Init(&init_pt);
  assert(code == ErrorType::ERR_SUCCESS);

}
void CrowdHeatmapImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::splitNormalized);
  runParm->sequence_.push_back(OperatorType::resized);
}
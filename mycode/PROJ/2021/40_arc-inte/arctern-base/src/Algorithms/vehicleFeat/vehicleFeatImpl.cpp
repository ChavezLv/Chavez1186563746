// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.02
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#include "vehicleFeatImpl.h"
#include "common/file_buffer.h"
#include "log/logger.h"
#include "src/Processors/Algorithms/attrpreprocess.h"

#include "vehicleFeat.h"

using namespace arctern;

vehicleFeatImpl::vehicleFeatImpl() {
  input_h_ = 224;
  input_w_ = 224;
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_BGR888;
}

vehicleFeatImpl::~vehicleFeatImpl() { vehicleFeatImpl::Release(); }

ErrorType vehicleFeatImpl::Init(const InitParameter *p) {
  auto ret = ArcternAlgorithm::Init(p);
  if (ret != ErrorType::ERR_SUCCESS) {
    vehicleFeatImpl::Release();
    return ret;
  }

  fg vehicleFeatResult;

  preProcessor_.reset(new AttrPreProcesser);

  arctern::AttrPreProInitParameter initParam;
  initParam.netH_ = input_h_;
  initParam.netW_ = input_w_;
  initParam.mean_ = 127.5f;
  initParam.std_ = 1.f;
  initParam.alignFace_ = AlignFace::NoAlignFace;

  assert(preProcessor_);

  auto code = preProcessor_->Init(&initParam);
  assert(code == ErrorType::ERR_SUCCESS);

  return ErrorType::ERR_SUCCESS;
}

void vehicleFeatImpl::Release() {}


int vehicleFeatImpl::GetConfigInfo(arctern::MxnetModel *model) { return 0; }

cv::Mat vehicleFeatImpl::LocalGetFace(const cv::Mat &image,
                                      const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const vehicleFeatRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);
  auto rect = check_shrink_box(oriRect, image.cols, image.rows);
  cv::Mat face = image(rect).clone();
  return face;
}

ErrorType vehicleFeatImpl::LocalPostProcess(
    const RunParameter *p, int startId, int endId,
    std::vector<arctern::Tensor<float>> &outputs) {
  auto res = dynamic_cast<vehicleFeatResult *>(result_);
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



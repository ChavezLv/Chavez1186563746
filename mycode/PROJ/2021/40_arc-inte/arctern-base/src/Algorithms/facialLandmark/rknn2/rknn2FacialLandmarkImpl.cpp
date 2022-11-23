/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.12.02
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/
#include "rknn2FacialLandmarkImpl.h"

using arctern::ErrorType;
using arctern::Rknn2FacialLandmarkImpl;

Rknn2FacialLandmarkImpl::Rknn2FacialLandmarkImpl() : FacialLandmarkImpl() {
  isNormaliized_ = false;
  netShape_ = NetworkShape::NHWC;
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_BGR888;
}

Rknn2FacialLandmarkImpl::~Rknn2FacialLandmarkImpl() {
}

ErrorType Rknn2FacialLandmarkImpl::Init(const InitParameter *p) {
  // rknn not support batch
  assert(p->batchNum_ == 1);

  return FacialLandmarkImpl::Init(p);
}

ErrorType Rknn2FacialLandmarkImpl::LocalPostProcess(const RunParameter *p,
                                                   int startId,
                                                   int endId,
                                                   std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<FacialLandmarkResult *>(result_);

  int offset = 0;
  int total = endId - startId;
  float *ret = outputs[0].data();
  int stride = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    auto &faceRect = faceRects_[startId + subId];

    for (int i = 0; i < num_landmark_; i++) {
      float tmpX = ret[i * 2 + offset];
      float tmpY = ret[i * 2 + 1 + offset];
      float coor_x = tmpX * faceRect.width + faceRect.x;
      float coor_y = tmpY * faceRect.height + faceRect.y;

      r->landmarks_[startId + subId].push_back(ArcternPoint({coor_x, coor_y}));
    }

    offset += stride;
  }

  return ErrorType::ERR_SUCCESS;
}


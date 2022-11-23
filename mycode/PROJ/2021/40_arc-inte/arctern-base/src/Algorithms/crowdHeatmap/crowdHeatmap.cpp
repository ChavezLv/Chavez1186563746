/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.18
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "crowdHeatmap.h"
#ifdef MXNET_ALLOWED
#include "./crowdHeatmapImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif TRT_ALLOWED
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "trt/TrtCrowdHeatMapImpl.h"
#endif
using arctern::CrowdHeatmap;
using arctern::CrowdHeatmapResult;
using arctern::ErrorType;
using arctern::Result;

CrowdHeatmapResult::CrowdHeatmapResult() {
}

CrowdHeatmapResult::~CrowdHeatmapResult() {
}

CrowdHeatmap::CrowdHeatmap() : impl_(nullptr) {
}

CrowdHeatmap::~CrowdHeatmap() noexcept {
}

ErrorType CrowdHeatmap::Init(const InitParameter *p) {
  Release();
#ifdef MXNET_ALLOWED
  impl_ = new arctern::CrowdHeatmapImpl();
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif TRT_ALLOWED
  impl_ = new arctern::TrtCrowdHeatMapImpl;
  auto ret =arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif

  return ret;
}

ErrorType CrowdHeatmap::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}


void CrowdHeatmap::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

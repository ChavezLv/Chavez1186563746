/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.19
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "facialLandmark.h"
#include "./src/log/logger.h"

#ifdef MXNET_ALLOWED
#include "./facialLandmarkImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif defined(HISI_ALLOWED)
#include "src/Algorithms/HisiInterfaceCommonOperation.h"
#include "./hisi/hisiFacialLandmarkImpl.h"
#elif defined(TVM_ALLOWED)
#include "src/Algorithms/TvmInterfaceCommonOperation.h"
#include "./tvm/tvmFacialLandmarkImpl.h"
#elif defined(RKNN2_ALLOWED)
#include "src/Algorithms/Rknn2InterfaceCommonOperation.h"
#include "./rknn2/rknn2FacialLandmarkImpl.h"
#elif defined(TRT_ALLOWED)
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "src/Algorithms/facialLandmark/trt/TrtFacialLandmarkImpl.h"
#elif defined(ATLAS_ALLOWED)
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "src/Algorithms/facialLandmark/atlas/atlasFacialLandmarkImpl.h"
#elif defined(RKNN_ALLOWED)
#include "src/Algorithms/RknnInterfaceCommonOperation.h"
#include "./rknn/rknnFacialLandmarkImpl.h"
#endif

using arctern::FacialLandmarkRunParameter;
using arctern::FacialLandmark;
using arctern::FacialLandmarkResult;
using arctern::ErrorType;
using arctern::Result;

FacialLandmarkRunParameter::FacialLandmarkRunParameter() {
}

FacialLandmarkRunParameter::~FacialLandmarkRunParameter() noexcept {
}

FacialLandmarkResult::FacialLandmarkResult() {
}

FacialLandmarkResult::~FacialLandmarkResult() noexcept {
}

FacialLandmark::FacialLandmark() : impl_(nullptr) {
}

FacialLandmark::~FacialLandmark() noexcept {
  FacialLandmark::Release();
}

ErrorType FacialLandmark::Init(const InitParameter *p) {
  Release();

  ErrorType ret = ErrorType::ERR_SUCCESS;

#ifdef MXNET_ALLOWED
  impl_ = new arctern::FacialLandmarkImpl();
  ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(HISI_ALLOWED)
  impl_ = new arctern::HisiFacialLandmarkImpl;
  ret = arctern::HisiInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TVM_ALLOWED)
  impl_ = new arctern::TvmFacialLandmarkImpl;
  ret = arctern::TvmInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(RKNN2_ALLOWED)
  impl_ = new arctern::Rknn2FacialLandmarkImpl;
  ret = arctern::Rknn2InterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TRT_ALLOWED)
  impl_ = new arctern::TrtFacialLandmarkImpl;
  ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#elif defined(ATLAS_ALLOWED)
  impl_ = new AtlasFacialLandmarkImpl;
  ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#elif defined(RKNN_ALLOWED)
  impl_ = new arctern::RknnFacialLandmarkImpl;
  ret = arctern::RknnInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

void FacialLandmark::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

ErrorType FacialLandmark::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}
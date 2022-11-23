/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.14
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "faceDetectTiny.h"
#include "../src/log/logger.h"

#ifdef MXNET_ALLOWED
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#include "./faceDetectTinyImpl.h"
#elif defined(HISI_ALLOWED)
#include "src/Algorithms/HisiInterfaceCommonOperation.h"
#include "./hisi/hisiTinyyoloImpl.h"
#elif defined(TVM_ALLOWED)
#include "src/Algorithms/TvmInterfaceCommonOperation.h"
#include "./tvm/tvmTinyyoloImpl.h"
#elif defined(TRT_ALLOWED)
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "./trt/TrtFaceDetectTiny.h"
#elif defined(RKNN_ALLOWED)
#include "src/Algorithms/RknnInterfaceCommonOperation.h"
#include "rknn/rknnFaceDetectTinyImpl.h"
#elif defined(RKNN2_ALLOWED)
#include "src/Algorithms/Rknn2InterfaceCommonOperation.h"
#include "rknn2/rknn2FaceDetectTinyImpl.h"
#elif defined(MNN_ALLOWED)
#include "src/Algorithms/MnnInterfaceCommonOperation.h"
#include "mnn/mnnFaceDetectTinyImpl.h"
#elif defined(ATLAS_ALLOWED)
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "atlas/atlasFaceDetectTinyImpl.h"
#endif

using namespace arctern;

FaceDetectTinyRunParameter::FaceDetectTinyRunParameter() {

}

FaceDetectTinyRunParameter::~FaceDetectTinyRunParameter() {

}

ErrorType FaceDetectTinyRunParameter::CheckSize() const {
  return ERR_SUCCESS;
}

FaceDetectTinyResult::FaceDetectTinyResult() {
}

FaceDetectTinyResult::~FaceDetectTinyResult() noexcept {
}

FaceDetectTiny::FaceDetectTiny() : impl_(nullptr) {
}

FaceDetectTiny::~FaceDetectTiny() noexcept {
  FaceDetectTiny::Release();
}

ErrorType FaceDetectTiny::Init(const InitParameter *p) {
  /// first delete the object, then new it
  Release();

  ErrorType ret = ErrorType::ERR_SUCCESS;

#ifdef MXNET_ALLOWED
  impl_ = new arctern::faceDetectTinyImpl;
  ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(HISI_ALLOWED)
  impl_ = new arctern::HisiYoloV3;
  ret = arctern::HisiInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TVM_ALLOWED)
  impl_ = new arctern::TvmYoloV3;
  ret = arctern::TvmInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TRT_ALLOWED)
  impl_ = new arctern::TrtFaceDetectTinyImpl;
  ret = arctern::TrtInterfaceCommonOperation::Init(dynamic_cast<ArcternAlgorithmEx*>(impl_),p);
#elif defined(RKNN2_ALLOWED)
  impl_ = new arctern::Rknn2FaceDetectTinyImpl;
   ret = arctern::Rknn2InterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(MNN_ALLOWED)
   impl_ = new arctern::MnnFaceDetectTinyImpl;
   ret = arctern::MnnInterfaceCommonOperation::Init((ArcternAlgorithm*)impl_, p);
#elif defined(ATLAS_ALLOWED)
  impl_ = new arctern::AtlasFaceDetectTinyImpl;
  ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#elif defined(RKNN_ALLOWED)
  impl_ = new arctern::RknnFaceDetectTinyImpl;
   ret = arctern::RknnInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

void FaceDetectTiny::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

ErrorType FaceDetectTiny::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    const auto faceDetectTinyRunParam = dynamic_cast<const FaceDetectTinyRunParameter*>(p);
    assert(faceDetectTinyRunParam != nullptr);
    return impl_->Process(p,r);
  }
  return ERR_UNEXPECT;
}



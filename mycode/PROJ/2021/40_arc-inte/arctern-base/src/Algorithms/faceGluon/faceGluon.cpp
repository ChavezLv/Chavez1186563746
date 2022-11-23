/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.21
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include "faceGluon.h"
#include "log/logger.h"

#ifdef MXNET_ALLOWED
#include "./src/Algorithms/MxnetInterfaceCommonOperation.h"
#include "./faceGluonImpl.h"
#elif defined(HISI_ALLOWED)
#include "src/Algorithms/HisiInterfaceCommonOperation.h"
#include "./hisi/hisiFaceGluonImpl.h"
#elif defined(TVM_ALLOWED)
#include "src/Algorithms/TvmInterfaceCommonOperation.h"
#include "./tvm/tvmFaceGluonImpl.h"
#elif defined(RKNN_ALLOWED)
#include "src/Algorithms/RknnInterfaceCommonOperation.h"
#include "rknn/rknnFaceGluonImpl.h"
#elif  defined(TRT_ALLOWED)
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "src/Algorithms/faceGluon/trt/TrtFaceGluonImpl.h"
#elif defined(RKNN2_ALLOWED)
#include "src/Algorithms/Rknn2InterfaceCommonOperation.h"
#include "rknn2/rknn2FaceGluonImpl.h"
#elif defined(ATLAS_ALLOWED)
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "atlas/atlasFaceGluonImpl.h"
#endif

using arctern::FaceGluonRunParameter;
using arctern::FaceGluonResult;
using arctern::FaceGluon;
using arctern::ErrorType;
using arctern::Result;

FaceGluonRunParameter::FaceGluonRunParameter() {
}

FaceGluonRunParameter::~FaceGluonRunParameter() noexcept {
}

FaceGluonResult::FaceGluonResult() {
}

FaceGluonResult::~FaceGluonResult() noexcept {
}

FaceGluon::FaceGluon() : impl_(nullptr) {
}

FaceGluon::~FaceGluon() noexcept {
  FaceGluon::Release();
}

ErrorType FaceGluon::Init(const InitParameter *p) {
  Release();

  ErrorType ret = ErrorType::ERR_SUCCESS;

#ifdef MXNET_ALLOWED
  impl_ = new arctern::FaceGluonImpl();
  ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(HISI_ALLOWED)
  impl_ = new arctern::HisiFaceGluonImpl();
  ret = arctern::HisiInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TVM_ALLOWED)
  impl_ = new arctern::TvmFaceGluonImpl();
  ret = arctern::TvmInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(RKNN_ALLOWED)
  impl_ = new arctern::RknnFaceGluonImpl();
  ret = arctern::RknnInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TRT_ALLOWED)
  impl_ = new arctern::TrtFaceGluonImpl;
  ret = arctern::TrtInterfaceCommonOperation::Init(dynamic_cast<ArcternAlgorithmEx*>(impl_),p);
#elif defined(RKNN2_ALLOWED)
  impl_ = new arctern::Rknn2FaceGluonImpl();
  ret = arctern::Rknn2InterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(ATLAS_ALLOWED)
  impl_ = new arctern::AtlasFaceGluonImpl;
  ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

void FaceGluon::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

ErrorType FaceGluon::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    auto param = dynamic_cast<const FaceGluonRunParameter*>(p);
    assert(param != nullptr);
    return impl_->Process(p,r);
  }
  SPDLOG_ERROR("error!!!!you must initialize  the algorithm!!!!");
  return ERR_UNEXPECT;
}






/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.17
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "faceQuality.h"
#ifdef MXNET_ALLOWED
#include "./faceQualityImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif defined(HISI_ALLOWED)
#include "src/Algorithms/HisiInterfaceCommonOperation.h"
#include "./hisi/hisiFaceQualityImpl.h"
#elif defined(TVM_ALLOWED)
#include "src/Algorithms/TvmInterfaceCommonOperation.h"
#include "./tvm/tvmFaceQualityImpl.h"
#elif defined(RKNN_ALLOWED)
#include "src/Algorithms/RknnInterfaceCommonOperation.h"
#include "rknn/rknnFaceQualityImpl.h"
#elif defined(TRT_ALLOWED)
#include "src/Algorithms/faceQuality/trt/TrtFaceQualityImpl.h"
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#elif defined(RKNN2_ALLOWED)
#include "src/Algorithms/Rknn2InterfaceCommonOperation.h"
#include "rknn2/rknn2FaceQualityImpl.h"
#elif defined(ATLAS_ALLOWED)
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "atlas/atlasFaceQualityImpl.h"
#endif

using arctern::FaceQuality;
using arctern::FaceQualityRunParameter;
using arctern::FaceQualityResult;
using arctern::ErrorType;
using arctern::Result;

FaceQualityRunParameter::FaceQualityRunParameter() {
}

FaceQualityRunParameter::~FaceQualityRunParameter()  {
}

FaceQualityResult::FaceQualityResult() {
}

FaceQualityResult::~FaceQualityResult() {
}

FaceQuality::FaceQuality() : impl_(nullptr) {
}

FaceQuality::~FaceQuality()  {
  FaceQuality::Release();
}

ErrorType FaceQuality::Init(const InitParameter *p) {
  Release();

  ErrorType ret = ErrorType::ERR_SUCCESS;

#ifdef MXNET_ALLOWED
  impl_ = new arctern::FaceQualityImpl();
  ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(HISI_ALLOWED)
  impl_ = new arctern::HisiFaceQualityImpl();
  ret = arctern::HisiInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TVM_ALLOWED)
  impl_ = new arctern::TvmFaceQualityImpl();
  ret = arctern::TvmInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(RKNN_ALLOWED)
  impl_ = new arctern::RknnFaceQualityImpl();
  ret = arctern::RknnInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TRT_ALLOWED)
  impl_ = new arctern::TrtFaceQualityImpl;
  ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#elif defined(RKNN2_ALLOWED)
  impl_ = new arctern::Rknn2FaceQualityImpl();
  ret = arctern::Rknn2InterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(ATLAS_ALLOWED)
  impl_ = new AtlasFaceQualityImpl;
  ret = AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif
  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

ErrorType FaceQuality::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void FaceQuality::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

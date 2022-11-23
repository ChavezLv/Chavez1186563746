/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.14
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "faceAbnomalLight.h"

#ifdef MXNET_ALLOWED
#include "./faceAbnomalLightImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"

#elif defined(TVM_ALLOWED)
#include "src/Algorithms/TvmInterfaceCommonOperation.h"
#include "tvm/tvmFaceAbnomalLightImpl.h"
#elif TRT_ALLOWED
#include "src/Algorithms/faceAbnomalLight/trt/TrtFaceAbnormalLightImpl.h"
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#endif
using namespace arctern;
using arctern::FaceAbnomalLight;
using arctern::FaceAbnomalLightRunParameter;
using arctern::FaceAbnomalLightResult;
using arctern::ErrorType;
using arctern::Result;

FaceAbnomalLightRunParameter::FaceAbnomalLightRunParameter() {
}

FaceAbnomalLightRunParameter::~FaceAbnomalLightRunParameter() noexcept {
}

FaceAbnomalLightResult::FaceAbnomalLightResult() {
}

FaceAbnomalLightResult::~FaceAbnomalLightResult() {
  FaceAbnomalLightResult::Release();
}

FaceAbnomalLight::FaceAbnomalLight() : impl_(nullptr) {
}

FaceAbnomalLight::~FaceAbnomalLight() noexcept {
    cout <<"deconstruct"<<endl;
    Release();

}

ErrorType FaceAbnomalLight::Init(const InitParameter *p) {
  Release();

#ifdef MXNET_ALLOWED
  impl_ =(AlgorithmBase*) new FaceAbnomalLightImpl;
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
/*
  impl_ = new TrtFaceAbnormalLightImpl;
  ErrorType ret = impl_->Init(p);
*/
 #elif defined(TVM_ALLOWED)
  impl_ = new TvmFaceAbnomalLightImpl;
  auto ret = arctern::TvmInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
 #elif  defined(TRT_ALLOWED)
  impl_ = new TrtFaceAbnormalLightImpl;
  auto ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_, p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }
  return ret;
}

ErrorType FaceAbnomalLight::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }
  return ERR_UNEXPECT;
}

void FaceAbnomalLight::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

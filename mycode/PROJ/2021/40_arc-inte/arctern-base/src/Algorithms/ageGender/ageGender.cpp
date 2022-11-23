/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.31
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "ageGender.h"



#ifdef MXNET_ALLOWED
#include "ageGenderImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif TVM_ALLOWED
#include "src/Algorithms/TvmInterfaceCommonOperation.h"
#include "tvm/tvmAgeGenderImpl.h"
#elif TRT_ALLOWED
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "src/Algorithms/ageGender/trt/TrtAgeGenderImpl.h"
#elif ATLAS_ALLOWED
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "atlas/atlasAgeGenderImpl.h"
#endif
using arctern::AgeGender;
using arctern::AgeGenderRunParameter;
using arctern::AgeGenderResult;
using arctern::ErrorType;
using arctern::Result;

AgeGenderRunParameter::AgeGenderRunParameter() {
}

AgeGenderRunParameter::~AgeGenderRunParameter() noexcept {
}

AgeGenderResult::AgeGenderResult() {
}

AgeGenderResult::~AgeGenderResult() {
}

AgeGender::AgeGender() : impl_(nullptr) {
}

AgeGender::~AgeGender() noexcept {
}

ErrorType AgeGender::Init(const InitParameter *p) {
  Release();

#ifdef MXNET_ALLOWED
  impl_ = new AgeGenderImpl;
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif  TVM_ALLOWED
  impl_ = new TvmAgeGenderImpl;
  ErrorType ret = arctern::TvmInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif  TRT_ALLOWED
  impl_ = new TrtAgeGenderImpl;
  auto ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#elif ATLAS_ALLOWED
  impl_ = new AtlasAgeGenderImpl;
  auto ret =arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif

  if(ErrorType::ERR_SUCCESS != ret) {
    delete impl_;
    impl_ = nullptr;
  }
  return ret;
}

ErrorType AgeGender::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void AgeGender::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

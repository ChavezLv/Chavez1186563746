/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.01
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "personHeadwear.h"
#ifdef MXNET_ALLOWED
#include "./personHeadwearImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif defined(TRT_ALLOWED)
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#include "src/Algorithms/personHeadwear/trt/TrtPersonHeadwearImpl.h"
#elif defined(ATLAS_ALLOWED)
#include "src/Algorithms/personHeadwear/atlas/atlasPersonHeadwearImpl.h"
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#endif
using arctern::PersonHeadwear;
using arctern::PersonHeadwearRunParameter;
using arctern::PersonHeadwearResult;
using arctern::ErrorType;
using arctern::Result;

PersonHeadwearRunParameter::PersonHeadwearRunParameter() {
}

PersonHeadwearRunParameter::~PersonHeadwearRunParameter() noexcept {
}

PersonHeadwearResult::PersonHeadwearResult() {
}

PersonHeadwearResult::~PersonHeadwearResult() {
}

PersonHeadwear::PersonHeadwear() : impl_(nullptr) {
}

PersonHeadwear::~PersonHeadwear() noexcept {
  PersonHeadwear::Release();
}

ErrorType PersonHeadwear::Init(const InitParameter *p) {
  Release();
#ifdef MXNET_ALLOWED
  impl_ = new arctern::PersonHeadwearImpl();
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif defined(TRT_ALLOWED)
  impl_ = new arctern::TrtPersonHeadwearImpl;
  auto ret = arctern::TrtInterfaceCommonOperation::Init(dynamic_cast<ArcternAlgorithmEx*>(impl_),p);
#elif defined(ATLAS_ALLOWED)
  impl_ = new AtlasPersonHeadwearImpl;
  auto ret = AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

ErrorType PersonHeadwear::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void PersonHeadwear::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

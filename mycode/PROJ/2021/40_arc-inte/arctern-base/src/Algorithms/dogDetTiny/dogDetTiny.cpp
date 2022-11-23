/****************************************************************************
 *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         kuanghongshen
 *  Last modified:  2021.06.04
 *  email:          kuang.hongshen@intellif.com
 ****************************************************************************/

#include "dogDetTiny.h"
#include "../src/log/logger.h"
#ifdef MXNET_ALLOWED
#include "./dogDetTinyImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif defined(TRT_ALLOWED)
#include "src/Algorithms/dogDetTiny/trt/TrtDogDetTiny.h"
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#endif
namespace arctern {

dogDetTinyRunParameter::dogDetTinyRunParameter() {

}

dogDetTinyRunParameter::~dogDetTinyRunParameter() {

}

ErrorType dogDetTinyRunParameter::CheckSize() const {
  return ERR_SUCCESS;
}

dogDetTinyResult::dogDetTinyResult() {}

dogDetTinyResult::~dogDetTinyResult() noexcept {}

dogDetTiny::dogDetTiny() : impl_{} {}

dogDetTiny::~dogDetTiny() noexcept { dogDetTiny::Release(); }

ErrorType dogDetTiny::Init(const InitParameter *p) {
  Release();
  ErrorType ret = ErrorType::ERR_SUCCESS;
#ifdef MXNET_ALLOWED
  impl_ = new arctern::dogDetTinyImpl;
  ret = arctern::MxnetInterfaceCommonOperation::Init(
      (ArcternAlgorithm **) &impl_, p);
#elif defined(TRT_ALLOWED)
  impl_ = new arctern::TrtDogDetTinyImpl;
  ret =arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif
  if (ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

void dogDetTiny::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

ErrorType dogDetTiny::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    const auto dogDetTinyRunParam = dynamic_cast<const dogDetTinyRunParameter*>(p);
    assert(dogDetTinyRunParam != nullptr);
    return impl_->Process(p,r);
  }
  return ERR_UNEXPECT;
}

} // namespace arctern

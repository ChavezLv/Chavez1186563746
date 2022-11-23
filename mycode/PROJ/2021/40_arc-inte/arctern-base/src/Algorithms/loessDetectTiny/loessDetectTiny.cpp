
#include "loessDetectTiny.h"
#include "../src/log/logger.h"

#ifdef  ATLAS_ALLOWED
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "atlas/atlasLoessDetectTinyImpl.h"
#endif

using namespace arctern;

LoessDetectTinyRunParameter::LoessDetectTinyRunParameter() {

}

LoessDetectTinyRunParameter::~LoessDetectTinyRunParameter() {

}

ErrorType LoessDetectTinyRunParameter::CheckSize() const {
  return ERR_SUCCESS;
}

LoessDetectTinyResult::LoessDetectTinyResult() {
}

LoessDetectTinyResult::~LoessDetectTinyResult() noexcept {
}

LoessDetectTiny::LoessDetectTiny() : impl_(nullptr) {
}

LoessDetectTiny::~LoessDetectTiny() noexcept {
  LoessDetectTiny::Release();
}

ErrorType LoessDetectTiny::Init(const InitParameter *p) {
  /// first delete the object, then new it
  Release();

  ErrorType ret = ErrorType::ERR_SUCCESS;

#ifdef ATLAS_ALLOWED
  impl_ = new arctern::AtlasLoessDetectTinyImpl;
  ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

void LoessDetectTiny::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

ErrorType LoessDetectTiny::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    const auto faceDetectTinyRunParam = dynamic_cast<const LoessDetectTinyRunParameter*>(p);
    assert(faceDetectTinyRunParam != nullptr);
    return impl_->Process(p,r);
  }
  return ERR_UNEXPECT;
}



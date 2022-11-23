

#include "fireDet.h"
#include "../src/log/logger.h"

#ifdef ATLAS_ALLOWED
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "atlas/atlasFireDetImpl.h"
#elif MXNET_ALLOWED
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#include "fireDetImpl.h"
#endif

using namespace arctern;

FireDetRunParameter::FireDetRunParameter() {

}

FireDetRunParameter::~FireDetRunParameter() {

}

ErrorType FireDetRunParameter::CheckSize() const {
  return ERR_SUCCESS;
}

FireDetResult::FireDetResult() {
}

FireDetResult::~FireDetResult() noexcept {
}

FireDet::FireDet() : impl_(nullptr) {
}

FireDet::~FireDet() noexcept {
  FireDet::Release();
}

ErrorType FireDet::Init(const InitParameter *p) {
  /// first delete the object, then new it
  Release();

  ErrorType ret = ErrorType::ERR_SUCCESS;

#ifdef ATLAS_ALLOWED
  impl_ = new arctern::AtlasFireDetImpl;
  ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#elif MXNET_ALLOWED
  impl_ = new arctern::FireDetImpl;
  ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**) &impl_,p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

void FireDet::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

ErrorType FireDet::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    const auto faceDetectTinyRunParam = dynamic_cast<const FireDetRunParameter*>(p);
    assert(faceDetectTinyRunParam != nullptr);
    return impl_->Process(p,r);
  }
  return ERR_UNEXPECT;
}



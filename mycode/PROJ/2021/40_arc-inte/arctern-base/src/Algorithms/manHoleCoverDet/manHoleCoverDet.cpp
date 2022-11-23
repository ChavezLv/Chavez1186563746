#include "manHoleCoverDet.h"
#include "../src/log/logger.h"


#ifdef MXNET_ALLOWED
#include "./manHoleCoverDetImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"

#elif ATLAS_ALLOWED
#include "atlas/atlasManHoleCoverDetImpl.h"
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"

#elif TRT_ALLOWED
#include "src/Algorithms/manHoleCoverDet/trt/TrtManHoleCoverDetImpl.h"
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#endif

using namespace arctern;

ManHoleCoverDetRunParameter::ManHoleCoverDetRunParameter() {

}

ManHoleCoverDetRunParameter::~ManHoleCoverDetRunParameter() {

}

ErrorType ManHoleCoverDetRunParameter::CheckSize() const {
  return ERR_SUCCESS;
}

ManHoleCoverDetResult::ManHoleCoverDetResult() {
}

ManHoleCoverDetResult::~ManHoleCoverDetResult() noexcept {
}

ManHoleCoverDet::ManHoleCoverDet() : impl_(nullptr) {
}

ManHoleCoverDet::~ManHoleCoverDet() noexcept {
  ManHoleCoverDet::Release();
}

ErrorType ManHoleCoverDet::Init(const InitParameter *p) {
  /// first delete the object, then new it
  Release();

  ErrorType ret = ErrorType::ERR_SUCCESS;
#ifdef MXNET_ALLOWED
  impl_ = new arctern::ManHoleCoverDetImpl;
  ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif ATLAS_ALLOWED
  impl_ = new arctern::AtlasManHoleCoverDetImpl;
  ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#elif TRT_ALLOWED
  impl_ = new arctern::TrtManHoleCoverDetectImpl;
  ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_, p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }
  else{
    std::cout << "ManHoleCoverDet::Init" << ret << std::endl;
  }

  return ret;
}

void ManHoleCoverDet::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

ErrorType ManHoleCoverDet::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    const auto bannerDetRunParam = dynamic_cast<const ManHoleCoverDetRunParameter*>(p);
    assert(bannerDetRunParam != nullptr);
    return impl_->Process(p,r);
  }
  return ERR_UNEXPECT;
}



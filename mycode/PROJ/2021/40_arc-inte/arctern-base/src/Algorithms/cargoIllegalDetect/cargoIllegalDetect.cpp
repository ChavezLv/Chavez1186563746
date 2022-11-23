#include "cargoIllegalDetect.h"
#include "../src/log/logger.h"


#ifdef MXNET_ALLOWED
#include "./cargoIllegalDetectImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"

#elif TRT_ALLOWED
#include "src/Algorithms/cargoIllegalDetect/trt/TrtCargoIllegalDetectImpl.h"
#include "src/Algorithms/TrtInterfaceCommonOperation.h"

#endif

using namespace arctern;

CargoIllegalDetectRunParameter::CargoIllegalDetectRunParameter() {

}

CargoIllegalDetectRunParameter::~CargoIllegalDetectRunParameter() {

}

ErrorType CargoIllegalDetectRunParameter::CheckSize() const {
  return ERR_SUCCESS;
}

CargoIllegalDetectResult::CargoIllegalDetectResult() {
}

CargoIllegalDetectResult::~CargoIllegalDetectResult() noexcept {
}

CargoIllegalDetect::CargoIllegalDetect() : impl_(nullptr) {
}

CargoIllegalDetect::~CargoIllegalDetect() noexcept {
  CargoIllegalDetect::Release();
}

ErrorType CargoIllegalDetect::Init(const InitParameter *p) {
  /// first delete the object, then new it
  Release();

  ErrorType ret = ErrorType::ERR_SUCCESS;
#ifdef MXNET_ALLOWED
  impl_ = new arctern::CargoIllegalDetectImpl;
  ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif TRT_ALLOWED
  impl_ = new arctern::TrtCargoIllegalDetectectImpl;
  ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_, p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }
  else{
    std::cout << "CargoIllegalDetect::Init" << ret << std::endl;
  }

  return ret;
}

void CargoIllegalDetect::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

ErrorType CargoIllegalDetect::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    const auto bannerDetRunParam = dynamic_cast<const CargoIllegalDetectRunParameter*>(p);
    assert(bannerDetRunParam != nullptr);
    return impl_->Process(p,r);
  }
  return ERR_UNEXPECT;
}



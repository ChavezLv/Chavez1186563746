#include "bannerDet.h"
#include "../src/log/logger.h"


#ifdef MXNET_ALLOWED
#include "./bannerDetImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"

#elif ATLAS_ALLOWED
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "atlas/atlasBannerDetImpl.h"

#elif TRT_ALLOWED
#include "src/Algorithms/bannerDet/trt/TrtBannerDetImpl.h"
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#endif

using namespace arctern;

BannerDetRunParameter::BannerDetRunParameter() {

}

BannerDetRunParameter::~BannerDetRunParameter() {

}

ErrorType BannerDetRunParameter::CheckSize() const {
  return ERR_SUCCESS;
}

BannerDetResult::BannerDetResult() {
}

BannerDetResult::~BannerDetResult() noexcept {
}

BannerDet::BannerDet() : impl_(nullptr) {
}

BannerDet::~BannerDet() noexcept {
  BannerDet::Release();
}

ErrorType BannerDet::Init(const InitParameter *p) {
  /// first delete the object, then new it
  Release();

  ErrorType ret = ErrorType::ERR_SUCCESS;
#ifdef MXNET_ALLOWED
  impl_ = new arctern::BannerDetImpl;
  ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif ATLAS_ALLOWED
  impl_ = new arctern::AtlasBannerDetImpl;
  ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#elif TRT_ALLOWED
  impl_ = new arctern::TrtBannerDetectImpl;
  ret = arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_, p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }
  else{
    std::cout << "BannerDet::Init" << ret << std::endl;
  }

  return ret;
}

void BannerDet::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

ErrorType BannerDet::Process(const RunParameter *p,Result *r) {
  std::cout << "BannerDet::Process" << std::endl;
  if (nullptr != impl_) {
    const auto bannerDetRunParam = dynamic_cast<const BannerDetRunParameter*>(p);
    assert(bannerDetRunParam != nullptr);
    return impl_->Process(p,r);
  }else{
      std::cout << "BannerDet::Process--impl_ is nullptr! " << std::endl;
  }
  return ERR_UNEXPECT;
}



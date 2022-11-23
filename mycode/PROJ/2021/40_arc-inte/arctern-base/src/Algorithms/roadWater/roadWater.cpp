

#include "roadWater.h"
#include "../src/log/logger.h"

#ifdef ATLAS_ALLOWED
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "atlas/atlasRoadWaterImpl.h"
#endif

using namespace arctern;

RoadWaterRunParameter::RoadWaterRunParameter() {

}

RoadWaterRunParameter::~RoadWaterRunParameter() {

}

ErrorType RoadWaterRunParameter::CheckSize() const {
  return ERR_SUCCESS;
}

RoadWaterResult::RoadWaterResult() {
}

RoadWaterResult::~RoadWaterResult() noexcept {
}

RoadWater::RoadWater() : impl_(nullptr) {
}

RoadWater::~RoadWater() noexcept {
  RoadWater::Release();
}

ErrorType RoadWater::Init(const InitParameter *p) {
  /// first delete the object, then new it
  Release();

  ErrorType ret = ErrorType::ERR_SUCCESS;

#ifdef ATLAS_ALLOWED
  impl_ = new arctern::AtlasRoadWaterImpl;
  ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

void RoadWater::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

ErrorType RoadWater::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    const auto faceDetectTinyRunParam = dynamic_cast<const RoadWaterRunParameter*>(p);
    assert(faceDetectTinyRunParam != nullptr);
    return impl_->Process(p,r);
  }
  return ERR_UNEXPECT;
}



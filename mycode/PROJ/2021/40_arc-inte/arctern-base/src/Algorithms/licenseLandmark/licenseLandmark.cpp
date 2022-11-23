
#include "licenseLandmark.h"
#ifdef MXNET_ALLOWED
#include "licenseLandmarkImpl.h"
#include "src/Algorithms/MxnetInterfaceCommonOperation.h"
#elif TRT_ALLOWED
#include "src/Algorithms/licenseLandmark/trt/TrtLicenseLandmarkImpl.h"
#include "src/Algorithms/TrtInterfaceCommonOperation.h"
#endif
using arctern::LicenseLandmark;
using arctern::LicenseLandmarkRunParameter;
using arctern::LicenseLandmarkResult;
using arctern::ErrorType;
using arctern::Result;

LicenseLandmarkRunParameter::LicenseLandmarkRunParameter() {
}

LicenseLandmarkRunParameter::~LicenseLandmarkRunParameter() noexcept {
}

ErrorType LicenseLandmarkRunParameter::CheckSize() const {
  return ERR_SUCCESS;
}

LicenseLandmarkResult::LicenseLandmarkResult() {
}

LicenseLandmarkResult::~LicenseLandmarkResult() {
}

LicenseLandmark::LicenseLandmark() : impl_(nullptr) {
}

LicenseLandmark::~LicenseLandmark() noexcept {
  LicenseLandmark::Release();
}

ErrorType LicenseLandmark::Init(const InitParameter *p) {
  Release();
#ifdef  MXNET_ALLOWED
  impl_ = new arctern::LicenseLandmarkImpl();
  ErrorType ret = arctern::MxnetInterfaceCommonOperation::Init((ArcternAlgorithm**)&impl_, p);
#elif TRT_ALLOWED
  impl_ = new arctern::TrtLicenseLandmarkImpl;
  auto ret =arctern::TrtInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif
  return ret;
}

ErrorType LicenseLandmark::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    const auto RunParam = dynamic_cast<const LicenseLandmarkRunParameter*>(p);
    assert(RunParam != nullptr);
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void LicenseLandmark::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

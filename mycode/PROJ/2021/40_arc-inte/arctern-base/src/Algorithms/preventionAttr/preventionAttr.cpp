

#include "./preventionAttr.h"

#ifdef ATLAS_ALLOWED
#include "src/Algorithms/preventionAttr/atlas/atlasPreventionAttrImpl.h"
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#endif

using arctern::PreventionAttr;
using arctern::PreventionAttrRunParameter;
using arctern::PreventionAttrResult;
using arctern::ErrorType;
using arctern::Result;

PreventionAttrRunParameter::PreventionAttrRunParameter() {
}

PreventionAttrRunParameter::~PreventionAttrRunParameter() noexcept {
}

PreventionAttrResult::PreventionAttrResult() {
}

PreventionAttrResult::~PreventionAttrResult() {
}

PreventionAttr::PreventionAttr() : impl_(nullptr) {
}

PreventionAttr::~PreventionAttr() noexcept {
  PreventionAttr::Release();
}

ErrorType PreventionAttr::Init(const InitParameter *p) {
  Release();

#ifdef ATLAS_ALLOWED
impl_ = new AtlasPreventionAttrImpl;
auto ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*) impl_,p);
#endif

  if(ErrorType::ERR_SUCCESS != ret) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

ErrorType PreventionAttr::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    return impl_->Process(p,r);
  }

  return ERR_UNEXPECT;
}

void PreventionAttr::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

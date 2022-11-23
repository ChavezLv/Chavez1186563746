/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.14
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "messyMaterials.h"
#include "../src/log/logger.h"

#ifdef ATLAS_ALLOWED
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "atlas/atlasMessyMaterialsImpl.h"
#endif

using namespace arctern;

MessyMaterialsRunParameter::MessyMaterialsRunParameter() {

}

MessyMaterialsRunParameter::~MessyMaterialsRunParameter() {

}

ErrorType MessyMaterialsRunParameter::CheckSize() const {
  return ERR_SUCCESS;
}

MessyMaterialsResult::MessyMaterialsResult() {
}

MessyMaterialsResult::~MessyMaterialsResult() noexcept {
}

MessyMaterials::MessyMaterials() : impl_(nullptr) {
}

MessyMaterials::~MessyMaterials() noexcept {
  MessyMaterials::Release();
}

ErrorType MessyMaterials::Init(const InitParameter *p) {
  /// first delete the object, then new it
  Release();

  ErrorType ret = ErrorType::ERR_SUCCESS;

#ifdef ATLAS_ALLOWED
  impl_ = new arctern::AtlasMessyMaterialsImpl;
  ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

void MessyMaterials::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

ErrorType MessyMaterials::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    const auto rParam = dynamic_cast<const MessyMaterialsRunParameter*>(p);
    assert(rParam != nullptr);
    return impl_->Process(p,r);
  }
  return ERR_UNEXPECT;
}



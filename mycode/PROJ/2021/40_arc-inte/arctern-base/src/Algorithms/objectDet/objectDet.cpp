/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.14
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "objectDet.h"
#include "../src/log/logger.h"

#ifdef ATLAS_ALLOWED
#include "src/Algorithms/AtlasInterfaceCommonOperation.h"
#include "atlas/atlasObjectDetImpl.h"
#endif

using namespace arctern;

ObjectDetRunParameter::ObjectDetRunParameter() {

}

ObjectDetRunParameter::~ObjectDetRunParameter() {

}

ErrorType ObjectDetRunParameter::CheckSize() const {
  return ERR_SUCCESS;
}

ObjectDetResult::ObjectDetResult() {
}

ObjectDetResult::~ObjectDetResult() noexcept {
}

ObjectDet::ObjectDet() : impl_(nullptr) {
}

ObjectDet::~ObjectDet() noexcept {
  ObjectDet::Release();
}

ErrorType ObjectDet::Init(const InitParameter *p) {
  /// first delete the object, then new it
  Release();

  ErrorType ret = ErrorType::ERR_SUCCESS;

#ifdef ATLAS_ALLOWED
  impl_ = new arctern::AtlasObjectDetImpl;
  ret = arctern::AtlasInterfaceCommonOperation::Init((ArcternAlgorithmEx*)impl_,p);
#endif

  if(ret != ErrorType::ERR_SUCCESS) {
    delete impl_;
    impl_ = nullptr;
  }

  return ret;
}

void ObjectDet::Release() {
  if (nullptr != impl_) {
    impl_->Release();
    delete impl_;
    impl_ = nullptr;
  }
}

ErrorType ObjectDet::Process(const RunParameter *p,Result *r) {
  if (nullptr != impl_) {
    const auto faceDetectTinyRunParam = dynamic_cast<const ObjectDetRunParameter*>(p);
    assert(faceDetectTinyRunParam != nullptr);
    return impl_->Process(p,r);
  }
  return ERR_UNEXPECT;
}



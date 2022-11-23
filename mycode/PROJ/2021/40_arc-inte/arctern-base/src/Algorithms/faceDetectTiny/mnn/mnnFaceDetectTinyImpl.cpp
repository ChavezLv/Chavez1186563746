//
// Created by gh on 2021/8/26.
//

#include "mnnFaceDetectTinyImpl.h"

namespace arctern {

MnnFaceDetectTinyImpl::MnnFaceDetectTinyImpl() {
  netShape_ = NetworkShape::NHWC;
}

int MnnFaceDetectTinyImpl::GetModelHeight(){
  return input_h_;
}
int MnnFaceDetectTinyImpl::GetModelWidth(){
  return input_w_;
}
}
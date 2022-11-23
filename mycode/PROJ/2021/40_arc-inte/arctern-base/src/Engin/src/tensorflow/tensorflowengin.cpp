/****************************************************************************
 *  Filename:       mxnetengin.cpp
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.02.21
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/
#include "./src/log/logger.h"
#include "./src/Engin/src/initenginparameter.h"
#include "./tensorflowengin.h"
using arctern::TensorflowEngin;
using arctern::InitEnginParameter;
using arctern::ErrorType;
using arctern::Result;



TensorflowEngin::TensorflowEngin() {
    return;
}

TensorflowEngin::~TensorflowEngin() {
    TensorflowEngin::Release();
}

ErrorType TensorflowEngin::Init(const InitParameter *p) {
#ifdef TENSORFLOW_ALLOWED
     std::cout << "ok ,use TensorflowEngin!!" << std::endl;
#else
     SPDLOG_ERROR(" ERROR, TensorflowEngin ENGIN NOT ALLOWED!!!! %s");
     return ErrorType::ERR_BAD_ENGIN;
#endif

     return ErrorType::ERR_SUCCESS;
}

void TensorflowEngin::Release() {
    return;
}

const Result * TensorflowEngin::Process(const RunParameter *p) {
#ifdef  TENSORFLOW_ALLOWED
     std::cout << "ok ,use TensorflowEngin!!" << std::endl;
#else
     SPDLOG_ERROR(" ERROR, TensorflowEngin ENGIN NOT ALLOWED!!!! %s");
     result_.errorType_ = ErrorType::ERR_BAD_ENGIN;
     return static_cast<Result*>(&result_);
#endif
     result_.errorType_ = ErrorType::ERR_SUCCESS;
     return static_cast<Result*>(&result_);
}

ErrorType TensorflowEngin::SetParameter(ConfigParameter *p) {
     return ErrorType::ERR_SUCCESS;
}

const StatusParameter * TensorflowEngin::GetParameter(StatusParameter *p) {
     return nullptr;
}

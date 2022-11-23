/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.11.20
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

//#include "src/Engin/src/mxnet/MxnetEngineManager.h"
//#include "src/Engin/src/mxnet/mxnetengin.h"
#include "src/Base/arcternalgorithm.h"
#include "TvmInterfaceCommonOperation.h"
#include "src/Engin/src/tvm/tvmEngineManager.h"
#include "src/Engin/src/tvm/tvmengin.h"


using arctern::ErrorType;
using arctern::Result;
using arctern::ArcternAlgorithm;
using arctern::TvmInterfaceCommonOperation;

TvmInterfaceCommonOperation::~TvmInterfaceCommonOperation()  {
}

ErrorType TvmInterfaceCommonOperation::Init(ArcternAlgorithm **impl, const InitParameter *param) {
  auto *p = const_cast<InitParameter*>(param);
  auto alg = (ArcternAlgorithm*)*impl;

  auto ret = alg->CheckInitParam(p);
  if (ret != ErrorType::ERR_SUCCESS) return ret;
  arctern::TvmEngineManager engineManager;
  std::vector<std::shared_ptr<EnginBase>> engins;
   ret = engineManager.CreateEngine(p, engins);
  if (ret != ErrorType::ERR_SUCCESS) {
    return ret;
  }
  // warnning SetEngin must be before Init!!!
  ((ArcternAlgorithm *) *impl)->SetEngine(engins);
  // warnning getConfigInfo must be before Init!!!
  ((ArcternAlgorithm *) *impl)->GetConfigInfo(
      const_cast<MxnetModel *>(engineManager.mxnet_models_->models()->Get(0)));
  ret = (*impl)->Init(param);
  // warnning!!!! I don't know what happen if there are multiple models!!!
  engineManager.Release();
  return ret;
}

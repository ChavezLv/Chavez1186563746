/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.08.31
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/

#include "src/Engin/src/mxnet/MxnetEngineManager.h"
#include "src/Engin/src/mxnet/mxnetengin.h"
#include "src/Base/arcternalgorithm.h"
#include "./MxnetInterfaceCommonOperation.h"

using arctern::ErrorType;
using arctern::Result;
using arctern::ArcternAlgorithm;
using arctern::MxnetInterfaceCommonOperation;

ErrorType MxnetInterfaceCommonOperation::Init(ArcternAlgorithm** impl,const InitParameter *param) {
  auto *p = const_cast<InitParameter*>(param);
  auto alg = (ArcternAlgorithm*)*impl;

  auto ret = alg->CheckInitParam(p);
  if (ret != ErrorType::ERR_SUCCESS) return ret;

  if(p->gpuId_ < 0) {
      std::cout << "warning: you use cpu version,gpuId_ must be less than 0"<< std::endl;
  }
  p->gpuId_ = -1;

  arctern::MxnetEngineManager engineManager;
  std::vector<std::shared_ptr<EnginBase>> engines;
  ret = engineManager.CreateEngine(p, engines);
  if (ret != ErrorType::ERR_SUCCESS) {
    return ret;
  }
  ///> SetEngine must be before Init!!!
  alg->SetEngine(engines);

  ///> getConfigInfo must be before Init!!!
  alg->GetConfigInfo(const_cast<MxnetModel *>(engineManager.mxnet_models_->models()->Get(0)));
  alg->GetModelsConfigInfo(const_cast<MxnetModels *>(engineManager.mxnet_models_));

  ret = alg->Init(p);
  ///>  I don't know what happen if there are multiple models!!!
  engineManager.Release();
  return ret;
}

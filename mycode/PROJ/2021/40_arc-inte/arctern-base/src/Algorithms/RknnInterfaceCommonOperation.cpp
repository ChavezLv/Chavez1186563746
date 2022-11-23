/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.11.26
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/
#include "RknnInterfaceCommonOperation.h"
#include "src/Base/arcternalgorithm.h"
#include "src/Engin/src/rknn/rknnengin.h"
#include "src/Engin/src/rknn/RknnEngineManager.h"

using arctern::ErrorType;
using arctern::ArcternAlgorithm;
using arctern::RknnInterfaceCommonOperation;

RknnInterfaceCommonOperation::~RknnInterfaceCommonOperation() noexcept
{
}

ErrorType RknnInterfaceCommonOperation::Init(
        ArcternAlgorithm **impl,const InitParameter *p)
{
  auto *param = const_cast<InitParameter*>(p);
  auto alg = (ArcternAlgorithm*)*impl;

  auto ret = alg->CheckInitParam(param);
  if (ret != ErrorType::ERR_SUCCESS)  {
    return ret;
  }

  arctern::RknnEngineManager engineManager;
  std::vector<std::shared_ptr<EnginBase>> engins;
  ret = engineManager.CreateEngine(param, engins);
  if (ret != ErrorType::ERR_SUCCESS) {
      return ret;
  }

    // warnning SetEngin must be before Init!!!
  ((ArcternAlgorithm *) *impl)->SetEngine(engins);

    // warnning getConfigInfo must be before Init!!!
    // FIXME: rknn model is not a flatbuffers bin for the moment
  ((ArcternAlgorithm *) *impl)->GetConfigInfo(
      const_cast<MxnetModel *>(engineManager.mxnet_models_->models()->Get(0)));
    // std::cout << "get config Info end" << std::endl;

    ret = (*impl)->Init(p);
    // std::cout << " impl init end" << std::endl;

    // warnning!!!! I don't know what happen if there are multiple models!!!
    engineManager.Release();

    return ret;
}

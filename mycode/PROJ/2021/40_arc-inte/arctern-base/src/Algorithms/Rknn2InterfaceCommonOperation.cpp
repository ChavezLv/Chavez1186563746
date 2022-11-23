/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.11.26
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/
#include "Rknn2InterfaceCommonOperation.h"
#include "src/Base/arcternalgorithm.h"
#include "src/Engin/src/rknn2/rknnengin.h"
#include "src/Engin/src/rknn2/RknnEngineManager.h"

using arctern::ErrorType;
using arctern::ArcternAlgorithm;
using arctern::Rknn2InterfaceCommonOperation;

Rknn2InterfaceCommonOperation::~Rknn2InterfaceCommonOperation() noexcept {
}

ErrorType Rknn2InterfaceCommonOperation::Init(
    ArcternAlgorithm **impl, const InitParameter *param) {
  auto *p = const_cast<InitParameter *>(param);
  auto alg = (ArcternAlgorithm *) *impl;

  auto ret = alg->CheckInitParam(p);
  if (ret != ErrorType::ERR_SUCCESS) return ret;

  arctern::RknnEngineManager engineManager;
  std::vector<std::shared_ptr<EnginBase>> engins;
  ret = engineManager.CreateEngine(p, engins);
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

  ret = (*impl)->Init(param);
  // std::cout << " impl init end" << std::endl;

  // warnning!!!! I don't know what happen if there are multiple models!!!
  engineManager.Release();

  return ret;
}

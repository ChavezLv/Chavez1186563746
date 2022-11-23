/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.23
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include "HisiInterfaceCommonOperation.h"
#include "src/Base/arcternalgorithm.h"
#include "src/Engin/src/hisi/HisiEngineManager.h"
#include "src/Engin/src/hisi/hisiengin.h"

using arctern::ErrorType;
using arctern::Result;
using arctern::ArcternAlgorithm;
using arctern::HisiInterfaceCommonOperation;

HisiInterfaceCommonOperation::~HisiInterfaceCommonOperation() noexcept {
}

ErrorType HisiInterfaceCommonOperation::Init(ArcternAlgorithm **impl, InitParameter *p) {
  ErrorType checkParamret = ((ArcternAlgorithm *) *impl)->checkInitParam(p);
  if (checkParamret != ErrorType::ERR_SUCCESS) return checkParamret;
  arctern::HisiEngineManager engineManager;
//  EnginBase *engin = nullptr;
//  size_t enginSize = 0;
  std::vector<std::shared_ptr<EnginBase>> engins;
  ErrorType ret = engineManager.CreateEngine(p, engins);
  if (ret != ErrorType::ERR_SUCCESS) {
    return ret;
  }
  // warnning SetEngin must be before Init!!!
  ((ArcternAlgorithm *) *impl)->SetEngine(engins);
  // warnning getConfigInfo must be before Init!!!
  ((ArcternAlgorithm *) *impl)->GetConfigInfo(
      const_cast<MxnetModel *>(engineManager.mxnet_models_->models()->Get(0)));

 // std::cout << "get config Info end" << std::endl;
  ret = (*impl)->Init(p);
 // std::cout << " impl init end" << std::endl;
  // warnning!!!! I don't know what happen if there are multiple models!!!
  engineManager.Release();
  return ret;
}

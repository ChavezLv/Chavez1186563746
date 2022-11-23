//
// Created by gh on 2021/8/25.
//

#include "src/Base/arcternalgorithm.h"
#include "MnnInterfaceCommonOperation.h"
#include "Engin/src/mnn/mnnEngineManager.h"

using arctern::ErrorType;
using arctern::Result;
using arctern::ArcternAlgorithm;
using arctern::MnnInterfaceCommonOperation;

ErrorType MnnInterfaceCommonOperation::Init(ArcternAlgorithm* impl,const InitParameter *param) {
  std::cout<<__FILE__ <<" func "<<__FUNCTION__ <<"  "<<__LINE__<<std::endl;
  auto *p = const_cast<InitParameter*>(param);
  auto ret = impl->CheckInitParam(p);
  if (ret != ErrorType::ERR_SUCCESS) return ret;
  std::cout<<__FILE__ <<" func "<<__FUNCTION__ <<"  "<<__LINE__<<std::endl;
  if(p->gpuId_ < 0) {
    std::cout << "warning: you use cpu version,gpuId_ must be less than 0"<< std::endl;
  }
  p->gpuId_ = -1;

  std::cout<<__FILE__ <<" func "<<__FUNCTION__ <<"  "<<__LINE__<<std::endl;
  arctern::MnnEngineManager engineManager;
  engineManager.parseMxnetModels(p->model_path);
  impl->GetConfigInfo(const_cast<MxnetModel *>(engineManager.mxnet_models_->models()->Get(0)));
  engineManager.SetHW(impl->GetModelHeight(),impl->GetModelWidth());

  std::vector<std::shared_ptr<EnginBase>> engines;
  ret = engineManager.CreateEngine(p, engines);
  if (ret != ErrorType::ERR_SUCCESS) {
    return ret;
  }

  std::cout<<__FILE__ <<" func "<<__FUNCTION__ <<"  "<<__LINE__<<std::endl;
  ///> SetEngine must be before Init!!!
  impl->SetEngine(engines);

  std::cout<<__FILE__ <<" func "<<__FUNCTION__ <<"  "<<__LINE__<<std::endl;
  ret = impl->Init(p);
  std::cout<<__FILE__ <<" func "<<__FUNCTION__ <<"  "<<__LINE__<<std::endl;
  ///>  I don't know what happen if there are multiple models!!!
  engineManager.Release();
  return ret;
}

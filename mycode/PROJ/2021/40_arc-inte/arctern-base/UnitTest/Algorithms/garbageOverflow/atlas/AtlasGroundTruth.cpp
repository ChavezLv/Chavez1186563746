//
// Created by Admin on 2019/6/11.
//
#include "AtlasGroundTruth.h"

AtlasGarbageOverflowUnittest::AtlasGarbageOverflowUnittest(const AtlasTestParam & parameter):
GarbageOverflowUnittest(parameter.ymlGroundTruth,parameter.imgPath,parameter.modelPath,parameter.result),
m_gpuId(parameter.gpuIndex),m_modelsPath8(parameter.modelPath8){

}

std::shared_ptr<arctern::AlgorithmInterface> AtlasGarbageOverflowUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::GarbageOverflow> ptr(new arctern::GarbageOverflow);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.thresh_ = 0.5;
  if(initParam.batchNum_ == 8){
    initParam.model_path = m_modelsPath8;
  }
  else{
    initParam.model_path = modelPath_;
  }
  
  initParam.gpuId_ = m_gpuId;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

AtlasGarbageOverflowUnittest::~AtlasGarbageOverflowUnittest() {
}


void AtlasGarbageOverflowUnittest::checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                                                std::shared_ptr<BaseGroundTruthData> gtData,
                                                int startId,
                                                int len) {
  BuildFunc build = std::bind(&AtlasGarbageOverflowUnittest::buildRunParameter,this,gtData,startId,len);
  CheckFunc check = std::bind(&AtlasGarbageOverflowUnittest::checkResult, this,gtData,startId,len,std::placeholders::_1);

  CheckPictures(alg,gtData,startId,len,imgDir_,build,check,result_);
}

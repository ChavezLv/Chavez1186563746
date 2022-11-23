//
// Created by Admin on 2019/6/15.
//

#include "TrtGroundTruth.h"
#include "cuda.h"
#include "cuda_runtime.h"
#include "UnitTest/UnitTestBase/trt/TrtUtils.h"
TrtPersonFeatUnittest::TrtPersonFeatUnittest(const TrtTestParam & parameter):
    PersonFeatUnittest(parameter.ymlGroundTruth,parameter.ymlInput,parameter.imgPath,parameter.modelPath,parameter.result),
    m_gpuId(parameter.gpuIndex){

}

std::shared_ptr<arctern::AlgorithmInterface> TrtPersonFeatUnittest::createAlgHandle(int batch) {


  std::shared_ptr<arctern::PersonFeat> ptr(new arctern::PersonFeat);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  initParam.gpuId_ = m_gpuId;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

TrtPersonFeatUnittest::~TrtPersonFeatUnittest() {
}


void TrtPersonFeatUnittest::checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                                                std::shared_ptr<BaseGroundTruthData> gtData,
                                                int startId,
                                                int len) {
  BuildFunc build = std::bind(&TrtPersonFeatUnittest::buildRunParameter,this,gtData,startId,len);
  CheckFunc check = std::bind(&TrtPersonFeatUnittest::checkResult, this,gtData,startId,len,std::placeholders::_1);

  CheckPictures(alg,gtData,startId,len,imgDir_,build,check,result_);
}

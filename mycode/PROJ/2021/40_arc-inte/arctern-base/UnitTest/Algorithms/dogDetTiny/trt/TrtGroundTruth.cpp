//
// Created by Admin on 2019/6/16.
//

#include "TrtGroundTruth.h"
#include "cuda.h"
#include "cuda_runtime.h"
#include "UnitTest/UnitTestBase/trt/TrtUtils.h"
TrtDogDetTinyUnittest::TrtDogDetTinyUnittest(const TrtTestParam & parameter):
    dogDetTinyUnittest(parameter.ymlGroundTruth,parameter.imgPath,parameter.modelPath,parameter.result),
    m_gpuId(parameter.gpuIndex){

}

std::shared_ptr<arctern::AlgorithmInterface> TrtDogDetTinyUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::dogDetTiny> ptr(new arctern::dogDetTiny);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;
  initParam.thresh_ = 0.4;

  initParam.gpuId_ = m_gpuId;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

TrtDogDetTinyUnittest::~TrtDogDetTinyUnittest() {
}


void TrtDogDetTinyUnittest::checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                                                std::shared_ptr<BaseGroundTruthData> gtData,
                                                int startId,
                                                int len) {
  BuildFunc build = std::bind(&TrtDogDetTinyUnittest::buildRunParameter,this,gtData,startId,len);
  CheckFunc check = std::bind(&TrtDogDetTinyUnittest::checkResult, this,gtData,startId,len,std::placeholders::_1);

  CheckPictures(alg,gtData,startId,len,imgDir_,build,check,result_);
}

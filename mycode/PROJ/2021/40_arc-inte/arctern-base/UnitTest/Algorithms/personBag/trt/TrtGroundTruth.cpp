//
// Created by Admin on 2021/6/18.
//

#include <UnitTest/UnitTestBase/trt/TrtUtils.h>
#include "TrtGroundTruth.h"
#include "cuda.h"
#include "cuda_runtime.h"

TrtPersonBagUnittest::TrtPersonBagUnittest(const TrtTestParam & parameter):
    personBagUnittest(parameter.ymlGroundTruth,parameter.ymlInput,parameter.imgPath,parameter.modelPath,parameter.result),
    m_gpuId(parameter.gpuIndex){

}

std::shared_ptr<arctern::AlgorithmInterface> TrtPersonBagUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::PersonBag> ptr(new arctern::PersonBag);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  initParam.gpuId_ = m_gpuId;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

TrtPersonBagUnittest::~TrtPersonBagUnittest() {
}


void TrtPersonBagUnittest::checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                                                std::shared_ptr<BaseGroundTruthData> gtData,
                                                int startId,
                                                int len) {
  BuildFunc build = std::bind(&TrtPersonBagUnittest::buildRunParameter,this,gtData,startId,len);
  CheckFunc check = std::bind(&TrtPersonBagUnittest::checkResult, this,gtData,startId,len,std::placeholders::_1);

  CheckPictures(alg,gtData,startId,len,imgDir_,build,check,result_);
}

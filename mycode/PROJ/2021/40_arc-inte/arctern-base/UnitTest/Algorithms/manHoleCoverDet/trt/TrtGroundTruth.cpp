//
// Created by Admin on 2021/12/20.
//

#include "TrtGroundTruth.h"
#include <UnitTest/UnitTestBase/trt/TrtUtils.h>
/* #include "cuda.h" */
/* #include "cuda_runtime.h" */

TrtManholeCoverDetUnittest::TrtManholeCoverDetUnittest(const TrtTestParam & parameter):
    ManHoleCoverDetUnittest(parameter.ymlGroundTruth,parameter.imgPath,parameter.modelPath,parameter.result),
    m_gpuId(parameter.gpuIndex){

}

std::shared_ptr<arctern::AlgorithmInterface> TrtManholeCoverDetUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::ManHoleCoverDet> ptr(new arctern::ManHoleCoverDet);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;
  initParam.thresh_ = 0.1;
  initParam.gpuId_ = m_gpuId;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

TrtManholeCoverDetUnittest::~TrtManholeCoverDetUnittest() {
}


void TrtManholeCoverDetUnittest::checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                                                std::shared_ptr<BaseGroundTruthData> gtData,
                                                int startId,
                                                int len) {
  BuildFunc build = std::bind(&TrtManholeCoverDetUnittest::buildRunParameter,this,gtData,startId,len);
  CheckFunc check = std::bind(&TrtManholeCoverDetUnittest::checkResult, this,gtData,startId,len,std::placeholders::_1);

  CheckPictures(alg,gtData,startId,len,imgDir_,build,check,result_);
}

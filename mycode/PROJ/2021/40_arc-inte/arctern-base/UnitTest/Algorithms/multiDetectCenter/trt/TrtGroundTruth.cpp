//
// Created by Admin on 2021/6/18.
//

#include <UnitTest/UnitTestBase/trt/TrtUtils.h>
#include "TrtGroundTruth.h"
#include "cuda.h"
#include "cuda_runtime.h"

TrtMultiDetectCenterUnittest::TrtMultiDetectCenterUnittest(const TrtTestParam & parameter):
    MultiDetectCenterUnittest(parameter.ymlGroundTruth,parameter.imgPath,parameter.modelPath,parameter.result),
    m_gpuId(parameter.gpuIndex){

}

std::shared_ptr<arctern::AlgorithmInterface> TrtMultiDetectCenterUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::MultiDetectCenter> ptr(new arctern::MultiDetectCenter);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;
  initParam.thresh_ = 0.3;

  initParam.gpuId_ = m_gpuId;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

TrtMultiDetectCenterUnittest::~TrtMultiDetectCenterUnittest() {
}


void TrtMultiDetectCenterUnittest::checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                                                std::shared_ptr<BaseGroundTruthData> gtData,
                                                int startId,
                                                int len) {
  BuildFunc build = std::bind(&TrtMultiDetectCenterUnittest::buildRunParameter,this,gtData,startId,len);
  CheckFunc check = std::bind(&TrtMultiDetectCenterUnittest::checkResult, this,gtData,startId,len,std::placeholders::_1);

  CheckPictures(alg,gtData,startId,len,imgDir_,build,check,result_);
}

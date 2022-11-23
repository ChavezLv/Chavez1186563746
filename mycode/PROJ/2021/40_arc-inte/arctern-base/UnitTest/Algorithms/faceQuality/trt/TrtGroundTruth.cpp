//
// Created by Admin on 2019/6/11.
//
#include <UnitTest/UnitTestBase/trt/TrtUtils.h>
#include "TrtGroundTruth.h"
#include "cuda.h"
#include "cuda_runtime.h"

TrtFaceQualityUnittest::TrtFaceQualityUnittest(const TrtTestParam & parameter):
    FaceQualityUnittest(parameter.ymlGroundTruth,parameter.ymlInput,parameter.imgPath,parameter.modelPath,parameter.result),
m_gpuId(parameter.gpuIndex){

}

std::shared_ptr<arctern::AlgorithmInterface> TrtFaceQualityUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceQuality> ptr(new arctern::FaceQuality);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  initParam.gpuId_ = m_gpuId;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

TrtFaceQualityUnittest::~TrtFaceQualityUnittest() {
}


void TrtFaceQualityUnittest::checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                                                std::shared_ptr<BaseGroundTruthData> gtData,
                                                int startId,
                                                int len) {
  BuildFunc build = std::bind(&TrtFaceQualityUnittest::buildRunParameter,this,gtData,startId,len);
  CheckFunc check = std::bind(&TrtFaceQualityUnittest::checkResult, this,gtData,startId,len,std::placeholders::_1);

  CheckPictures(alg,gtData,startId,len,imgDir_,build,check,result_);
}

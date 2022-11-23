//
// Created by Admin on 2019/6/16.
//

#include <UnitTest/UnitTestBase/trt/TrtUtils.h>
#include "TrtGroundTruth.h"
#include "cuda.h"
#include "cuda_runtime.h"

TrtFaceDetectRTNUnittest::TrtFaceDetectRTNUnittest(const TrtTestParam & parameter):
    FaceDetectRTNUnittest(parameter.ymlGroundTruth,parameter.imgPath,parameter.modelPath,parameter.result),
    m_gpuId(parameter.gpuIndex){

}

std::shared_ptr<arctern::AlgorithmInterface> TrtFaceDetectRTNUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceDetectRTN> ptr(new arctern::FaceDetectRTN);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;
  initParam.thresh_ = 0.6;
  initParam.gpuId_ = m_gpuId;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

TrtFaceDetectRTNUnittest::~TrtFaceDetectRTNUnittest() {
}


void TrtFaceDetectRTNUnittest::checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                                              std::shared_ptr<BaseGroundTruthData> gtData,
                                              int startId,
                                              int len) {
  BuildFunc build = std::bind(&TrtFaceDetectRTNUnittest::buildRunParameter,this,gtData,startId,len);
  CheckFunc check = std::bind(&TrtFaceDetectRTNUnittest::checkResult, this,gtData,startId,len,std::placeholders::_1);

  CheckPictures(alg,gtData,startId,len,imgDir_,build,check,result_);
}
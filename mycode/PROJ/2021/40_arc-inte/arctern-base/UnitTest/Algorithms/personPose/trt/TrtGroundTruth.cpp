//
// Created by chengaohua on 2021/7/7.
//

#include "TrtGroundTruth.h"
#include "cuda.h"
#include "cuda_runtime.h"
#include "UnitTest/UnitTestBase/trt/TrtUtils.h"
TrtPersonPoseUnittest::TrtPersonPoseUnittest(const TrtTestParam & parameter):
    personPoseUnittest(parameter.ymlGroundTruth,parameter.ymlInput,parameter.imgPath,parameter.modelPath,parameter.result),
    m_gpuId(parameter.gpuIndex){

}

std::shared_ptr<arctern::AlgorithmInterface> TrtPersonPoseUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::PersonPose> ptr(new arctern::PersonPose);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  initParam.gpuId_ = m_gpuId;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

TrtPersonPoseUnittest::~TrtPersonPoseUnittest() {
}


void TrtPersonPoseUnittest::checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                                         std::shared_ptr<BaseGroundTruthData> gtData,
                                         int startId,
                                         int len) {
  BuildFunc build = std::bind(&TrtPersonPoseUnittest::buildRunParameter,this,gtData,startId,len);
  CheckFunc check = std::bind(&TrtPersonPoseUnittest::checkResult, this,gtData,startId,len,std::placeholders::_1);

  CheckPictures(alg,gtData,startId,len,imgDir_,build,check,result_);
}

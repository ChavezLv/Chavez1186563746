//
// Created by Admin on 2019/6/11.
//
#include "TrtGroundTruth.h"
#include "UnitTest/UnitTestBase/trt/TrtUtils.h"

TrtFaceAbnomalLightUnittest::TrtFaceAbnomalLightUnittest(const TrtTestParam & parameter):
FaceAbnomalLightUnittest(parameter.ymlGroundTruth,parameter.ymlInput,parameter.imgPath,parameter.modelPath,
                         parameter.result),
m_gpuId(parameter.gpuIndex){

}

std::shared_ptr<arctern::AlgorithmInterface> TrtFaceAbnomalLightUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceAbnomalLight> ptr(new arctern::FaceAbnomalLight);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  initParam.gpuId_ = m_gpuId;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

TrtFaceAbnomalLightUnittest::~TrtFaceAbnomalLightUnittest() {
}


void TrtFaceAbnomalLightUnittest::checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                                                std::shared_ptr<BaseGroundTruthData> gtData,
                                                int startId,
                                                int len) {
  BuildFunc build = std::bind(&TrtFaceAbnomalLightUnittest::buildRunParameter,this,gtData,startId,len);
  CheckFunc check = std::bind(&TrtFaceAbnomalLightUnittest::checkResult, this,gtData,startId,len,std::placeholders::_1);
  CheckPictures(alg,gtData,startId,len,imgDir_,build,check,result_);
}

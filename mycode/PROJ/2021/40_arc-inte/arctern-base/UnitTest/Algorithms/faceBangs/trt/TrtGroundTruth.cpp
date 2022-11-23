//
// Created by Admin on 2021/6/21.
//

#include "TrtGroundTruth.h"
#include "UnitTest/UnitTestBase/trt/TrtUtils.h"
TrtFaceBangsUnittest::TrtFaceBangsUnittest(const TrtTestParam & parameter):
    FaceBangsUnittest(parameter.ymlGroundTruth,parameter.ymlInput,parameter.imgPath,parameter.modelPath,parameter.result),
    m_gpuId(parameter.gpuIndex){

}

std::shared_ptr<arctern::AlgorithmInterface> TrtFaceBangsUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceBangs> ptr(new arctern::FaceBangs);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  initParam.gpuId_ = m_gpuId;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

TrtFaceBangsUnittest::~TrtFaceBangsUnittest() {
}


void TrtFaceBangsUnittest::checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                                                std::shared_ptr<BaseGroundTruthData> gtData,
                                                int startId,
                                                int len) {
  BuildFunc build = std::bind(&TrtFaceBangsUnittest::buildRunParameter,this,gtData,startId,len);
  CheckFunc check = std::bind(&TrtFaceBangsUnittest::checkResult, this,gtData,startId,len,std::placeholders::_1);

  CheckPictures(alg,gtData,startId,len,imgDir_,build,check,result_);
}
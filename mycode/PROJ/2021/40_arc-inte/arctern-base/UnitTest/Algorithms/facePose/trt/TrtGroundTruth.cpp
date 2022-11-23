//
// Created by Admin on 2021/6/18.
//

#include "TrtGroundTruth.h"
#include "cuda.h"
#include "cuda_runtime.h"
#include "UnitTest/UnitTestBase/trt/TrtUtils.h"
TrtFacePoseUnittest::TrtFacePoseUnittest(const TrtTestParam & parameter):
    FacePoseUnittest(parameter.ymlGroundTruth,parameter.ymlInput,parameter.imgPath,parameter.modelPath,parameter.result),
    m_gpuId(parameter.gpuIndex){
}

std::shared_ptr<arctern::AlgorithmInterface> TrtFacePoseUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FacePose> ptr(new arctern::FacePose);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  initParam.gpuId_ = m_gpuId;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

TrtFacePoseUnittest::~TrtFacePoseUnittest() {
}


void TrtFacePoseUnittest::checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                                                std::shared_ptr<BaseGroundTruthData> gtData,
                                                int startId,
                                                int len) {
  BuildFunc build = std::bind(&TrtFacePoseUnittest::buildRunParameter,this,gtData,startId,len);
  CheckFunc check = std::bind(&TrtFacePoseUnittest::checkResult, this,gtData,startId,len,std::placeholders::_1);

  CheckPictures(alg,gtData,startId,len,imgDir_,build,check,result_);
}

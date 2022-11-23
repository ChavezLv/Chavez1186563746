//
// Created by Admin on 2019/6/11.
//
#include "TrtGroundTruth.h"
#include "cuda.h"
#include "cuda_runtime.h"
#include "UnitTest/UnitTestBase/trt/TrtUtils.h"

TrtFaceMaskUnittest::TrtFaceMaskUnittest(const TrtTestParam & parameter):
FaceMaskUnittest(parameter.ymlGroundTruth,parameter.ymlInput,parameter.imgPath,parameter.modelPath,parameter.result),
m_gpuId(parameter.gpuIndex){

}

std::shared_ptr<arctern::AlgorithmInterface> TrtFaceMaskUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceMask> ptr(new arctern::FaceMask);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  initParam.gpuId_ = m_gpuId;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

TrtFaceMaskUnittest::~TrtFaceMaskUnittest() {
}


void TrtFaceMaskUnittest::checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                                                std::shared_ptr<BaseGroundTruthData> gtData,
                                                int startId,
                                                int len) {
  BuildFunc build = std::bind(&TrtFaceMaskUnittest::buildRunParameter,this,gtData,startId,len);
  CheckFunc check = std::bind(&TrtFaceMaskUnittest::checkResult, this,gtData,startId,len,std::placeholders::_1);

  CheckPictures(alg,gtData,startId,len,imgDir_,build,check,result_);
}

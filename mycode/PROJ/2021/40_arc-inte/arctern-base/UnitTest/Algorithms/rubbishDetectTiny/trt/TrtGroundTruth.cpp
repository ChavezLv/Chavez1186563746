//
// Created by Admin on 2021/12/8.
//

#include "TrtGroundTruth.h"
#include "UnitTest/UnitTestBase/trt/TrtUtils.h" 
/* #include "cuda.h" */
/* #include "cuda_runtime.h" */

TrtRubbishDetectTinyUnittest::TrtRubbishDetectTinyUnittest(const TrtTestParam & parameter):
    RubbishDetectTinyUnittest(parameter.ymlGroundTruth, parameter.imgPath,parameter.modelPath, parameter.result),
    m_gpuId(parameter.gpuIndex){

}

std::shared_ptr<arctern::AlgorithmInterface> TrtRubbishDetectTinyUnittest::createAlgHandle(int batch) {
  shared_ptr<arctern::RubbishDetectTiny> ptr(new arctern::RubbishDetectTiny);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;
  initParam.thresh_ = 0.10;

  initParam.gpuId_ = m_gpuId;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

TrtRubbishDetectTinyUnittest::~TrtRubbishDetectTinyUnittest() {
}


void TrtRubbishDetectTinyUnittest::checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                                                std::shared_ptr<BaseGroundTruthData> gtData,
                                                int startId,
                                                int len) {
  BuildFunc build = std::bind(&TrtRubbishDetectTinyUnittest::buildRunParameter,this,gtData,startId,len);
  CheckFunc check = std::bind(&TrtRubbishDetectTinyUnittest::checkResult, this,gtData,startId,len,std::placeholders::_1);
  std::shared_ptr<RubbishDetectTinyData> data = std::dynamic_pointer_cast<RubbishDetectTinyData>(gtData);
  CheckPictures(alg, gtData, startId, len, imgDir_, build, check, result_);
}

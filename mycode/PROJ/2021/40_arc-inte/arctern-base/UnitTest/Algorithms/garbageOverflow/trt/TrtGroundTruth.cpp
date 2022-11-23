//
// Created by Admin on 2021/12/8.
//

#include "TrtGroundTruth.h"
#include "UnitTest/UnitTestBase/trt/TrtUtils.h" 
/* #include "cuda.h" */
/* #include "cuda_runtime.h" */

TrtGarbageOverflowUnittest::TrtGarbageOverflowUnittest(const TrtTestParam & parameter):
    GarbageOverflowUnittest(parameter.ymlGroundTruth, parameter.imgPath,parameter.modelPath, parameter.result),
    m_gpuId(parameter.gpuIndex){

}

std::shared_ptr<arctern::AlgorithmInterface> TrtGarbageOverflowUnittest::createAlgHandle(int batch) {
  shared_ptr<arctern::GarbageOverflow> ptr(new arctern::GarbageOverflow);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;
  initParam.thresh_ = 0.40;

  initParam.gpuId_ = m_gpuId;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

TrtGarbageOverflowUnittest::~TrtGarbageOverflowUnittest() {
}


void TrtGarbageOverflowUnittest::checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                                                std::shared_ptr<BaseGroundTruthData> gtData,
                                                int startId,
                                                int len) {
  BuildFunc build = std::bind(&TrtGarbageOverflowUnittest::buildRunParameter,this,gtData,startId,len);
  CheckFunc check = std::bind(&TrtGarbageOverflowUnittest::checkResult, this,gtData,startId,len,std::placeholders::_1);
  std::shared_ptr<GarbageOverflowData> data = std::dynamic_pointer_cast<GarbageOverflowData>(gtData);
  CheckPictures(alg, gtData, startId, len, imgDir_, build, check, result_);
}

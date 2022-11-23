//
// Created by Admin on 2021/12/21.
//

#include "TrtGroundTruth.h"
#include <UnitTest/UnitTestBase/trt/TrtUtils.h>

TrtCargoIllegalDetectUnittest::TrtCargoIllegalDetectUnittest(const TrtTestParam & parameter):
    CargoIllegalDetectUnittest(parameter.ymlGroundTruth,parameter.imgPath,parameter.modelPath,parameter.result),
    m_gpuId(parameter.gpuIndex){

}

std::shared_ptr<arctern::AlgorithmInterface> TrtCargoIllegalDetectUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::CargoIllegalDetect> ptr(new arctern::CargoIllegalDetect);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;
  initParam.thresh_ = 0.001;
  initParam.gpuId_ = m_gpuId;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

TrtCargoIllegalDetectUnittest::~TrtCargoIllegalDetectUnittest() {
}


void TrtCargoIllegalDetectUnittest::checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                                                std::shared_ptr<BaseGroundTruthData> gtData,
                                                int startId,
                                                int len) {
  BuildFunc build = std::bind(&TrtCargoIllegalDetectUnittest::buildRunParameter,this,gtData,startId,len);
  CheckFunc check = std::bind(&TrtCargoIllegalDetectUnittest::checkResult, this,gtData,startId,len,std::placeholders::_1);

  CheckPictures(alg,gtData,startId,len,imgDir_,build,check,result_);
}

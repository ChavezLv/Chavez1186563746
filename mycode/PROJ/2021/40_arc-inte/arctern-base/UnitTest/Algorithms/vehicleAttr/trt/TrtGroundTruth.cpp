//
// Created by Admin on 2021/6/21.
//

#include "TrtGroundTruth.h"
#include "cuda.h"
#include "cuda_runtime.h"
#include "UnitTest/UnitTestBase/trt/TrtUtils.h"
TrtVehicleAttrUnittest::TrtVehicleAttrUnittest(const TrtTestParam & parameter):
    VehicleAttrUnittest(parameter.ymlGroundTruth,parameter.ymlInput,parameter.imgPath,parameter.modelPath,parameter.result),
    m_gpuId(parameter.gpuIndex){

}

std::shared_ptr<arctern::AlgorithmInterface> TrtVehicleAttrUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::VehicleAttr> ptr(new arctern::VehicleAttr);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  initParam.gpuId_ = m_gpuId;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

TrtVehicleAttrUnittest::~TrtVehicleAttrUnittest() {
}


void TrtVehicleAttrUnittest::checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                                                std::shared_ptr<BaseGroundTruthData> gtData,
                                                int startId,
                                                int len) {
  BuildFunc build = std::bind(&TrtVehicleAttrUnittest::buildRunParameter,this,gtData,startId,len);
  CheckFunc check = std::bind(&TrtVehicleAttrUnittest::checkResult, this,gtData,startId,len,std::placeholders::_1);

  CheckPictures(alg,gtData,startId,len,imgDir_,build,check,result_);
}

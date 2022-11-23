#include "TrtGroundTruth.h"
#include "UnitTest/UnitTestBase/trt/TrtUtils.h"
TrtVehicleCycleFeatUnittest::TrtVehicleCycleFeatUnittest(const TrtVehicleAttrParam & parameter):
VehicleCycleFeatUnittest(parameter.ymlGroundTruth,parameter.ymlInput,parameter.imgPath,parameter.modelPath,parameter.result),
                                                m_gpuId(parameter.gpuIndex) {

}

std::shared_ptr<arctern::AlgorithmInterface> TrtVehicleCycleFeatUnittest::createAlgHandle(int batch){
    std::shared_ptr<arctern::VehicleCycleFeat> ptr(new arctern::VehicleCycleFeat);
    arctern::InitParameter initParam;
    initParam.batchNum_ = batch;
    initParam.model_path = modelPath_;

    initParam.gpuId_ = m_gpuId;

    auto ret = ptr->Init(&initParam);

    EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

    return ptr;
}

TrtVehicleCycleFeatUnittest::~TrtVehicleCycleFeatUnittest() {
}


void TrtVehicleCycleFeatUnittest::checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                                                std::shared_ptr<BaseGroundTruthData> gtData,
                                                int startId,
                                                int len) {
  BuildFunc build = std::bind(&TrtVehicleCycleFeatUnittest::buildRunParameter,this,gtData,startId,len);
  CheckFunc check = std::bind(&TrtVehicleCycleFeatUnittest::checkResult, this,gtData,startId,len,std::placeholders::_1);

  CheckPictures(alg,gtData,startId,len,imgDir_,build,check,result_);
}

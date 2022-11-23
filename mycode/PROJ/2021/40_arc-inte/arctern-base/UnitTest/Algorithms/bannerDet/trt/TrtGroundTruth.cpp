//
// Created by Admin on 2021/12/8.
//

#include "TrtGroundTruth.h"
#include "UnitTest/UnitTestBase/trt/TrtUtils.h" 
/* #include "cuda.h" */
/* #include "cuda_runtime.h" */

TrtBannerDetUnittest::TrtBannerDetUnittest(const TrtTestParam & parameter):
    BannerDetUnittest(parameter.ymlGroundTruth, parameter.imgPath,parameter.modelPath, parameter.result),
    m_gpuId(parameter.gpuIndex){

}

std::shared_ptr<arctern::AlgorithmInterface> TrtBannerDetUnittest::createAlgHandle(int batch) {
  shared_ptr<arctern::BannerDet> ptr(new arctern::BannerDet);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;
  initParam.thresh_ = 0.10;

  initParam.gpuId_ = m_gpuId;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

TrtBannerDetUnittest::~TrtBannerDetUnittest() {
}


void TrtBannerDetUnittest::checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                                                std::shared_ptr<BaseGroundTruthData> gtData,
                                                int startId,
                                                int len) {
  BuildFunc build = std::bind(&TrtBannerDetUnittest::buildRunParameter,this,gtData,startId,len);
  CheckFunc check = std::bind(&TrtBannerDetUnittest::checkResult, this,gtData,startId,len,std::placeholders::_1);
  std::shared_ptr<BannerDetData> data = std::dynamic_pointer_cast<BannerDetData>(gtData);
  CheckPictures(alg, gtData, startId, len, imgDir_, build, check, result_);
}

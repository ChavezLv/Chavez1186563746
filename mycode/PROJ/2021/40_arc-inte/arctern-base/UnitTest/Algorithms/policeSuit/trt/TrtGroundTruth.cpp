#include "TrtGroundTruth.h"
#include "UnitTest/UnitTestBase/trt/TrtUtils.h"
TrtPoliceSuitUnittest::TrtPoliceSuitUnittest(
    const TrtPoliceSuitTestParam &parameter)
    : PoliceSuitUnittest(parameter.ymlGroundTruth, parameter.ymlInput,
                         parameter.imgPath, parameter.modelPath,parameter.result),
      m_gpuId(parameter.gpuIndex) {}

std::shared_ptr<arctern::AlgorithmInterface>
TrtPoliceSuitUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::PoliceSuit> ptr(new arctern::PoliceSuit);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  initParam.gpuId_ = m_gpuId;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

TrtPoliceSuitUnittest::~TrtPoliceSuitUnittest() {}

void TrtPoliceSuitUnittest::checkPictures(
    std::shared_ptr<arctern::AlgorithmInterface> alg,
    std::shared_ptr<BaseGroundTruthData> gtData, int startId, int len) {
  BuildFunc build = std::bind(&TrtPoliceSuitUnittest::buildRunParameter,this,gtData,startId,len);
  CheckFunc check = std::bind(&TrtPoliceSuitUnittest::checkResult, this,gtData,startId,len,std::placeholders::_1);

  CheckPictures(alg,gtData,startId,len,imgDir_,build,check,result_);
}

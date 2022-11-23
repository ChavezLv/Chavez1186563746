#include "TrtGroundTruth.h"
#include "cuda.h"
#include "cuda_runtime.h"
#include "UnitTest/UnitTestBase/trt/TrtUtils.h"
TrtObjectQualityUnittest::TrtObjectQualityUnittest(
    const TrtObjQualityTestParam &parameter)
    : ObjectQualityUnittest(parameter.ymlGroundTruth, parameter.ymlInput,
                            parameter.imgPath, parameter.modelPath,parameter.result),
      m_gpuId(parameter.gpuIndex) {}

std::shared_ptr<arctern::AlgorithmInterface>
TrtObjectQualityUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::ObjectQuality> ptr(new arctern::ObjectQuality);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  initParam.gpuId_ = m_gpuId;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

TrtObjectQualityUnittest::~TrtObjectQualityUnittest() {}

void TrtObjectQualityUnittest::checkPictures(
    std::shared_ptr<arctern::AlgorithmInterface> alg,
    std::shared_ptr<BaseGroundTruthData> gtData, int startId, int len) {
  BuildFunc build = std::bind(&TrtObjectQualityUnittest::buildRunParameter,this,gtData,startId,len);
  CheckFunc check = std::bind(&TrtObjectQualityUnittest::checkResult, this,gtData,startId,len,std::placeholders::_1);

  CheckPictures(alg,gtData,startId,len,imgDir_,build,check,result_);
}

//
// Created by Admin on 2021/6/21.
//

#include "TrtGroundTruth.h"
#include "UnitTest/UnitTestBase/trt/TrtUtils.h"

TrtPersonCompletenessUnittest::TrtPersonCompletenessUnittest(const TrtTestParam & parameter):
    PersonCompletenessUnittest(parameter.ymlGroundTruth,parameter.ymlInput,parameter.imgPath,parameter.modelPath,parameter.result),
    m_gpuId(parameter.gpuIndex){

}

std::shared_ptr<arctern::AlgorithmInterface> TrtPersonCompletenessUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::PersonCompleteness> ptr(new arctern::PersonCompleteness);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  initParam.gpuId_ = m_gpuId;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

TrtPersonCompletenessUnittest::~TrtPersonCompletenessUnittest() {
}


void TrtPersonCompletenessUnittest::checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                                                std::shared_ptr<BaseGroundTruthData> gtData,
                                                int startId,
                                                int len) {
  BuildFunc build = std::bind(&TrtPersonCompletenessUnittest::buildRunParameter,this,gtData,startId,len);
  CheckFunc check = std::bind(&TrtPersonCompletenessUnittest::checkResult, this,gtData,startId,len,std::placeholders::_1);

  CheckPictures(alg,gtData,startId,len,imgDir_,build,check,result_);
}

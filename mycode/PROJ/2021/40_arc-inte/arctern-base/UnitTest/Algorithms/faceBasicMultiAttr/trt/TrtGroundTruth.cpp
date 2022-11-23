//
// Created by Admin on 2021/6/21.
//

#include "TrtGroundTruth.h"
#include "UnitTest/UnitTestBase/trt/TrtUtils.h"

TrtFaceBasicMultiAttrUnittest::TrtFaceBasicMultiAttrUnittest(const TrtTestParam & parameter):
    FaceBasicMultiAttrUnittest(parameter.ymlGroundTruth,parameter.ymlInput,parameter.imgPath,parameter.modelPath,parameter.result),
    m_gpuId(parameter.gpuIndex){

}

std::shared_ptr<arctern::AlgorithmInterface> TrtFaceBasicMultiAttrUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceBasicMultiAttr> ptr(new arctern::FaceBasicMultiAttr);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  initParam.gpuId_ = m_gpuId;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

TrtFaceBasicMultiAttrUnittest::~TrtFaceBasicMultiAttrUnittest() {
}


void TrtFaceBasicMultiAttrUnittest::checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                                                std::shared_ptr<BaseGroundTruthData> gtData,
                                                int startId,
                                                int len) {
  BuildFunc build = std::bind(&TrtFaceBasicMultiAttrUnittest::buildRunParameter,this,gtData,startId,len);
  CheckFunc check = std::bind(&TrtFaceBasicMultiAttrUnittest::checkResult, this,gtData,startId,len,std::placeholders::_1);

  CheckPictures(alg,gtData,startId,len,imgDir_,build,check,result_);
}

//
// Created by Admin on 2021/6/18.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)


TEST(FaceHeadwear, TRT001) {

  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-headwearcolor-helmet-f32-d-0.0.1-rc.yml";
  parameter.ymlInput = getDir() + "input_params/face-headwearcolor-helmet-f32-d-0.0.1-i.yml";
  parameter.imgPath = getDir() + "data/predict_face_headwearcolor_helmet/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-headwearcolor-helmet-f32-d-0.0.1.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceHeadwearResult result;
  parameter.result=&result;
  TrtFaceHeadwearUnittest unittest(parameter);
  unittest.test();
}


#endif
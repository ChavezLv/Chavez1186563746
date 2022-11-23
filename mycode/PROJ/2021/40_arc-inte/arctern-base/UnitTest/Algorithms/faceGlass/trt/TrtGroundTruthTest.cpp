//
// Created by Admin on 2021/6/18.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)


TEST(FaceGlass, TRT103) {

  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-glass-1.0.3-rc.yml";
  parameter.ymlInput = getDir() + "input_params/face-glass-1.0.3-i.yml";
  parameter.imgPath = getDir() + "data/predict_face_glass/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-glass-1.0.3.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceGlassResult result;
  parameter.result=&result;
  TrtFaceGlassUnittest unittest(parameter);
  unittest.test();
}


#endif
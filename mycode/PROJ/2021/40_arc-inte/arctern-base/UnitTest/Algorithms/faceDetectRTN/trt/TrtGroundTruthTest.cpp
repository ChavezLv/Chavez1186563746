//
// Created by Admin on 2019/6/16.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)
TEST(FaceDetectRTN, TRT_102) {

  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-det-retinaface-d-1.0.2-rc.yml";
  parameter.ymlInput = "";
  parameter.imgPath = getDir() + "data/detect_retinaface/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-det-retinaface-d-1.0.2.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceDetectRTNResult result;
  parameter.result=&result;
  TrtFaceDetectRTNUnittest unittest(parameter);
  unittest.test();
}

#endif
//
// Created by Admin on 2021/6/18.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)


TEST(FacePose, TRT100) {
  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-pose-cls-d-1.0.0-rc.yml";
  parameter.ymlInput = getDir() + "input_params/face-pose-cls-d-1.0.0-i.yml";
  parameter.imgPath = getDir() + "data/predict_face_pose_cls/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-pose-cls-d-1.0.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FacePoseResult result;
  parameter.result=&result;
  TrtFacePoseUnittest unittest(parameter);
  unittest.test();
}


#endif
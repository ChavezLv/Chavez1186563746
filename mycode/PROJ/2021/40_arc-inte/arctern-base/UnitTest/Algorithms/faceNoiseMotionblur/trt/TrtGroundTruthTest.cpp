//
// Created by Admin on 2021/6/18.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)


/*TEST(FaceNoiseMotionblur, TRT009) {

  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-noise-motionblur-classifier-f32-d-0.0.9-rc.yml";
  parameter.ymlInput = getDir() + "input_params/face-noise-motionblur-classifier-f32-d-0.0.9-i.yml";
  parameter.imgPath = getDir() + "data/predict_face_noise_motionblur/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-noise-motionblur-classifier-f32-d-0.0.9.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
parameter.result=&result;
  TrtFaceNoiseMotionblurUnittest unittest(parameter);
  unittest.test();
}*/

TEST(FaceNoiseMotionblur, TRT010) {

  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-noise-motionblur-classifier-f32-d-0.1.0-rc.yml";
  parameter.ymlInput = getDir() + "input_params/face-noise-motionblur-classifier-f32-d-0.1.0-i.yml";
  parameter.imgPath = getDir() + "data/predict_face_noise_motionblur/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-noise-motionblur-classifier-f32-d-0.1.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::FaceNoiseMotionblurResult result;
  parameter.result=&result;
  TrtFaceNoiseMotionblurUnittest unittest(parameter);
  unittest.test();
}


#endif
#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)


TEST(PersonPose, TRT003) {

  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/pose-predictor-f32-d-0.0.3-rc.yml";
  parameter.ymlInput = getDir() + "input_params/pose-predictor-f32-d-0.0.3-i.yml";
  parameter.imgPath = getDir() + "data/predict_pose/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/body/pose-predictor-f32-d-0.0.3.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::PersonPoseResult result;
  parameter.result=&result;
  TrtPersonPoseUnittest unittest(parameter);
  unittest.test();
}


#endif
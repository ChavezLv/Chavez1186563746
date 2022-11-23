//
// Created by Admin on 2021/6/18.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)


TEST(AgeGender, TRT010) {
  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gender-age-d-0.1.0-rc.yml";
  parameter.ymlInput = getDir() + "input_params/face-gender-age-d-0.1.0-i.yml";
  parameter.imgPath = getDir() + "data/predict_face_gender_age/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-gender-age-d-0.1.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
parameter.result=&result;
  TrtAgeGenderUnittest unittest(parameter);
  unittest.test();
}


#endif
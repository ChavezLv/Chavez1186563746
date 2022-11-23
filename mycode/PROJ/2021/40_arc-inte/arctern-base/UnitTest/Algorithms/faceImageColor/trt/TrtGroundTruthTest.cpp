//
// Created by Admin on 2021/6/18.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)


TEST(FaceImageColor, TRT021) {
  /*TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/ANDROID_TVM_ARM32/rk3288_CPU@face-image-color-f32-d-1.0.2-rc.yml";
  parameter.ymlInput = getDir() + "input_params/rk3288_CPU@face-image-color-f32-d-1.0.2-i.yml";
  parameter.imgPath = getDir() + "data/predict_face_image_color/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/tvm/rk3288/rk3288_CPU_image_color_1.0.2/face-image-color-f32-d-1.0.2.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
parameter.result=&result;
  TrtFaceImageColorUnittest unittest(parameter);
  unittest.test();*/
}


#endif
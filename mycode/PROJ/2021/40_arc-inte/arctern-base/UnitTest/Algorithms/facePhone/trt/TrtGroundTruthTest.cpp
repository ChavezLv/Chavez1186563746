//
// Created by Admin on 2019/6/11.
//
#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)


TEST(FacePhone, TRT001) {
 TrtTestParam parameter;
 parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-phone-f32-d-0.0.1-rc.yml";
 parameter.ymlInput = getDir() + "input_params/face-phone-f32-d-0.0.1-i.yml";
 parameter.imgPath = getDir() + "data/predict_face_phone/";
 parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-phone-f32-d-0.0.1.bin");
 parameter.irModelPath = "trt_mid_model_file.test";
 parameter.gpuIndex = 0;
 arctern::FacePhoneResult result;
 parameter.result=&result;
 TrtFacePhoneUnittest unittest(parameter);
 unittest.test();
}




#endif
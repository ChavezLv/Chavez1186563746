//
// Created by Admin on 2021/6/18.
//

#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)
TEST(MultiDetectCenter, BODY_FACE_HEAD100) {
  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/body-face-head-detector-center-combined-f32-d-1.0.0-rc.json";
  parameter.ymlInput = "";
  parameter.imgPath = getDir() + "data/body_face_head_detector_1.0/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/unclassified/body-face-head-detector-center-combined-f32-d-1.0.0.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::MultiDetectCenterResult result;
  parameter.result=&result;
  TrtMultiDetectCenterUnittest unittest(parameter);
  unittest.test();
}
/*
TEST(bodyFaceHeadDetCenter, TRT_CENTERCOMBINED004) {
  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/body-face-head-detector-center-combined-f32-d-0.0.4-rc.yml";
  parameter.ymlInput = "";
  parameter.imgPath = getDir() + "data/detect_body_face_head_center/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/unclassified/body-face-head-detector-center-combined-f32-d-0.0.4.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::MultiDetectCenterResult result;
  parameter.result=&result;
  TrtMultiDetectCenterUnittest unittest(parameter);
  unittest.test();
}
*/

/*
TEST(bodyFaceHeadDetCenter, TRT_CENTERCOMBINED006) {
  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/body-face-head-detector-center-combined-f32-d-0.0.6-rc.yml";
  parameter.ymlInput = "";
  parameter.imgPath = getDir() + "data/detect_body_face_head_center/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/unclassified/body-face-head-detector-center-combined-f32-d-0.0.6.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;

  arctern::MultiDetectCenterResult result;
  parameter.result=&result;
  TrtMultiDetectCenterUnittest unittest(parameter);
  unittest.test();
}
*/
/*TEST(MultiDetectCenter, TRT002) {
  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/multi-detector-center-combined-f32-d-0.0.2-rc.yml";
  parameter.ymlInput = "";
  parameter.imgPath = getDir() + "data/detect_multi_cat_center/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/unclassified/multi-detector-center-combined-f32-d-0.0.2.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::MultiDetectCenterResult result;
  parameter.result=&result;
  TrtMultiDetectCenterUnittest unittest(parameter);
  unittest.test();
}*/

TEST(MultiDetectCenter, TRT004) {
  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/multi-detector-center-combined-f32-d-0.0.4-rc.yml";
  parameter.ymlInput = "";
  parameter.imgPath = getDir() + "data/detect_multi_cat_center/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/unclassified/multi-detector-center-combined-f32-d-0.0.4.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
  arctern::MultiDetectCenterResult result;
  parameter.result=&result;
  TrtMultiDetectCenterUnittest unittest(parameter);
  unittest.test();
}


/*
TEST(MultiDetectCenter, TRT001) {
  TrtTestParam parameter;
  parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/multi-detector-center-combined-f32-d-0.0.1-rc.yml";
  parameter.ymlInput = "";
  parameter.imgPath = getDir() + "data/detect_multi_cat_center/";
  parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/unclassified/multi-detector-center-combined-f32-d-0.0.1.bin");
  parameter.irModelPath = "trt_mid_model_file.test";
  parameter.gpuIndex = 0;
parameter.result=&result;
  TrtMultiDetectCenterUnittest unittest(parameter);
  unittest.test();
}*/



#endif
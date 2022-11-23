//
// Created by Admin on 2019/6/11.
//
#include "TrtGroundTruth.h"
#include "gtest/gtest.h"
#include "UnitTest/config.h"

#if defined(TRT_ALLOWED)


TEST(FaceMask, TRT021) {

 TrtTestParam parameter;
 parameter.ymlGroundTruth = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-mask-d-0.2.1-rc.yml";
 parameter.ymlInput = getDir() + "input_params/face-mask-d-0.2.1-i.yml";
 parameter.imgPath = getDir() + "data/predict_face_mask/";
 parameter.modelPath = ARCTERN_PROJECT_PATH + string("/../arctern-models/develop/face/face-mask-d-0.2.1.bin");
 parameter.irModelPath = "trt_mid_model_file.test";
 parameter.gpuIndex = 0;
 arctern::FaceMaskResult result;
 parameter.result=&result;
 TrtFaceMaskUnittest unittest(parameter);
 unittest.test();
}


#endif
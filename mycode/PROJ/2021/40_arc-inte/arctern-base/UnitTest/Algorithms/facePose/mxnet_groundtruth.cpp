/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.27
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "../../config.h"

#ifdef MXNET_ALLOWED
TEST(FacePose, MXNET) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-pose-cls-d-1.0.0-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-pose-cls-d-1.0.0-i.yml";
  std::string imgDir = getDir() + "data/predict_face_pose_cls/";
  std::string modelPath = ARCTERN_PROJECT_PATH +
      std::string("/../arctern-models/develop/face/face-pose-cls-d-1.0.0.bin");
  arctern::FacePoseResult result;
  FacePoseUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}

#elif defined(TVM_ALLOWED)
TEST(FacePose, MXNET) {
  std::string ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-pose-cls-d-1.0.0-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-pose-cls-d-1.0.0-i.yml";
  std::string imgDir = getDir() + "data/predict_face_pose_cls/";
  std::string modelPath = ARCTERN_PROJECT_PATH +
      std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-pose-cls-tvm-f32-d-1.0.0-var7.bin");
  arctern::FacePoseResult result;
  FacePoseUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);
  unittest.test();
}
#endif


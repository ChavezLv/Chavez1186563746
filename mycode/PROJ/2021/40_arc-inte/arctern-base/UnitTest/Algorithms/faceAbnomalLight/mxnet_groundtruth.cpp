/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.14
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "../../config.h"

#ifdef MXNET_ALLOWED
TEST(FaceAbnomalLight, MXNET) {
  std::string
      ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-abnomal-light-classifier-d-0.0.1-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-abnomal-light-classifier-d-0.0.1-i.yml";
  std::string imgDir = getDir() + "data/predict_face_abnomal_light/";
  std::string modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-abnomal-light-classifier-d-0.0.1.bin");
  arctern::FaceAbnomalLightResult result;
  FaceAbnomalLightUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);

  unittest.test();
}

TEST(FaceAbnomalLight, MXNET004) {
  std::string
      ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-abnormal-light-classifier-f32-d-0.0.4-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-abnormal-light-classifier-f32-d-0.0.4-i.yml";
  std::string imgDir = getDir() + "data/predict_face_abnomal_light/";
  std::string modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-abnormal-light-classifier-f32-d-0.0.4.bin");
  arctern::FaceAbnomalLightResult result;
  FaceAbnomalLightUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);

  unittest.test();
}

#elif defined(TVM_ALLOWED)
TEST(FaceAbnomalLight, TVM004) {
  std::string
      ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-abnormal-light-classifier-f32-d-0.0.4-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-abnormal-light-classifier-f32-d-0.0.4-i.yml";
  std::string imgDir = getDir() + "data/predict_face_abnomal_light/";
  std::string modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-abnormal-light-classifier-tvm-f32-d-0.0.4-var7.bin");
  arctern::FaceAbnomalLightResult result;
  FaceAbnomalLightUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);

  unittest.test();
}

TEST(FaceAbnomalLight, TVM003) {
  std::string
      ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-abnormal-light-classifier-f32-d-0.0.3-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-abnormal-light-classifier-f32-d-0.0.3-i.yml";
  std::string imgDir = getDir() + "data/predict_face_abnomal_light/";
  std::string modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-abnormal-light-classifier-tvm-f32-d-0.0.3-var7.bin");
  arctern::FaceAbnomalLightResult result;
  FaceAbnomalLightUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);

  unittest.test();
}

TEST(FaceAbnomalLight, TVM002) {
  std::string
      ymlGroundTruthPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-abnomal-light-classifier-d-0.0.2-rc.yml";
  std::string ymlInputPath = getDir() + "input_params/face-abnomal-light-classifier-d-0.0.2-i.yml";
  std::string imgDir = getDir() + "data/predict_face_abnomal_light/";
  std::string modelPath =
      ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-abnomal-light-classifier-tvm-f32-d-0.0.2-var7.bin");
  arctern::FaceAbnomalLightResult result;
  FaceAbnomalLightUnittest unittest(ymlGroundTruthPath, ymlInputPath, imgDir, modelPath,&result);

  unittest.test();
}
#endif

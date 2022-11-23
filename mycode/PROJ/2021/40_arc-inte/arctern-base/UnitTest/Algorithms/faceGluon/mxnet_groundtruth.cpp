/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.21
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "../../config.h"

#ifdef MXNET_ALLOWED
TEST(FaceGluon, MXNET_741) {
  std::string inputParamPath =
      getDir() + "input_params/face-gluon-f32-d-7.4.1-i.json";
  std::string
      ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-f32-d-7.4.1-rc.json";
  std::string imgDir = getDir() + "data/face_gluon_7.3/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-gluon-f32-d-7.4.1.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FaceGluon, MXNET_740) {
  std::string inputParamPath =
      getDir() + "input_params/face-gluon-f32-d-7.4.0-i.json";
  std::string
      ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-f32-d-7.4.0-rc.json";
  std::string imgDir = getDir() + "data/face_gluon_7.3/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-gluon-f32-d-7.4.0.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}
TEST(FaceGluon, MXNET_730) {
  std::string inputParamPath =
      getDir() + "input_params/face-gluon-f32-d-7.3.0-i.json";
  std::string
      ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-f32-d-7.3.0-rc.json";
  std::string imgDir = getDir() + "data/face_gluon_7.3/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-gluon-f32-d-7.3.0.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FaceGluon, MXNET_190) {
  std::string inputParamPath = getDir() + "input_params/face-gluon-1.9.0-i.yml";
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-1.9.0-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-gluon-1.9.0.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FaceGluon, MXNET_230) {
  std::string inputParamPath = getDir() + "input_params/face-gluon-2.3.0-i.yml";
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-2.3.0-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-gluon-2.3.0.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FaceGluon, MXNET_240) {
  std::string inputParamPath = getDir() + "input_params/face-gluon-2.4.0-i.yml";
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-2.4.0-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-gluon-2.4.0.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FaceGluon, MXNET_250) {
  std::string inputParamPath = getDir() + "input_params/face-gluon-2.5.0-i.yml";
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-2.5.0-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-gluon-2.5.0.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FaceGluon, MXNET_335) {
  std::string inputParamPath = getDir() + "input_params/face-gluon-d-3.3.5-i.yml";
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-d-3.3.5-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-gluon-d-3.3.5.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FaceGluon, MXNET_340) {
  std::string inputParamPath = getDir() + "input_params/face-gluon-d-3.4.0-i.yml";
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-d-3.4.0-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-gluon-d-3.4.0.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FaceGluon, MXNET_355) {
  std::string inputParamPath = getDir() + "input_params/face-gluon-d-3.5.5-i.yml";
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-d-3.5.5-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-gluon-d-3.5.5.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FaceGluon, MXNET_360) {
  std::string inputParamPath = getDir() + "input_params/face-gluon-d-3.6.0-i.yml";
  std::string
      ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-d-3.6.0-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-gluon-d-3.6.0.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FaceGluon, MXNET_370) {
  std::string inputParamPath =
      getDir() + "input_params/face-gluon-d-3.7.0-i.yml";
  std::string
      ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-d-3.7.0-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-gluon-d-3.7.0.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FaceGluon, MXNET_380) {
  std::string inputParamPath =
      getDir() + "input_params/face-gluon-f32-d-3.8.0-i.yml";
  std::string
      ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-f32-d-3.8.0-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-gluon-f32-d-3.8.0.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FaceGluon, MXNET_710) {
  std::string inputParamPath =
      getDir() + "input_params/face-gluon-f32-d-7.1.0-i.yml";
  std::string
      ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-f32-d-7.1.0-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-gluon-f32-d-7.1.0.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

/*TEST(FaceGluon, MXNET_450) {
  std::string inputParamPath = getDir() + "input_params/face-gluon-d-4.5.0-i.yml";
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-d-4.5.0-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-gluon-d-4.5.0.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FaceGluon, MXNET_394) {
  std::string inputParamPath = getDir() + "input_params/face-gluon-f32-d-3.9.4-i.yml";
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-f32-d-3.9.4-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-gluon-f32-d-3.9.4.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}*/

#elif defined(HISI_ALLOWED)
TEST(FaceGluon, HISI_361) {
  std::string inputParamPath = getDir() + "input_params/face-gluon-nnie3516-i8-q1-d-3.6.1-i.yml";
  std::string ymlPath = getDir() + "results_arcterncpp/ARMLINUX_NNIE3516/face-gluon-nnie3516-i8-q1-d-3.6.1-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/nnie3516/face-gluon-nnie3516-i8-q1-d-3.6.1.bin");

  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}
#elif defined(TVM_ALLOWED) && defined(__arm__)
TEST(FaceGluon, TVM_380) {
  std::string inputParamPath = getDir() + "input_params/rk3288_CPU@face-gluon-rk3288C-f32-d-3.8.0-i.yml";
  std::string ymlPath = getDir() + "results_arcterncpp/ANDROID_TVM_ARM32/rk3288_CPU@face-gluon-rk3288C-f32-d-3.8.0-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm/rk3288/rk3288_CPU_gluon_3.8.0/face-gluon-rk3288C-f32-d-3.8.0.bin");
arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

#elif defined(TVM_ALLOWED)
TEST(FaceGluon, TVM_380) {
  arctern::FaceGluonResult result;
  std::string inputParamPath = getDir() + "input_params/rk3288_CPU@face-gluon-rk3288C-f32-d-3.8.0-i.yml";
  std::string ymlPath = getDir() + "results_arcterncpp/ANDROID_TVM_ARM32/rk3288_CPU@face-gluon-rk3288C-f32-d-3.8.0-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-gluon-tvm-f32-d-3.8.0-var7.bin");
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FaceGluon, TVM_390) {
  std::string inputParamPath =
      getDir() + "input_params/face-gluon-f32-d-3.9.0-i.yml";
  std::string
      ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-f32-d-3.9.0-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-gluon-tvm-f32-d-3.9.0-var7.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FaceGluon, TVM_370) {
  std::string inputParamPath =
      getDir() + "input_params/face-gluon-d-3.7.0-i.yml";
  std::string
      ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-d-3.7.0-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-gluon-tvm-f32-d-3.7.0-var7.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FaceGluon, TVM_360) {
  std::string inputParamPath =
      getDir() + "input_params/face-gluon-d-3.6.0-i.yml";
  std::string
      ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-d-3.6.0-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-gluon-tvm-f32-d-3.6.0-var7.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FaceGluon, TVMT_250) {
  std::string inputParamPath = getDir() + "input_params/face-gluon-2.5.0-i.yml";
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-2.5.0-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-gluon-tvm-f32-d-2.5.0-var7.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

// TEST(FaceGluon, TVM_350) {
//   std::string inputParamPath =
//       getDir() + "input_params/face-gluon-f32-d-3.5.0-i.yml";
//   std::string
//       ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-f32-d-3.5.0-rc.yml";
//   std::string imgDir = getDir() + "data/get_face_feat/";
//   std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-gluon-tvm-f32-d-3.5.0-var7.bin");
//   arctern::FaceGluonResult result;
//   FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
//   unittest.test();
// }

TEST(FaceGluon, TVM_720) {
  std::string inputParamPath =
      getDir() + "input_params/face-gluon-f32-d-7.2.0-i.yml";
  std::string
      ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-f32-d-7.2.0-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-gluon-tvm-f32-d-7.2.0-var7.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();


}
#elif RKNN_ALLOWED
TEST(FaceGluon, RKNN380) {
  std::string inputParamPath = getDir() +
      "input_params/face-gluon-f32-d-3.8.0-i.yml";
  std::string ymlPath = getDir() +
      "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-f32-d-3.8.0-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = std::string("/userdata/rknn/models/face-gluon-rv1109N-f16-q1-d-3.8.0.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FaceGluon, RKNN386) {
  std::string inputParamPath = getDir() +
      "input_params/face-access-f32-d-3.8.6-i.yml";
  std::string ymlPath = getDir() +
      "results_arcterncpp/LINUX_MXNET_CPU/face-access-f32-d-3.8.6-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = std::string("/userdata/rknn/models/face-gluon-rv1109N-f16-q1-d-3.8.6.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}


TEST(FaceGluon, RKNN390) {
  std::string inputParamPath = getDir() +
      "input_params/face-gluon-f32-d-3.9.0-i.yml";
  std::string ymlPath = getDir() +
      "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-f32-d-3.9.0-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = std::string("/userdata/rknn/models/face-gluon-rv1109N-f16-q1-d-3.9.0.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

TEST(FaceGluon, RKNN394) {
  std::string inputParamPath = getDir() +
      "input_params/face-gluon-f32-d-3.9.4-i.yml";
  std::string ymlPath = getDir() +
      "results_arcterncpp/LINUX_MXNET_CPU/face-gluon-f32-d-3.9.4-rc.yml";
  std::string imgDir = getDir() + "data/get_face_feat/";
  std::string modelPath = std::string("/userdata/rknn/models/face-gluon-rv1109N-f16-q1-d-3.9.4.bin");
  arctern::FaceGluonResult result;
  FaceGluonUnittest unittest(ymlPath, inputParamPath, imgDir, modelPath,&result);
  unittest.test();
}

#endif

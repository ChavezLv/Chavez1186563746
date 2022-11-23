/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.17
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "UnitTest/config.h"
#include "UnitTest/Algorithms/faceDetectTiny/groundtruth.h"
#include "gtest/gtest.h"
#ifdef MNN_ALLOWED

TEST(FaceDetectTiny, MNN_video101) {
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-det-video-f32-d-1.0.1-rc.json";
  std::string imgDir = getDir() + "data/face_det_video_1.0.1/";
  std::string modelPath =
        ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-det-tiny-f32-d-1.0.1.bin");
  arctern::FaceDetectTinyResult result;
  FaceDetectTinyUnittest unittest(ymlPath, imgDir, modelPath,&result);
  unittest.test();
}

#elif MXNET_ALLOWED
TEST(FaceDetectTiny, MXNET_tiny200) {
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-det-tiny-f32-d-2.0.0-rc.yml";
  std::string imgDir = getDir() + "data/detect_tiny_face/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-det-tiny-f32-d-2.0.0.bin");
  arctern::FaceDetectTinyResult result;
  FaceDetectTinyUnittest unittest(ymlPath, imgDir, modelPath,&result);
  unittest.test();
}
TEST(FaceDetectTiny, MXNET_video003) {
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-det-video-d-0.0.3-rc.yml";
  std::string imgDir = getDir() + "data/detect_tiny_face/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-det-video-d-0.0.3.bin");
  arctern::FaceDetectTinyResult result;
  FaceDetectTinyUnittest unittest(ymlPath, imgDir, modelPath,&result);
  unittest.test();
}
TEST(FaceDetectTiny, MXNET_video004) {
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-det-video-f32-d-0.0.4-rc.yml";
  std::string imgDir = getDir() + "data/detect_tiny_face/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-det-video-f32-d-0.0.4.bin");
  arctern::FaceDetectTinyResult result;
  FaceDetectTinyUnittest unittest(ymlPath, imgDir, modelPath,&result);
  unittest.test();
}
TEST(FaceDetectTiny, MXNET_video005) {
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-det-video-f32-d-0.0.5-rc.yml";
  std::string imgDir = getDir() + "data/detect_tiny_face/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/face-det-video-f32-d-0.0.5.bin");
  arctern::FaceDetectTinyResult result;
  FaceDetectTinyUnittest unittest(ymlPath, imgDir, modelPath,&result);
  unittest.test();
}

#elif defined(HISI_ALLOWED)


TEST(FaceDetectTiny, HISI) {
  std::string ymlPath = getDir() + "results_arcterncpp/ARMLINUX_NNIE3516/face-det-video-nnie3516-i8-0.0.3-q1-rc.yml";
  std::string imgDir = getDir() + "data/detect_tiny_face/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/nnie3516/face-det-video-nnie3516-i8-0.0.3-q1.bin");
  FaceDetectTinyUnittest unittest(ymlPath, imgDir, modelPath,&result);
  unittest.test();
}

#elif defined(TVM_ALLOWED)

TEST(FaceDetectTiny, TVM_video003) {
  std::string ymlPath = getDir() + "results_arcterncpp/LINUX_MXNET_CPU/face-det-video-d-0.0.3-rc.yml";
  std::string imgDir = getDir() + "data/detect_tiny_face/";
  std::string
      modelPath = ARCTERN_PROJECT_PATH + std::string("/../arctern-models/develop/face/tvm0.7/avx2/face-det-video-tvm-f32-d-0.0.3-var7.bin");
  arctern::FaceDetectTinyResult result;
  FaceDetectTinyUnittest unittest(ymlPath, imgDir, modelPath,&result);
  unittest.test();
}

#elif defined(RKNN_ALLOWED)
TEST(FaceDetectTiny, RKNN003) {
  std::string ymlPath = getDir() + "results_arcterncpp/ARMLINUX_NNIE3516/face-det-video-nnie3516-i8-0.0.3-q1-rc.yml";
  std::string imgDir = getDir() + "data/detect_tiny_face/";
  std::string modelPath =   std::string("/userdata/rknn/models/face-det-video-rv1109N-i8-q1-d-0.0.3.bin");
  arctern::FaceDetectTinyResult result;
  FaceDetectTinyUnittest unittest(ymlPath, imgDir, modelPath,&result);
  unittest.test();
}
#endif

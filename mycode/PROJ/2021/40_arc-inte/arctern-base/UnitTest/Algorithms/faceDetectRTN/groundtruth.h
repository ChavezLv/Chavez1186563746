/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.13
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef UNITTEST_ALGORITHMS_FACEDETECTRTN_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_FACEDETECTRTN_GROUNDTRUTH_H_

#include "src/Algorithms/faceDetectRTN/faceDetectRTN.h"
#include "yaml-cpp/yaml.h"
#include "log/logger.h"
#include "yaml-cpp/node/parse.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"
typedef struct {
  std::vector<arctern::ArcternRect> rects;
  std::vector<float> confidences;
} ONEPICTURE;

class FaceDetectRTNData : public BaseGroundTruthData {
 public:
  FaceDetectRTNData();
  ~FaceDetectRTNData() override;
  void addGroundTruth(YAML::Node &, std::string) override;

 public:
  std::vector<ONEPICTURE> faceRects_;
};

class FaceDetectRTNUnittest : public BaseAlgrithmUnittest {
 public:
  FaceDetectRTNUnittest(std::string ymlPath, std::string imgDir, std::string modelpath,arctern::Result *p);
  ~FaceDetectRTNUnittest();

  std::shared_ptr<arctern::AlgorithmInterface> createAlgHandle(int batch) override;

  std::shared_ptr<arctern::RunParameter> buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                           int startId,
                                                           int len) override;

  std::shared_ptr<BaseGroundTruthData> generateGroundTruth() override;

  void checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                   int startId,
                   int len,
                   const arctern::Result *p) override;

 public:
  arctern::ArcternDeviceType deviceType_ = arctern::ARCTERN_CPU;
  int gpuId_ = -1;
  arctern::ArcternTypeBits typeBits_ = arctern::ARCTERN_FP32;

};
#endif  // UNITTEST_ALGORITHMS_FACEDETECTRTN_GROUNDTRUTH_H_

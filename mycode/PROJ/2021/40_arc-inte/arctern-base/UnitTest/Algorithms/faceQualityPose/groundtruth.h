/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.15
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef UNITTEST_ALGORITHMS_FACEQUALITYPOSE_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_FACEQUALITYPOSE_GROUNDTRUTH_H_

#include "src/Algorithms/faceQualityPose/faceQualityPose.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"


typedef struct {
  arctern::ArcternRect rect;
  arctern::SingleFaceQualityPose qualityPose;
} ONEPICTURE;

class FaceQualityPoseData : public BaseGroundTruthData {
 public:
  FaceQualityPoseData() = default;
  ~FaceQualityPoseData() override = default;

  void addGroundTruth(YAML::Node &, std::string) override;

  void addInputParam(YAML::Node &, std::string) override;

 public:
  std::vector<ONEPICTURE> faceQualityPose_;
};

class FaceQualityPoseUnittest : public BaseAlgrithmUnittest {
 public:
  FaceQualityPoseUnittest(std::string ymlGroundTruth, std::string ymlInput, std::string imgDir, std::string modelpath, arctern::Result *p);

  ~FaceQualityPoseUnittest() override ;

  std::shared_ptr<arctern::AlgorithmInterface> createAlgHandle(int batch) override;

  std::shared_ptr<arctern::RunParameter> buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                           int startId,
                                                           int len) override;

  std::shared_ptr<BaseGroundTruthData> generateGroundTruth() override;

  void checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                   int startId,
                   int len,
                   const arctern::Result *p) override;
};

#endif  // UNITTEST_ALGORITHMS_FACEQUALITYPOSE_GROUNDTRUTH_H_


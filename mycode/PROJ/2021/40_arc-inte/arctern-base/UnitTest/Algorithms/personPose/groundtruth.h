/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.09
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#ifndef UNITTEST_ALGORITHMS_PERSONPOSE_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_PERSONPOSE_GROUNDTRUTH_H_

#include "UnitTest/UnitTestBase/baseAlgrithm.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "src/Algorithms/personPose/personPose.h"
using namespace arctern;

typedef struct {
  arctern::ArcternRect rect;
  PersonPosePoints result;
} ONEPICTURE;

class personPoseData : public BaseGroundTruthData {
public:
  personPoseData() = default;
  ~personPoseData() override = default;

  void addGroundTruth(YAML::Node &, std::string) override;

  void addInputParam(YAML::Node &, std::string) override;

public:
  std::vector<ONEPICTURE> m_vResults;
};

class personPoseUnittest : public BaseAlgrithmUnittest {
public:
  personPoseUnittest(std::string ymlGroundTruth, std::string ymlInput,
                     std::string imgDir, std::string modelpath,arctern::Result *p);

  ~personPoseUnittest() override;

  std::shared_ptr<arctern::AlgorithmInterface>
  createAlgHandle(int batch) override;

  std::shared_ptr<arctern::RunParameter>
  buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata, int startId,
                    int len) override;

  std::shared_ptr<BaseGroundTruthData> generateGroundTruth() override;

  void checkResult(std::shared_ptr<BaseGroundTruthData> gdata, int startId,
                   int len, const arctern::Result *p) override;
};

#endif // UNITTEST_ALGORITHMS_VEHICLEATTR_GROUNDTRUTH_H_

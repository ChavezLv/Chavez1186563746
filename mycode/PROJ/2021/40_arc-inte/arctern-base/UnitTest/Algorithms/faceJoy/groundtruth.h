/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         kuanghongshen
 *  Last modified:  2021.06.03
 *  email:          kuang.hongshen@intellif.com
 ****************************************************************************/

#ifndef UNITTEST_ALGORITHMS_FACEJOY_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_FACEJOY_GROUNDTRUTH_H_
#include "UnitTest/UnitTestBase/baseAlgrithm.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "src/Algorithms/faceJoy/faceJoy.h"

typedef struct {
  arctern::ArcternRect rect;
  arctern::FaceJoyLandmarks landmarks;
  arctern::faceJoyScores result;
} ONEPICTURE;

class FaceJoyData : public BaseGroundTruthData {
public:
  FaceJoyData();
  ~FaceJoyData() override;

  void addGroundTruth(YAML::Node &, std::string) override;

  void addInputParam(YAML::Node &, std::string) override;

public:
  std::vector<ONEPICTURE> m_vLandmarks;
};

class FaceJoyUnittest : public BaseAlgrithmUnittest {
public:
  FaceJoyUnittest(std::string ymlGroundTruth, std::string ymlInput,
                  std::string imgDir, std::string modelpath,arctern::Result *p);

  ~FaceJoyUnittest() override;

  std::shared_ptr<arctern::AlgorithmInterface>
  createAlgHandle(int batch) override;

  std::shared_ptr<arctern::RunParameter>
  buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata, int startId,
                    int len) override;

  std::shared_ptr<BaseGroundTruthData> generateGroundTruth() override;

  void checkResult(std::shared_ptr<BaseGroundTruthData> gdata, int startId,
                   int len, const arctern::Result *p) override;
};
#endif

/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.03
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#ifndef UNITTEST_ALGORITHMS_OBJECTQUALITY_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_OBJECTQUALITY_GROUNDTRUTH_H_

#include "src/Algorithms/objectQuality/objectQuality.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"

typedef struct {
  arctern::ArcternRect rect;
  arctern::qualityScore score;
} ONEPICTURE;

class ObjectQualityData : public BaseGroundTruthData {
 public:
  ObjectQualityData() = default;
  ~ObjectQualityData() override = default;

  void addGroundTruth(YAML::Node &, std::string) override;

  void addInputParam(YAML::Node &, std::string) override;

 public:
  std::vector<ONEPICTURE> objectQuality_;
};

class ObjectQualityUnittest : public BaseAlgrithmUnittest {
 public:
  ObjectQualityUnittest(std::string ymlGroundTruth, std::string ymlInput, std::string imgDir, std::string modelpath, arctern::Result *p);

  ~ObjectQualityUnittest() override ;

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

#endif  // UNITTEST_ALGORITHMS_OBJECTQUALITY_GROUNDTRUTH_H_


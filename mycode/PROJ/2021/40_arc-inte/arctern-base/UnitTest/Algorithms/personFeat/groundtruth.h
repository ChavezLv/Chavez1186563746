/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.11.30
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#ifndef UNITTEST_ALGORITHMS_PERSONFEAT_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_PERSONFEAT_GROUNDTRUTH_H_

#include "src/Algorithms/personFeat/personFeat.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"

typedef struct {
  arctern::ArcternRect rect;
  std::vector<uint8> features;
} PersonFeatONEPICTURE;

class PersonFeatData : public BaseGroundTruthData {
 public:
  PersonFeatData() = default;
  ~PersonFeatData() override = default;

  void addGroundTruth(YAML::Node &, std::string) override;

  void addInputParam(YAML::Node &, std::string) override;

 public:
  std::vector<PersonFeatONEPICTURE> features_;
};

class PersonFeatUnittest : public BaseAlgrithmUnittest {
 public:
  PersonFeatUnittest(std::string ymlGroundTruth, std::string ymlInput, std::string imgDir, std::string modelpath, arctern::Result *p);

  ~PersonFeatUnittest() override ;

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

#endif  // UNITTEST_ALGORITHMS_PERSONFEAT_GROUNDTRUTH_H_

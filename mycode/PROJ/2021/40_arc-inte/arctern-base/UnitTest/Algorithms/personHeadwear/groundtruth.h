/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.02
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#ifndef UNITTEST_ALGORITHMS_PERSONHEADWEAR_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_PERSONHEADWEAR_GROUNDTRUTH_H_

#include "src/Algorithms/personHeadwear/personHeadwear.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"
#include <unordered_map>

typedef struct {
  arctern::ArcternRect rect;
  arctern::SinglePersonHeadwear headwear;
} ONEPICTURE;

class PersonHeadwearData : public BaseGroundTruthData {
 public:
  PersonHeadwearData() = default;
  ~PersonHeadwearData() override = default;

  void addGroundTruth(YAML::Node &, std::string) override;

  void addInputParam(YAML::Node &, std::string) override;

 public:
  std::vector<ONEPICTURE> headwears_;
  std::vector<std::string> typestrs = {"helmet", "hat", "None", "other"};
  std::unordered_map<std::string, arctern::PersonHeadwearType> str2type_map = {
      {typestrs.at(0), arctern::PersonHeadwearType::HELMET},
      {typestrs.at(1), arctern::PersonHeadwearType::HAT},
      {typestrs.at(2), arctern::PersonHeadwearType::NONE},
      {typestrs.at(3), arctern::PersonHeadwearType::OTHERTYPE},
  };
};

class PersonHeadwearUnittest : public BaseAlgrithmUnittest {
 public:
  PersonHeadwearUnittest(std::string ymlGroundTruth, std::string ymlInput, std::string imgDir, std::string modelpath, arctern::Result *p);

  ~PersonHeadwearUnittest() override ;

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
#endif  // UNITTEST_ALGORITHMS_PERSONHEADWEAR_GROUNDTRUTH_H_


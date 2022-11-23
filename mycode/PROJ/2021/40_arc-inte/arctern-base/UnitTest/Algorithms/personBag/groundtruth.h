/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.09
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#ifndef UNITTEST_ALGORITHMS_VEHICLEPERSONBAG_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_VEHICLEPERSONBAG_GROUNDTRUTH_H_

#include "UnitTest/UnitTestBase/baseAlgrithm.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "src/Algorithms/personBag/personBag.h"
using namespace arctern;

typedef struct {
  arctern::ArcternRect rect;
  std::pair<int, float> result;
} ONEPICTURE;

class personBagData : public BaseGroundTruthData {
public:
  personBagData() = default;
  ~personBagData() override = default;

  void addGroundTruth(YAML::Node &, std::string) override;

  void addInputParam(YAML::Node &, std::string) override;

public:
  std::vector<ONEPICTURE> m_vResults;
};

class personBagUnittest : public BaseAlgrithmUnittest {
public:
  personBagUnittest(std::string ymlGroundTruth, std::string ymlInput,
                    std::string imgDir, std::string modelpath,arctern::Result *p);

  ~personBagUnittest() override;

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

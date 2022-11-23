/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         kuanghongshen
 *  Last modified:  2021.06.03
 *  email:          kuang.hongshen@intellif.com
 ****************************************************************************/

#pragma once
#include "UnitTest/UnitTestBase/baseAlgrithm.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "src/Algorithms/policeSuit/policeSuit.h"

typedef struct {
  arctern::ArcternRect rect;
  std::pair<arctern::PoliceSuitClassify, float> result;
} ONEPICTURE;

class PoliceSuitData : public BaseGroundTruthData {
public:
  PoliceSuitData();
  ~PoliceSuitData() override;

  void addGroundTruth(YAML::Node &, std::string) override;

  void addInputParam(YAML::Node &, std::string) override;

public:
  std::vector<ONEPICTURE> suitResults;
};

class PoliceSuitUnittest : public BaseAlgrithmUnittest {
public:
  PoliceSuitUnittest(std::string ymlGroundTruth, std::string ymlInput,
                     std::string imgDir, std::string modelpath,arctern::Result *p);

  ~PoliceSuitUnittest() override;

  std::shared_ptr<arctern::AlgorithmInterface>
  createAlgHandle(int batch) override;

  std::shared_ptr<arctern::RunParameter>
  buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata, int startId,
                    int len) override;

  std::shared_ptr<BaseGroundTruthData> generateGroundTruth() override;

  void checkResult(std::shared_ptr<BaseGroundTruthData> gdata, int startId,
                   int len, const arctern::Result *p) override;
};

/****************************************************************************
 *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         kuanghongshen
 *  Last modified:  2021.06.07
 *  email:          kuang.hongshen@intellif.com
 ****************************************************************************/

#pragma once
#include "UnitTest/UnitTestBase/baseAlgrithm.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "src/Algorithms/dogDetTiny/dogDetTiny.h"

typedef struct {
  std::vector<arctern::ArcternRect> rects;
  std::vector<float> confidences;
} ONEPICTURE;

class dogDetTinyData : public BaseGroundTruthData {
public:
  dogDetTinyData() = default;
  ~dogDetTinyData() override = default;
  void addGroundTruth(YAML::Node &, std::string) override;

public:
  std::vector<ONEPICTURE> faceRects_;
};

class dogDetTinyUnittest : public BaseAlgrithmUnittest {
public:
  dogDetTinyUnittest(std::string ymlPath, std::string imgDir,
                     std::string modelpath,arctern::Result *p);
  ~dogDetTinyUnittest() override;

  std::shared_ptr<arctern::AlgorithmInterface>
  createAlgHandle(int batch) override;

  std::shared_ptr<arctern::RunParameter>
  buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata, int startId,
                    int len) override;

  std::shared_ptr<BaseGroundTruthData> generateGroundTruth() override;

  void checkResult(std::shared_ptr<BaseGroundTruthData> gdata, int startId,
                   int len, const arctern::Result *p) override;
};

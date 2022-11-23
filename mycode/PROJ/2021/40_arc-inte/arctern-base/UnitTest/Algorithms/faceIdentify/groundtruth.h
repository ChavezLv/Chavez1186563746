/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         kuanghongshen
 *  Last modified:  2021.06.29
 *  email:          kuang.hongshen@intellif.com
 ****************************************************************************/

#ifndef _UNITTEST_ALGORITHMS_FACEIDENTIFY_GROUNDTRUTH_H_
#define _UNITTEST_ALGORITHMS_FACEIDENTIFY_GROUNDTRUTH_H_
#include "UnitTest/UnitTestBase/baseAlgrithm.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "src/Algorithms/faceIdentify/faceIdentify.h"

typedef struct {
  arctern::ArcternRect rect;
  std::pair<int, float> result;
} ONEPICTURE;

class FaceIdentifyData : public BaseGroundTruthData {
public:
  FaceIdentifyData();
  ~FaceIdentifyData() override;

  void addGroundTruth(YAML::Node &, std::string) override;

  void addInputParam(YAML::Node &, std::string) override;

public:
  std::vector<ONEPICTURE> typeResults;
};

class FaceIdentifyUnittest : public BaseAlgrithmUnittest {
public:
  FaceIdentifyUnittest(std::string ymlGroundTruth, std::string ymlInput,
                   std::string imgDir, std::string modelpath,arctern::Result *p);

  ~FaceIdentifyUnittest() override;

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

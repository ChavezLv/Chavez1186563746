/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         author
 *  Last modified:  2021.12.21
 *  email:          nullptr
 ****************************************************************************/

#ifndef UNITTEST_ALGORITHMS_CARGOILLEGAL_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_CARGOILLEGAL_GROUNDTRUTH_H_

#include "src/Algorithms/cargoIllegalDetect/cargoIllegalDetect.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"

typedef struct {
  std::vector<arctern::ArcternRect> rects;
  std::vector<float> confidences;
  std::vector<int> types;
} ONEPICTURE;

class CargoIllegalDetectData : public BaseGroundTruthData {
 public:
  CargoIllegalDetectData() = default;
  ~CargoIllegalDetectData() override = default;
  void addGroundTruth(YAML::Node &, std::string) override;

 public:
  std::vector<ONEPICTURE> rects_;
};

class CargoIllegalDetectUnittest : public BaseAlgrithmUnittest {
 public:
  CargoIllegalDetectUnittest(std::string ymlPath, std::string imgDir, std::string modelpath,arctern::Result *p);
  ~CargoIllegalDetectUnittest() override;

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
#endif  // UNITTEST_ALGORITHMS_CARGOILLEGAL_GROUNDTRUTH_H_

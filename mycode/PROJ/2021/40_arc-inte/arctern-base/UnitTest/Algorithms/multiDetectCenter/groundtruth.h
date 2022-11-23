/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.11.27
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#ifndef UNITTEST_ALGORITHMS_MULTIDETECTCENTER_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_MULTIDETECTCENTER_GROUNDTRUTH_H_

#include "src/Algorithms/multiDetectCenter/multiDetectCenter.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"

typedef struct {
  std::vector<arctern::ArcternRect> rects;
  std::vector<float> confidences;
  std::vector<int> types;
} ONEPICTURE;

class MultiDetectCenterData : public BaseGroundTruthData {
 public:
  MultiDetectCenterData() = default;
  ~MultiDetectCenterData() override = default;
  void addGroundTruth(YAML::Node &, std::string) override;
  void addGroundTruth(std::string filePath);
 public:
  std::vector<ONEPICTURE> multicatRects_;
};

class MultiDetectCenterUnittest : public BaseAlgrithmUnittest {
 public:
  MultiDetectCenterUnittest(std::string ymlPath, std::string imgDir, std::string modelpath,arctern::Result *p);
  ~MultiDetectCenterUnittest() override;

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
#endif  // UNITTEST_ALGORITHMS_MULTIDETECTCENTER_GROUNDTRUTH_H_

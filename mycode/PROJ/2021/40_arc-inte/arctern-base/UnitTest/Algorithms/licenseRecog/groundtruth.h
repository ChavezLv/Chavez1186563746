/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.02
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#ifndef UNITTEST_ALGORITHMS_LICENSERECOG_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_LICENSERECOG_GROUNDTRUTH_H_

#include "src/Algorithms/licenseRecog/licenseRecog.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"
#include <unordered_map>

typedef struct {
  arctern::ArcternRect rect;
  std::string license;
  std::vector<float> scores;
} ONEPICTURE;

class LicenseRecogData : public BaseGroundTruthData {
 public:
  LicenseRecogData() = default;
  ~LicenseRecogData() override = default;

  void addGroundTruth(YAML::Node &, std::string) override;

  void addInputParam(YAML::Node &, std::string) override;
  void addGroundTruth(std::string filePath);
  void addInputParam(std::string filePath);
 public:
  std::vector<ONEPICTURE> licenses_;
};

class LicenseRecogUnittest : public BaseAlgrithmUnittest {
 public:
  LicenseRecogUnittest(std::string ymlGroundTruth, std::string ymlInput, std::string imgDir, std::string modelpath, arctern::Result *p);

  ~LicenseRecogUnittest() override ;

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
#endif  // UNITTEST_ALGORITHMS_LICENSERECOG_GROUNDTRUTH_H_


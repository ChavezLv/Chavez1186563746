/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.17
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef UNITTEST_ALGORITHMS_BANNERDET_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_BANNERDET_GROUNDTRUTH_H_

#include "src/Algorithms/bannerDet/bannerDet.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"

typedef struct {
  std::vector<arctern::ArcternRect> rects;
  std::vector<float> confidences;
  std::vector<int> types;
} ONEPICTURE;

class BannerDetData : public BaseGroundTruthData {
 public:
  BannerDetData() = default;
  ~BannerDetData() override = default;
  void addGroundTruth(YAML::Node &, std::string) override;

 public:
  std::vector<ONEPICTURE> rects_;
};

class BannerDetUnittest : public BaseAlgrithmUnittest {
 public:
  BannerDetUnittest(std::string ymlPath, std::string imgDir,\
            std::string modelpath, arctern::Result *p);

  ~BannerDetUnittest() override;

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
#endif  // UNITTEST_ALGORITHMS_BANNERDET_GROUNDTRUTH_H_

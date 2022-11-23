/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.28
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef UNITTEST_ALGORITHMS_FACESMOKE_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_FACESMOKE_GROUNDTRUTH_H_

#include "src/Algorithms/faceSmoke/faceSmoke.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"

typedef struct {
  arctern::ArcternRect rect;
  arctern::SingleFaceSmoke smoke;
} ONEPICTURE;

class FaceSmokeData : public BaseGroundTruthData {
 public:
  FaceSmokeData() = default;
  ~FaceSmokeData() override = default;

  void addGroundTruth(YAML::Node &, std::string) override;

  void addInputParam(YAML::Node &, std::string) override;

 public:
  std::vector<ONEPICTURE> faceSmoke_;
};

class FaceSmokeUnittest : public BaseAlgrithmUnittest {
 public:
  FaceSmokeUnittest(std::string ymlGroundTruth, std::string ymlInput, std::string imgDir, std::string modelpath, arctern::Result *p);

  ~FaceSmokeUnittest() override ;

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

#endif  // UNITTEST_ALGORITHMS_FACESMOKE_GROUNDTRUTH_H_


/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.14
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef UNITTEST_ALGORITHMS_FACEABNOMALLIGHT_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_FACEABNOMALLIGHT_GROUNDTRUTH_H_

#include "src/Algorithms/faceAbnomalLight/faceAbnomalLight.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"

typedef struct {
  arctern::ArcternRect rect;
  arctern::SingleFaceAbnomalLight al;
} ONEPICTURE;

class FaceAbnomalLightData : public BaseGroundTruthData {
 public:
  FaceAbnomalLightData() = default;
  ~FaceAbnomalLightData() override = default;

  void addGroundTruth(YAML::Node &, std::string) override;

  void addInputParam(YAML::Node &, std::string) override;

 public:
  std::vector<ONEPICTURE> abnomalLight_;

};

class FaceAbnomalLightUnittest : public BaseAlgrithmUnittest {
 public:
  FaceAbnomalLightUnittest(std::string ymlGroundTruth,
                           std::string ymlInput,
                           std::string imgDir,
                           std::string modelpath,
                           arctern::Result * result);
  ~FaceAbnomalLightUnittest() override ;
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
#endif  // UNITTEST_ALGORITHMS_FACEABNOMALLIGHT_GROUNDTRUTH_H_


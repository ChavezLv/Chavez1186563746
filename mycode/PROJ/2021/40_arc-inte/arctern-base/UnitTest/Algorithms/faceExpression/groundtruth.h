/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.24
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef UNITTEST_ALGORITHMS_FACEEXPRESSION_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_FACEEXPRESSION_GROUNDTRUTH_H_

#include "src/Algorithms/faceExpression/faceExpression.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"

typedef struct {
  std::string file;
  arctern::ArcternRect rect;
  arctern::SingleFaceExpression expression;
} ONEPICTURE;

class FaceExpressionData : public BaseGroundTruthData {
 public:
  FaceExpressionData() = default;
  ~FaceExpressionData() override = default;

  void addGroundTruth(YAML::Node &, std::string) override;

  void addInputParam(YAML::Node &, std::string) override;

 public:
  std::vector<ONEPICTURE> expression_;
};

class FaceExpressionUnittest : public BaseAlgrithmUnittest {
 public:
  FaceExpressionUnittest(std::string ymlGroundTruth, std::string ymlInput, std::string imgDir, std::string modelpath, arctern::Result *p);

  ~FaceExpressionUnittest() override ;

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
#endif  // UNITTEST_ALGORITHMS_FACEEXPRESSION_GROUNDTRUTH_H_



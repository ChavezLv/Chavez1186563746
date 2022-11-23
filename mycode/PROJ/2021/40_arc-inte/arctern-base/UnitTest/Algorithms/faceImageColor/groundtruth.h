/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef UNITTEST_ALGORITHMS_FACEIMAGECOLOR_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_FACEIMAGECOLOR_GROUNDTRUTH_H_


#include "src/Algorithms/faceImageColor/faceImageColor.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"

typedef struct {
  arctern::ArcternRect rect;
  float score;
} ONEPICTURE;

class FaceImageColorData : public BaseGroundTruthData {
 public:
  FaceImageColorData() = default;
  ~FaceImageColorData() override = default;

  void addGroundTruth(YAML::Node &, std::string) override;

  void addInputParam(YAML::Node &, std::string) override;

 public:
  std::vector<ONEPICTURE> faceImageColor_;
};

class FaceImageColorUnittest : public BaseAlgrithmUnittest {
 public:
  FaceImageColorUnittest(std::string ymlGroundTruth, std::string ymlInput, std::string imgDir, std::string modelpath, arctern::Result *p);

  ~FaceImageColorUnittest() override ;

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

#endif  // UNITTEST_ALGORITHMS_FACEIMAGECOLOR_GROUNDTRUTH_H_



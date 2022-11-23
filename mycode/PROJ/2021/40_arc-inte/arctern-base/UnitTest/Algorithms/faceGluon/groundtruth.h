/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.21
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef UNITTEST_ALGORITHMS_FACEGLUON_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_FACEGLUON_GROUNDTRUTH_H_

#include "src/Algorithms/faceGluon/faceGluon.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"

typedef struct {
  arctern::ArcternRect rect;
  std::vector<arctern::ArcternPoint> landmarks;
  std::vector<uint8> features;
} GluonONEPICTURE;

class FaceGluonData : public BaseGroundTruthData {
 public:
  FaceGluonData() = default;
  ~FaceGluonData() override = default;

  void addGroundTruth(YAML::Node &, std::string) override;

  void addInputParam(YAML::Node &, std::string) override;

  void addGroundTruth(std::string filePath);
  void addInputParam(std::string filePath);

 public:
  std::vector<GluonONEPICTURE> features_;
};

class FaceGluonUnittest : public BaseAlgrithmUnittest {
 public:
  FaceGluonUnittest(std::string ymlGroundTruth, std::string ymlInput, std::string imgDir, std::string modelpath, arctern::Result *p);

  ~FaceGluonUnittest() override ;

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

//class FaceGluonGroundTruth {
// public:
//  FaceGluonGroundTruth(std::string groundTruth, std::string inputParam, std::string imgDir, std::string modelpath);
//
//  std::vector<GluonONEPICTURE> getGroundTruth(bool v3_3_5);
//
//  void getInputParam(std::vector<GluonONEPICTURE> &groundTruth);
//
//  void checkOnePicture(arctern::FaceGluon &gluon, GluonONEPICTURE picture);
//
//  void checkBatchPicture(arctern::FaceGluon &gluon, std::vector<GluonONEPICTURE> pictures);
//
//  void singleTest(bool v3_3_5 = false);
//
//  void batchTest(bool v3_3_5 = false);
//
// public:
//  std::string inputPath_;
//  std::string groundTruthPath_;  ///< groundtruth path
//  std::string imgDir_;  ///< prefix of image
//  std::string modelPath_;  ///< model path
//};
#endif  // UNITTEST_ALGORITHMS_FACEGLUON_GROUNDTRUTH_H_

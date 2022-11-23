/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.13
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_UNITTEST_UNITTESTBASE_BASEALGRITHM_H_
#define ARCTERN_UNITTEST_UNITTESTBASE_BASEALGRITHM_H_

#include <fstream>
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "include/algorithminterface.h"
#include "baseGroundTruth.h"
#include "include/result.h"
class BaseAlgrithmUnittest {
 public:
  BaseAlgrithmUnittest(std::string ymlGroundTruth, std::string ymlInputPath, std::string imgDir, std::string modelPath,
                       arctern::Result *result);
  virtual ~BaseAlgrithmUnittest();
  void test();

 protected:
  virtual std::shared_ptr<arctern::AlgorithmInterface> createAlgHandle(int batch) = 0;
  virtual std::shared_ptr<BaseGroundTruthData> generateGroundTruth() = 0;
  virtual std::shared_ptr<arctern::RunParameter> buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                   int startId,
                                                                   int len) = 0;

  virtual void checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                           int startId,
                           int len,
                           const arctern::Result *) = 0;

  virtual void checkPictures(std::shared_ptr<arctern::AlgorithmInterface>,
                     std::shared_ptr<BaseGroundTruthData> gtData,
                     int startId,
                     int len);

 private:

  void batchTest(const std::shared_ptr<BaseGroundTruthData> groundtruths);

  void singleTest(const std::shared_ptr<BaseGroundTruthData> groundtruths);
 public:
  arctern::Result *result_;
  std::string ymlGroundTruthPath_;  ///< groundtruth path
  std::string ymlInputPath_;        ///< groundtruth input path
  std::string imgDir_;  ///< prefix of image
  std::string modelPath_;  ///< model path
  bool grayMat_ = false;  ///<
  fstream fout_;
};

#endif //ARCTERN_UNITTEST_UNITTESTBASE_BASEALGRITHM_H_

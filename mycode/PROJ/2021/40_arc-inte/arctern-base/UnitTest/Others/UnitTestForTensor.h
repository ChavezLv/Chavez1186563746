/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020/12/17
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_BASE_UNITTEST_OTHERS_UNITTESTFORTENSOR_H_
#define ARCTERN_BASE_UNITTEST_OTHERS_UNITTESTFORTENSOR_H_
#include <vector>
#include <opencv2/opencv.hpp>
namespace arctern {
class UnitTestForTensor {
 public:
  UnitTestForTensor();
  ~UnitTestForTensor();

  int batchNum_ = 3;
  int sequenceNum_ = 4;
  int h_ = 6;
  int w_ = 8;
  int channel_ = 3;
  std::vector<cv::Mat> images;
  float* oriValue = nullptr;
  float* gtValue = nullptr;
  int gtValueSize = 0;
  int oriValueSize = 0;
};
}  // namespace arctern
#endif  // ARCTERN_BASE_UNITTEST_OTHERS_UNITTESTFORTENSOR_H_

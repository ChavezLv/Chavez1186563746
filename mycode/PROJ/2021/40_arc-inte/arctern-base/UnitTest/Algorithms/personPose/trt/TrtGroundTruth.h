//
// Created by chengaohua on 2021/7/7.
//

#ifndef ARCTERN_BASE_UNITTEST_ALGORITHMS_PERSONPOSE_TRT_TRTGROUNDTRUTH_H_
#define ARCTERN_BASE_UNITTEST_ALGORITHMS_PERSONPOSE_TRT_TRTGROUNDTRUTH_H_

#include "../groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"

#include "UnitTest/UnitTestBase/trt/TrtUtils.h"

class TrtPersonPoseUnittest : public personPoseUnittest  {
 public:
  TrtPersonPoseUnittest(const TrtTestParam& parameter);
  ~TrtPersonPoseUnittest();

  void checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                     std::shared_ptr<BaseGroundTruthData> gtData,
                     int startId,
                     int len) override;

  shared_ptr<arctern::AlgorithmInterface> createAlgHandle(int batch) override;

  int m_gpuId = 0;
};

#endif //ARCTERN_BASE_UNITTEST_ALGORITHMS_PERSONPOSE_TRT_TRTGROUNDTRUTH_H_

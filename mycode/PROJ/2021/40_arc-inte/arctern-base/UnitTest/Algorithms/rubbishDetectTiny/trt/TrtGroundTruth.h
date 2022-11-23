//
// Created by lvchaolin on 2021/12/8.
//

#ifndef __ARCTERN_BASE_UNITTEST_RUBBISHDETECTTINY_TRT_TRTGROUNDTRUTH_H__
#define __ARCTERN_BASE_UNITTEST_RUBBISHDETECTTINY_TRT_TRTGROUNDTRUTH_H__

#include "../groundtruth.h"

#include "gtest/gtest.h"
#include "include/autoTime.h"

#include "UnitTest/UnitTestBase/trt/TrtUtils.h"

class TrtRubbishDetectTinyUnittest : public RubbishDetectTinyUnittest  {
 public:
  TrtRubbishDetectTinyUnittest(const TrtTestParam& parameter);
  ~TrtRubbishDetectTinyUnittest();

  void checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                     std::shared_ptr<BaseGroundTruthData> gtData,
                     int startId,
                     int len) override;

  shared_ptr<arctern::AlgorithmInterface> createAlgHandle(int batch) override;

  int m_gpuId = 0;
};

#endif //end of __ARCTERN_BASE_UNITTEST_RUBBISHDETECTTINY_TRT_TRTGROUNDTRUTH_H__

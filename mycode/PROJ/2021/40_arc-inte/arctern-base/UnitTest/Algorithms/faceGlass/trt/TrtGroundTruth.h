//
// Created by Admin on 2021/6/18.
//

#ifndef ARCTERN_BASE_UNITTEST_ALGORITHMS_FACEGLASS_TRT_TRTGROUNDTRUTH_H_
#define ARCTERN_BASE_UNITTEST_ALGORITHMS_FACEGLASS_TRT_TRTGROUNDTRUTH_H_
#include "../groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "UnitTest/UnitTestBase/trt/TrtUtils.h"

class TrtFaceGlassUnittest : public FaceGlassUnittest  {
 public:
  TrtFaceGlassUnittest(const TrtTestParam& parameter);
  ~TrtFaceGlassUnittest();

  void checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                     std::shared_ptr<BaseGroundTruthData> gtData,
                     int startId,
                     int len) override;

  shared_ptr<arctern::AlgorithmInterface> createAlgHandle(int batch) override;

  int m_gpuId = 0;
};
#endif //ARCTERN_BASE_UNITTEST_ALGORITHMS_FACEGLASS_TRT_TRTGROUNDTRUTH_H_

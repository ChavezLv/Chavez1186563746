//
// Created by Admin on 2019/6/11.
//

#ifndef ARCTERN_BASE_UNITTEST_ALGORITHMS_RIVALFLOAT_ATLAS_ATLASGROUNDTRUTH_H_
#define ARCTERN_BASE_UNITTEST_ALGORITHMS_RIVALFLOAT_ATLAS_ATLASGROUNDTRUTH_H_
#include "../groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "UnitTest/UnitTestBase/atlas/AtlasUtils.h"

class AtlasRiverFloatUnittest : public RiverFloatUnittest  {
 public:
  AtlasRiverFloatUnittest(const AtlasTestParam& parameter);
  ~AtlasRiverFloatUnittest();

  void checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                     std::shared_ptr<BaseGroundTruthData> gtData,
                     int startId,
                     int len) override;

  shared_ptr<arctern::AlgorithmInterface> createAlgHandle(int batch) override;

  int m_gpuId = 0;
  std::string m_modelsPath8; ///< for atlas models b8
};
#endif //ARCTERN_BASE_UNITTEST_ALGORITHMS_RiverFloat_ATLAS_ATLASGROUNDTRUTH_H_

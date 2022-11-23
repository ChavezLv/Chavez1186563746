//
// Created by Admin on 2019/6/11.
//

#ifndef _UNITTEST_ALGORITHMS_POLICESUIT_TRT_TRTGROUNDTRUTH_H_
#define _UNITTEST_ALGORITHMS_POLICESUIT_TRT_TRTGROUNDTRUTH_H_
#include "../groundtruth.h"
#include "include/autoTime.h"
#include "gtest/gtest.h"

struct TrtPoliceSuitTestParam {
  string ymlGroundTruth;
  string ymlInput;
  string imgPath;
  string modelPath;
  string irModelPath;
  arctern::Result *result;
  int gpuIndex;
};

class TrtPoliceSuitUnittest : public PoliceSuitUnittest {
public:
  TrtPoliceSuitUnittest(const TrtPoliceSuitTestParam &parameter);
  ~TrtPoliceSuitUnittest();

  void checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                     std::shared_ptr<BaseGroundTruthData> gtData, int startId,
                     int len) override;

  shared_ptr<arctern::AlgorithmInterface> createAlgHandle(int batch) override;

  int m_gpuId = 0;
};
#endif

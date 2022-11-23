//
// Created by Admin on 2019/6/11.
//

#ifndef _SRC_ALGORITHMS_TRTOBJECTQUALITY_TRT_TRTFACEBANGSIMPL_H_
#define _SRC_ALGORITHMS_TRTOBJECTQUALITY_TRT_TRTFACEBANGSIMPL_H_
#include "../groundtruth.h"
#include "include/autoTime.h"
#include "gtest/gtest.h"

struct TrtObjQualityTestParam {
  string ymlGroundTruth;
  string ymlInput;
  string imgPath;
  string modelPath;
  string irModelPath;
  arctern::Result *result;
  int gpuIndex;
};

class TrtObjectQualityUnittest : public ObjectQualityUnittest {
public:
  TrtObjectQualityUnittest(const TrtObjQualityTestParam &parameter);
  ~TrtObjectQualityUnittest();

  void checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                     std::shared_ptr<BaseGroundTruthData> gtData, int startId,
                     int len) override;

  shared_ptr<arctern::AlgorithmInterface> createAlgHandle(int batch) override;

  int m_gpuId = 0;
};
#endif

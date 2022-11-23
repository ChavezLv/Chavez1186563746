//
// Created by Admin on 2019/6/11.
//

#ifndef _UNITTEST_ALGORITHMS_FACEIDENTIFY_TRT_TRTGROUNDTRUTH_H_
#define _UNITTEST_ALGORITHMS_FACEIDENTIFY_TRT_TRTGROUNDTRUTH_H_
#include "../groundtruth.h"
#include "include/autoTime.h"
#include "gtest/gtest.h"

struct TrtFaceIdentifyParam {
  string ymlGroundTruth;
  string ymlInput;
  string imgPath;
  string modelPath;
  string irModelPath;
  arctern::Result *result;
  int gpuIndex;
};

class TrtFaceIdentifyUnittest : public FaceIdentifyUnittest {
public:
  TrtFaceIdentifyUnittest(const TrtFaceIdentifyParam &parameter);
  ~TrtFaceIdentifyUnittest();

  void checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                     std::shared_ptr<BaseGroundTruthData> gtData, int startId,
                     int len) override;

  shared_ptr<arctern::AlgorithmInterface> createAlgHandle(int batch) override;

  int m_gpuId = 0;
};
#endif
//
// Created by Admin on 2019/6/11.
//

#ifndef TRT_UNITTEST_ALGORITHMS_VEHICLECYCLEFEAT_TRTGROUNDTRUTH_H_
#define TRT_UNITTEST_ALGORITHMS_VEHICLECYCLEFEAT_TRTGROUNDTRUTH_H_
#include "../groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"

struct TrtVehicleAttrParam {
  string ymlGroundTruth;
  string ymlInput;
  string imgPath;
  string modelPath;
  string irModelPath;
  int gpuIndex;
  arctern::Result *result;
};

class TrtVehicleCycleFeatUnittest : public VehicleCycleFeatUnittest {
 public:
  TrtVehicleCycleFeatUnittest(const TrtVehicleAttrParam &parameter);
  ~TrtVehicleCycleFeatUnittest();

  void checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                     std::shared_ptr<BaseGroundTruthData> gtData,
                     int startId,
                     int len) override;

  shared_ptr<arctern::AlgorithmInterface> createAlgHandle(int batch) override;

  int m_gpuId = 0;
};
#endif

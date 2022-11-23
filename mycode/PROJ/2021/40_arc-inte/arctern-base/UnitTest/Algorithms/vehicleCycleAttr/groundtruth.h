/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.09
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#ifndef UNITTEST_ALGORITHMS_VEHICLECYCLEATTR_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_VEHICLECYCLEATTR_GROUNDTRUTH_H_

#include "src/Algorithms/vehicleCycleAttr/vehicleCycleAttr.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"
using namespace arctern;

typedef struct {
  arctern::ArcternRect rect;
  arctern::SingleVehicleCycleAttr attrs;
} ONEPICTURE;

class VehicleCycleAttrData : public BaseGroundTruthData {
 public:
  VehicleCycleAttrData() = default;
  ~VehicleCycleAttrData() override = default;

  void addGroundTruth(YAML::Node &, std::string) override;

  void addInputParam(YAML::Node &, std::string) override;

 public:
  std::vector<ONEPICTURE> vehicleCycleAttrs_;
  const std::vector<std::string> vehicle_cycle_attrsName = {
    "类别","是否安装遮阳伞/车篷","驾驶状态","是否载人","方向"
  };
  const std::vector<std::string> vehicle_cycle_category = {
    "自行车","摩托车/电动车","三轮车","其他"
  };
  const std::vector<std::string> vehicle_cycle_orientation = {
    "前","后","其他"
  };
  int attrString2int(const std::string &attrname, const std::string &attrtype);
  const std::vector<std::pair<std::string, std::vector<std::string>>> attrs_map = {
    {vehicle_cycle_attrsName[0], vehicle_cycle_category   },
    {vehicle_cycle_attrsName[1], {"no",     "yes"}        },
    {vehicle_cycle_attrsName[2], {"no",     "yes"}        },
    {vehicle_cycle_attrsName[3], {"no",     "yes"}        },
    {vehicle_cycle_attrsName[4], vehicle_cycle_orientation},
  };
};

class VehicleCycleAttrUnittest : public BaseAlgrithmUnittest {
 public:
  VehicleCycleAttrUnittest(std::string ymlGroundTruth, std::string ymlInput, std::string imgDir, std::string modelpath, arctern::Result *p);

  ~VehicleCycleAttrUnittest() override ;

  std::shared_ptr<arctern::AlgorithmInterface> createAlgHandle(int batch) override;

  std::shared_ptr<arctern::RunParameter> buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                           int startId,
                                                           int len) override;

  std::shared_ptr<BaseGroundTruthData> generateGroundTruth() override;

  void checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                   int startId,
                   int len,
                   const arctern::Result *p) override;
};

#endif  // UNITTEST_ALGORITHMS_VEHICLECYCLEATTR_GROUNDTRUTH_H_


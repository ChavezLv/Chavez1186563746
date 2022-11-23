/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.08
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "yaml-cpp/yaml.h"
#include "log/logger.h"
#include "yaml-cpp/node/parse.h"

void VehicleCycleAttrData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto attrtype  = node[1]["VALUE"][0].as<std::vector<std::string>>();
    auto scores = node[2]["VALUE"][0].as<std::vector<float>>();
    
    for (int i = 0; i < 5*2; i += 2) {
      int attrTypeId = attrString2int(attrtype.at(i), attrtype.at(i+1));
      auto &attr = onepicture.attrs;
      int id = i/2;
      std::pair<int, float> attr_res = {attrTypeId, scores.at(id)};
      switch (id) {
        case 0 : {
          attr.vehicleNoMotorType = VehicleNoMotorType(attr_res.first);
          attr.vehicleNoMotorTypeScore = attr_res.second;
          break;
        }
        case 1 : {
          attr.hasUmbrella = attr_res.first;
          attr.hasUmbrellaScore = attr_res.second;
          break;
        }
        case 2 : {
          attr.isCycling = attr_res.first;
          attr.isCyclingScore = attr_res.second;
          break;
        }
        case 3 : {
          attr.isMultiPerson = attr_res.first;
          attr.isMultiPersonScore = attr_res.second;
          break;
        }
        case 4 : {
          attr.orientationType = VehicleCycleOrientationType(attr_res.first);
          attr.orientationTypeScore = attr_res.second;
          break;
        }
        default: {
          break;
        }
      }
    }

    vehicleCycleAttrs_.push_back(onepicture);
    files_.push_back(filename);
  }
}

int VehicleCycleAttrData::attrString2int(const std::string &attrname, const std::string &attrtype) {
  int attr_id = 0, attrtype_id = 0;
  for (size_t i = 0; i < attrs_map.size(); ++i) {
    if (attrname == attrs_map.at(i).first) {
      attr_id = i;
      break;
    }
  }
  auto &attrtypesV = attrs_map.at(attr_id).second;
  for (size_t j = 0; j < attrtypesV.size(); ++j) {
    if (attrtype == attrtypesV.at(j)) {
      attrtype_id = j;
      break;
    }
  }
  return attrtype_id;
}

void VehicleCycleAttrData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();
#ifdef ATLAS_ALLOWED
    static int atlas_index=0;
    vehicleCycleAttrs_[atlas_index].rect.x = rect[0];
    vehicleCycleAttrs_[atlas_index].rect.y = rect[1];
    vehicleCycleAttrs_[atlas_index].rect.width = rect[2];
    vehicleCycleAttrs_[atlas_index++].rect.height = rect[3];
#else
    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        vehicleCycleAttrs_[i].rect.x = rect[0];
        vehicleCycleAttrs_[i].rect.y = rect[1];
        vehicleCycleAttrs_[i].rect.width = rect[2];
        vehicleCycleAttrs_[i].rect.height = rect[3];
      }
    }
#endif
  }
}

VehicleCycleAttrUnittest::VehicleCycleAttrUnittest(std::string
                                           ymlGroundTruth,
                                           std::string
                                           ymlInput,
                                           std::string
                                           imgDir,
                                           std::string
                                           modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {
  fout_ << "VehicleCycleAttr:imageDir imageName vehicleNoMotorType score hasUmbrella score isCycling score isMultiPerson score orientationType score"<<std::endl;
}

VehicleCycleAttrUnittest::~VehicleCycleAttrUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> VehicleCycleAttrUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::VehicleCycleAttr> ptr(new arctern::VehicleCycleAttr);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> VehicleCycleAttrUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                               int startId,
                                                                               int len) {
  std::shared_ptr<arctern::VehicleCycleAttrRunParameter> ptr(new arctern::VehicleCycleAttrRunParameter);
  std::shared_ptr<VehicleCycleAttrData> data = std::dynamic_pointer_cast<VehicleCycleAttrData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->vehicleCycleAttrs_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> VehicleCycleAttrUnittest::generateGroundTruth() {
  std::shared_ptr<VehicleCycleAttrData> ptr(new VehicleCycleAttrData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void VehicleCycleAttrUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                       int startId,
                                       int len,
                                       const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::VehicleCycleAttrResult *>(p);
  std::shared_ptr<VehicleCycleAttrData> data = std::dynamic_pointer_cast<VehicleCycleAttrData>(gdata);

  EXPECT_EQ(result->vehicleCycleAttrs_.size(), len);

  for (int i = 0; i < len; i++) {
    auto &attrRes  = result->vehicleCycleAttrs_[i];
    auto &attrData = data->vehicleCycleAttrs_[i + startId].attrs;
    fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
    fout_ << data->vehicleCycleAttrs_[i+startId].rect.x<<" "<<data->vehicleCycleAttrs_[i+startId].rect.y << " "
          << data->vehicleCycleAttrs_[i+startId].rect.width << " " <<data->vehicleCycleAttrs_[i+startId].rect.height<<" ";
    fout_ <<(int) attrRes.vehicleNoMotorType <<" " << attrRes.vehicleNoMotorTypeScore<<" ";
    fout_ <<(int) attrRes.hasUmbrella <<" " << attrRes.hasUmbrellaScore<<" ";
    fout_ <<(int) attrRes.isCycling <<" " << attrRes.isCyclingScore<<" ";
    fout_ <<(int) attrRes.isMultiPerson <<" " << attrRes.isMultiPersonScore<<" ";
    fout_ <<(int) attrRes.orientationType <<" " << attrRes.orientationTypeScore<<" ";

    EXPECT_EQ(attrRes.vehicleNoMotorType, attrData.vehicleNoMotorType);
    EXPECT_EQ(attrRes.hasUmbrella, attrData.hasUmbrella);
    EXPECT_EQ(attrRes.isCycling, attrData.isCycling);
    EXPECT_EQ(attrRes.isMultiPerson, attrData.isMultiPerson);
    EXPECT_EQ(attrRes.orientationType, attrData.orientationType);
    EXPECT_NEAR(attrRes.vehicleNoMotorTypeScore, attrData.vehicleNoMotorTypeScore, 0.01);
    EXPECT_NEAR(attrRes.hasUmbrellaScore, attrData.hasUmbrellaScore, 0.01);
    EXPECT_NEAR(attrRes.isCyclingScore, attrData.isCyclingScore, 0.01);
    EXPECT_NEAR(attrRes.isMultiPersonScore, attrData.isMultiPersonScore, 0.01);
    EXPECT_NEAR(attrRes.orientationTypeScore, attrData.orientationTypeScore, 0.01);
  }
}


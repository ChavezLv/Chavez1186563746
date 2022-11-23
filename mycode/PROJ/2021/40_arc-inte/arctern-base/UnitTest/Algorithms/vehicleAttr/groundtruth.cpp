/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.09
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "yaml-cpp/yaml.h"
#include "log/logger.h"
#include "yaml-cpp/node/parse.h"

void VehicleAttrData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto attrtype  = node[1]["VALUE"][0].as<std::vector<std::string>>();
    auto scores = node[2]["VALUE"][0].as<std::vector<float>>();
    if (attrtype.size() != 6) {
        std::cerr << "Error : The length of vehicle attributes' groundTruth is false! " << std::endl;
    }
    onepicture.attrs.vehicleBrand = attrtype.at(1);
    onepicture.attrs.vehicleColor = attrtype.at(3);
    onepicture.attrs.vehicleType = attrtype.at(5);
    onepicture.attrs.vehicleBrandScore = scores.at(0);
    onepicture.attrs.vehicleColorScore = scores.at(1);
    onepicture.attrs.vehicleTypeScore = scores.at(2);

    vehicleAttrs_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void VehicleAttrData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();
#ifdef ATLAS_ALLOWED
    static int atlas_index = 0;
    vehicleAttrs_[atlas_index].rect.x = rect[0];
    vehicleAttrs_[atlas_index].rect.y = rect[1];
    vehicleAttrs_[atlas_index].rect.width = rect[2];
    vehicleAttrs_[atlas_index++].rect.height = rect[3];
#else
    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        vehicleAttrs_[i].rect.x = rect[0];
        vehicleAttrs_[i].rect.y = rect[1];
        vehicleAttrs_[i].rect.width = rect[2];
        vehicleAttrs_[i].rect.height = rect[3];
      }
    }
#endif
  }
}

VehicleAttrUnittest::VehicleAttrUnittest(std::string
                                           ymlGroundTruth,
                                           std::string
                                           ymlInput,
                                           std::string
                                           imgDir,
                                           std::string
                                           modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {
  fout_ << "VehicleAttr:imageDir imagename vehicleBrand score vehicleColor score vehicleType score"<<std::endl;
}

VehicleAttrUnittest::~VehicleAttrUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> VehicleAttrUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::VehicleAttr> ptr(new arctern::VehicleAttr);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> VehicleAttrUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                               int startId,
                                                                               int len) {
  std::shared_ptr<arctern::VehicleAttrRunParameter> ptr(new arctern::VehicleAttrRunParameter);
  std::shared_ptr<VehicleAttrData> data = std::dynamic_pointer_cast<VehicleAttrData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->vehicleAttrs_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> VehicleAttrUnittest::generateGroundTruth() {
  std::shared_ptr<VehicleAttrData> ptr(new VehicleAttrData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void VehicleAttrUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                       int startId,
                                       int len,
                                       const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::VehicleAttrResult *>(p);
  std::shared_ptr<VehicleAttrData> data = std::dynamic_pointer_cast<VehicleAttrData>(gdata);

  EXPECT_EQ(result->vehicleAttrs_.size(), len);

  for (int i = 0; i < len; i++) {
    auto &attrRes  = result->vehicleAttrs_[i];
    auto &attrData = data->vehicleAttrs_[i + startId].attrs;
    fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
    fout_ << data->vehicleAttrs_[i+startId].rect.x<<" "<<data->vehicleAttrs_[i+startId].rect.y << " "
          << data->vehicleAttrs_[i+startId].rect.width << " " <<data->vehicleAttrs_[i+startId].rect.height<<" ";
    fout_ << attrRes.vehicleBrand <<" "<<attrRes.vehicleBrandScore<<" ";
    fout_ << attrRes.vehicleColor <<" "<<attrRes.vehicleColorScore<<" ";
    fout_ << attrRes.vehicleType <<" "<<attrRes.vehicleTypeScore<<" ";

    EXPECT_EQ(attrRes.vehicleBrand, attrData.vehicleBrand);
    EXPECT_EQ(attrRes.vehicleColor, attrData.vehicleColor);
    EXPECT_EQ(attrRes.vehicleType, attrData.vehicleType);
    EXPECT_NEAR(attrRes.vehicleBrandScore, attrRes.vehicleBrandScore, 0.01);
    EXPECT_NEAR(attrRes.vehicleColorScore, attrRes.vehicleColorScore, 0.01);
    EXPECT_NEAR(attrRes.vehicleTypeScore, attrRes.vehicleTypeScore, 0.01);
  }
}


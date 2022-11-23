/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.09
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "include/autoTime.h"
#include "log/logger.h"
#include "src/Algorithms/memoryDB/memoryDB.h"
#include "yaml-cpp/node/parse.h"
#include "yaml-cpp/yaml.h"
#include "gtest/gtest.h"

void VehicCycleFeatData::addGroundTruth(YAML::Node &node,
                                        std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto features = node[1]["VALUE"][0].as<std::vector<float>>();
    std::copy(features.begin(), features.end(),
              std::back_inserter(onepicture.feat));

    vehicleScores.push_back(onepicture);
    files_.push_back(filename);
  }
}

void VehicCycleFeatData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();
#ifdef ATLAS_ALLOWED
    static int atlas_index = 0;
    vehicleScores[atlas_index].rect.x = rect[0];
    vehicleScores[atlas_index].rect.y = rect[1];
    vehicleScores[atlas_index].rect.width = rect[2];
    vehicleScores[atlas_index++].rect.height = rect[3];
#else
    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        vehicleScores[i].rect.x = rect[0];
        vehicleScores[i].rect.y = rect[1];
        vehicleScores[i].rect.width = rect[2];
        vehicleScores[i].rect.height = rect[3];
      }
    }
#endif
  }
}

VehicleCycleFeatUnittest::VehicleCycleFeatUnittest(std::string ymlGroundTruth,
                                                   std::string ymlInput,
                                                   std::string imgDir,
                                                   std::string modelpath,
                                                   arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {
  fout_ << "VehicleCycleFeat:imageDir imageName rect feats"<<std::endl;
}

VehicleCycleFeatUnittest::~VehicleCycleFeatUnittest() {}

std::shared_ptr<arctern::AlgorithmInterface>
VehicleCycleFeatUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::VehicleCycleFeat> ptr(new arctern::VehicleCycleFeat);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter>
VehicleCycleFeatUnittest::buildRunParameter(
    std::shared_ptr<BaseGroundTruthData> gdata, int startId, int len) {
  std::shared_ptr<arctern::VehicleCycleFeatRunParameter> ptr(
      new arctern::VehicleCycleFeatRunParameter);
  std::shared_ptr<VehicCycleFeatData> data =
      std::dynamic_pointer_cast<VehicCycleFeatData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->vehicleScores[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData>
VehicleCycleFeatUnittest::generateGroundTruth() {
  std::shared_ptr<VehicCycleFeatData> ptr(new VehicCycleFeatData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void VehicleCycleFeatUnittest::checkResult(
    std::shared_ptr<BaseGroundTruthData> gdata, int startId, int len,
    const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::VehicleCycleFeatResult *>(p);
  std::shared_ptr<VehicCycleFeatData> data =
      std::dynamic_pointer_cast<VehicCycleFeatData>(gdata);

  EXPECT_EQ(result->m_vFeatResults.size(), len);

  for (int i = 0; i < len; i++) {
    fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
    fout_ << data->vehicleScores[i+startId].rect.x<<" "<<data->vehicleScores[i+startId].rect.y << " "
          <<data->vehicleScores[i+startId].rect.width << " " <<data->vehicleScores[i+startId].rect.height<<" ";
    for(auto &item : result->m_vFeatResults[i]){
      fout_ <<(int) item <<" ";
    }
    fout_ << std::endl;

    EXPECT_EQ(result->m_vFeatResults[i].size(),
              data->vehicleScores[i + startId].feat.size());
    auto score = arctern::compute_similarity(
        result->m_vFeatResults[i], data->vehicleScores[i + startId].feat);

    EXPECT_NEAR(score, 1, 0.01);
  }
}

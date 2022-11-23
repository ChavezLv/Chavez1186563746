/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.11.30
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "yaml-cpp/yaml.h"
#include "log/logger.h"
#include "common/featureUtil.h"
#include "yaml-cpp/node/parse.h"
#include "src/Algorithms/memoryDB/memoryDB.h"
#include "../../../src/Algorithms/personFeat/personFeat.h"

void PersonFeatData::addGroundTruth(YAML::Node &node, std::string filename) {
  PersonFeatONEPICTURE onepicture;
  if (node.size() > 1) {
    auto features = node[1]["VALUE"][0].as<std::vector<float>>();
    std::copy(features.begin(), features.end(), std::back_inserter(onepicture.features));

    features_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void PersonFeatData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[2]["VALUE"].as<std::vector<float>>();
#ifdef ATLAS_ALLOWED
    static int atlas_index = 0;
    features_[atlas_index].rect.x = rect[0];
    features_[atlas_index].rect.y = rect[1];
    features_[atlas_index].rect.width = rect[2];
    features_[atlas_index].rect.height = rect[3];
    atlas_index++;
#else
    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        features_[i].rect.x = rect[0];
        features_[i].rect.y = rect[1];
        features_[i].rect.width = rect[2];
        features_[i].rect.height = rect[3];
      }
    }
#endif
  }
}

PersonFeatUnittest::PersonFeatUnittest(std::string
                                     ymlGroundTruth,
                                     std::string
                                     ymlInput,
                                     std::string
                                     imgDir,
                                     std::string
                                     modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {
  fout_ << "PersonFeat:imageDir imageName rect personfeat "<< std::endl;
}

PersonFeatUnittest::~PersonFeatUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> PersonFeatUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::PersonFeat> ptr(new arctern::PersonFeat);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> PersonFeatUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                            int startId,
                                                                            int len) {
  std::shared_ptr<arctern::PersonFeatRunParameter> ptr(new arctern::PersonFeatRunParameter);
  std::shared_ptr<PersonFeatData> data = std::dynamic_pointer_cast<PersonFeatData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->features_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> PersonFeatUnittest::generateGroundTruth() {
  std::shared_ptr<PersonFeatData> ptr(new PersonFeatData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void PersonFeatUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                    int startId,
                                    int len,
                                    const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::PersonFeatResult *>(p);
  std::shared_ptr<PersonFeatData> data = std::dynamic_pointer_cast<PersonFeatData>(gdata);

  EXPECT_EQ(result->features_.size(), len);

  for (int i = 0; i < len; i++) {
    fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
    fout_ << data->features_[i+startId].rect.x<<" "<<data->features_[i+startId].rect.y << " "
          <<data->features_[i+startId].rect.width << " " <<data->features_[i+startId].rect.height<<" ";
    for(auto &item : result->features_[i]){
      fout_ <<(int) item <<" ";
    }
    fout_ << std::endl;

    EXPECT_EQ(result->features_[i].size(), data->features_[i  + startId].features.size());
    auto score = arctern::compute_similarity(result->features_[i], data->features_[i + startId].features);
    EXPECT_NEAR(score, 1, 0.01);
  }
}


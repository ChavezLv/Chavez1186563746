/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.21
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "yaml-cpp/yaml.h"
#include "log/logger.h"
#include "common/featureUtil.h"
#include "yaml-cpp/node/parse.h"
#include "common/encryx.h"
#include "../../../src/Algorithms/faceGluon/faceGluon.h"
#include "src/common/json.hpp"
#include "src/Algorithms/memoryDB/memoryDB.h"

using json = nlohmann::json;
void  FaceGluonData::addGroundTruth(std::string filePath){
  json parser;
  std::ifstream fin(filePath);
  fin >> parser;
  for(int i=0;i<parser.size();i++) {

    GluonONEPICTURE onepicture;

    auto feats = parser[i]["feat"].get<std::vector<float>>();
    arctern::l2norm(feats);
    arctern::encrypt_feature(feats,onepicture.features);

    files_.push_back(parser.at(i)["filepath"]);
    features_.push_back(onepicture);
  }
  fin.close();
}

void  FaceGluonData::addInputParam(std::string filePath){
  json parser;
  std::ifstream fin(filePath);
  fin >> parser;

  for(int i=0;i<parser.size();i++) {
    int len = files_.size();
    for (int j = 0; j < len; j++) {
      if (files_[j] == parser.at(i)["filepath"]) {
        auto rect = parser[i]["rect"].get<std::vector<int>>();
        features_[j].rect.x = rect[0];
        features_[j].rect.y = rect[1];
        features_[j].rect.width = rect[2];
        features_[j].rect.height = rect[3];
      }
    }
  }
  fin.close();
}

void FaceGluonData::addGroundTruth(YAML::Node &node, std::string filename) {
  GluonONEPICTURE onepicture;
  if (node.size() > 1) {
    auto features = node[1]["VALUE"][0].as<std::vector<float>>();
    std::copy(features.begin(), features.end(), std::back_inserter(onepicture.features));

    features_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void FaceGluonData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[2]["VALUE"].as<std::vector<float>>();
    std::vector<float> points;
    if (node.size() == 4) {
      points = node[3]["landmarks"][0][0].as<std::vector<float>>();
    }
#if ATLAS_ALLOWED
    static int atlas_index = 0;
    features_[atlas_index].rect.x = rect[0];
    features_[atlas_index].rect.y = rect[1];
    features_[atlas_index].rect.width = rect[2];
    features_[atlas_index].rect.height = rect[3];

    int pointsNum = points.size() / 2;
    for (int idx = 0; idx < pointsNum; idx++) {
      features_[atlas_index].landmarks.push_back({points[2 * idx], points[2 * idx + 1]});
    }
    atlas_index++;
#else

    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        features_[i].rect.x = rect[0];
        features_[i].rect.y = rect[1];
        features_[i].rect.width = rect[2];
        features_[i].rect.height = rect[3];

        int pointsNum = points.size() / 2;
        for (int idx = 0; idx < pointsNum; idx++) {
          features_[i].landmarks.push_back({points[2 * idx], points[2 * idx + 1]});
        }
      }
    }
#endif
  }
}

FaceGluonUnittest::FaceGluonUnittest(std::string
                                     ymlGroundTruth,
                                     std::string
                                     ymlInput,
                                     std::string
                                     imgDir,
                                     std::string
                                     modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {
  fout_ << "FaceGluon:imageDir imageName rect feats "<<std::endl;
}

FaceGluonUnittest::~FaceGluonUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> FaceGluonUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceGluon> ptr(new arctern::FaceGluon);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> FaceGluonUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                            int startId,
                                                                            int len) {
  std::shared_ptr<arctern::FaceGluonRunParameter> ptr(new arctern::FaceGluonRunParameter);
  std::shared_ptr<FaceGluonData> data = std::dynamic_pointer_cast<FaceGluonData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->features_[i + startId].rect);
    ptr->faceLandmarksV_.push_back(data->features_[i + startId].landmarks);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FaceGluonUnittest::generateGroundTruth() {
  std::shared_ptr<FaceGluonData> ptr(new FaceGluonData);

  auto pos = ymlGroundTruthPath_.find_last_of('.');
  auto suffix = ymlGroundTruthPath_.substr(pos+1);
  if(suffix == "json"){
    ptr->addGroundTruth(ymlGroundTruthPath_);
    ptr->addInputParam(ymlInputPath_);
  }
  else {

    BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
    groundTruth.getGroundTruth(ptr);
    groundTruth.getInputParam(ptr);
  }
  return ptr;
}

void FaceGluonUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                    int startId,
                                    int len,
                                    const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::FaceGluonResult *>(p);
  std::shared_ptr<FaceGluonData> data = std::dynamic_pointer_cast<FaceGluonData>(gdata);

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


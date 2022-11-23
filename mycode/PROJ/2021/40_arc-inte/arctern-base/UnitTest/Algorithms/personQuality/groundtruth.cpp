/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.03
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#include "./groundtruth.h"
#include "yaml-cpp/yaml.h"
#include "yaml-cpp/node/parse.h"
#include "gtest/gtest.h"

void PersonQualityData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
      auto score = node[1]["VALUE"].as<std::vector<float>>();
      onepicture.score = score[0];

    personQuality_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void PersonQualityData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();

    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        personQuality_[i].rect.x = rect[0];
        personQuality_[i].rect.y = rect[1];
        personQuality_[i].rect.width = rect[2];
        personQuality_[i].rect.height = rect[3];
      }
    }
  }
}

PersonQualityUnittest::PersonQualityUnittest(std::string
                                   ymlGroundTruth,
                                   std::string
                                   ymlInput,
                                   std::string
                                   imgDir,
                                   std::string
                                   modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {
  fout_ << "PersonQuality:imageDir imageName rect quality score"<<std::endl;
}

PersonQualityUnittest::~PersonQualityUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> PersonQualityUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::PersonQuality> ptr(new arctern::PersonQuality);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> PersonQualityUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                           int startId,
                                                                           int len) {
  std::shared_ptr<arctern::PersonQualityRunParameter> ptr(new arctern::PersonQualityRunParameter);
  std::shared_ptr<PersonQualityData> data = std::dynamic_pointer_cast<PersonQualityData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->personQuality_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> PersonQualityUnittest::generateGroundTruth() {
  std::shared_ptr<PersonQualityData> ptr(new PersonQualityData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void PersonQualityUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                   int startId,
                                   int len,
                                   const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::PersonQualityResult *>(p);
  std::shared_ptr<PersonQualityData> data = std::dynamic_pointer_cast<PersonQualityData>(gdata);

  EXPECT_EQ(static_cast<int>(result->personQuality_.size()), len);

  for (int i = 0; i < len; i++) {
    fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
    fout_ << data->personQuality_[i+startId].rect.x<<" "<<data->personQuality_[i+startId].rect.y << " "
          << data->personQuality_[i+startId].rect.width << " " <<data->personQuality_[i+startId].rect.height<<" ";
    fout_ << result->personQuality_[i] <<" " << result->personQuality_[i]<<std::endl;

    EXPECT_NEAR(result->personQuality_[i], data->personQuality_[i + startId].score, 0.01);
  }
}


/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.16
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#include "./groundtruth.h"
#include "yaml-cpp/yaml.h"
#include "yaml-cpp/node/parse.h"
#include "gtest/gtest.h"

void ObjectQualityData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
      auto score = node[1]["VALUE"].as<std::vector<float>>();
      onepicture.score = score[0];

    objectQuality_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void ObjectQualityData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();

    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        objectQuality_[i].rect.x = rect[0];
        objectQuality_[i].rect.y = rect[1];
        objectQuality_[i].rect.width = rect[2];
        objectQuality_[i].rect.height = rect[3];
      }
    }
  }
}

ObjectQualityUnittest::ObjectQualityUnittest(std::string
                                   ymlGroundTruth,
                                   std::string
                                   ymlInput,
                                   std::string
                                   imgDir,
                                   std::string
                                   modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {

}

ObjectQualityUnittest::~ObjectQualityUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> ObjectQualityUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::ObjectQuality> ptr(new arctern::ObjectQuality);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> ObjectQualityUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                           int startId,
                                                                           int len) {
  std::shared_ptr<arctern::ObjectQualityRunParameter> ptr(new arctern::ObjectQualityRunParameter);
  std::shared_ptr<ObjectQualityData> data = std::dynamic_pointer_cast<ObjectQualityData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->objectQuality_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> ObjectQualityUnittest::generateGroundTruth() {
  std::shared_ptr<ObjectQualityData> ptr(new ObjectQualityData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void ObjectQualityUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                   int startId,
                                   int len,
                                   const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::ObjectQualityResult *>(p);
  std::shared_ptr<ObjectQualityData> data = std::dynamic_pointer_cast<ObjectQualityData>(gdata);

  EXPECT_EQ(static_cast<int>(result->objectQuality_.size()), len);

  for (int i = 0; i < len; i++) {
    EXPECT_NEAR(result->objectQuality_[i], data->objectQuality_[i + startId].score, 0.01);
  }
}


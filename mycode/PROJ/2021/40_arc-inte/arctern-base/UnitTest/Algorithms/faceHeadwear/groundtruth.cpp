/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.25
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "yaml-cpp/yaml.h"
#include "log/logger.h"
#include "yaml-cpp/node/parse.h"

static arctern::HeadwearColor convertColor(std::string des) {
  if (des == "no") {
    return arctern::NOHEADWEARCOLOR;
  }
  if (des == "black") {
    return arctern::BLACK;
  }
  if (des == "white") {
    return arctern::WHITE;
  }
  if (des == "gray-brown") {
    return arctern::GRAY_BROWN;
  }
  if (des == "red") {
    return arctern::RED;
  }
  if (des == "pink") {
    return arctern::PINK;
  }
  if (des == "orange") {
    return arctern::ORANGE;
  }
  if (des == "blue") {
    return arctern::BLUE;
  }
  if (des == "yellow") {
    return arctern::YELLOW;
  }
  return arctern::OTHERCOLOR;
}

static arctern::HeadwearType convertType(std::string des) {
  if (des == "no") {
    return arctern::NOHEADWEARTYPE;
  }
  if (des == "hat") {
    return arctern::HAT;
  }
  if (des == "builder") {
    return arctern::BUILDER;
  }
  if (des == "sport") {
    return arctern::SPORT;
  }
  if (des == "cycling") {
    return arctern::CYCLING;
  }
  return arctern::OTHERTYPE;
}

void FaceHeadwearData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto color = node[1]["VALUE"][0][0].as<std::string>();
    auto type = node[1]["VALUE"][0][1].as<std::string>();
    auto score = node[2]["VALUE"][0].as<std::vector<float>>();
    onepicture.headwear.colorScore = score[0];
    onepicture.headwear.typeScore = score[1];
    onepicture.headwear.type = convertType(type);
    onepicture.headwear.color = convertColor(color);

    faceHeadwear_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void FaceHeadwearData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();

    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        faceHeadwear_[i].rect.x = rect[0];
        faceHeadwear_[i].rect.y = rect[1];
        faceHeadwear_[i].rect.width = rect[2];
        faceHeadwear_[i].rect.height = rect[3];
      }
    }
  }
}

FaceHeadwearUnittest::FaceHeadwearUnittest(std::string
                                           ymlGroundTruth,
                                           std::string
                                           ymlInput,
                                           std::string
                                           imgDir,
                                           std::string
                                           modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {

}

FaceHeadwearUnittest::~FaceHeadwearUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> FaceHeadwearUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceHeadwear> ptr(new arctern::FaceHeadwear);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> FaceHeadwearUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                               int startId,
                                                                               int len) {
  std::shared_ptr<arctern::FaceHeadwearRunParameter> ptr(new arctern::FaceHeadwearRunParameter);
  std::shared_ptr<FaceHeadwearData> data = std::dynamic_pointer_cast<FaceHeadwearData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->faceHeadwear_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FaceHeadwearUnittest::generateGroundTruth() {
  std::shared_ptr<FaceHeadwearData> ptr(new FaceHeadwearData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void FaceHeadwearUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                       int startId,
                                       int len,
                                       const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::FaceHeadwearResult *>(p);
  std::shared_ptr<FaceHeadwearData> data = std::dynamic_pointer_cast<FaceHeadwearData>(gdata);

  EXPECT_EQ(result->faceHeadwears_.size(), len);

  for (int i = 0; i < len; i++) {
    EXPECT_EQ(result->faceHeadwears_[i].type, data->faceHeadwear_[i + startId].headwear.type);
    EXPECT_EQ(result->faceHeadwears_[i].color, data->faceHeadwear_[i + startId].headwear.color);
    EXPECT_NEAR(result->faceHeadwears_[i].typeScore, data->faceHeadwear_[i + startId].headwear.typeScore, 0.01);
    EXPECT_NEAR(result->faceHeadwears_[i].colorScore, data->faceHeadwear_[i + startId].headwear.colorScore, 0.01);
  }
}


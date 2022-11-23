/****************************************************************************
 *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         kuanghongshen
 *  Last modified:  2021.06.07
 *  email:          kuang.hongshen@intellif.com
 ****************************************************************************/
#include "./groundtruth.h"
#include "yaml-cpp/node/parse.h"
#include "yaml-cpp/yaml.h"
#include "gtest/gtest.h"
#include <iostream>

using arctern::ArcternRect;
using arctern::ErrorType;
using arctern::PoliceSuitResult;
using arctern::Result;

PoliceSuitData::PoliceSuitData() {}

PoliceSuitData::~PoliceSuitData() {}

void PoliceSuitData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto type = node[1]["VALUE"][0].as<std::vector<int>>();
    onepicture.result.first = arctern::PoliceSuitClassify(type[0]);

    auto score = node[2]["VALUE"][0].as<std::vector<float>>();
    onepicture.result.second = score[0];

    suitResults.push_back(onepicture);
    files_.push_back(filename);
  }
}

void PoliceSuitData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();

    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        suitResults[i].rect.x = rect[0];
        suitResults[i].rect.y = rect[1];
        suitResults[i].rect.width = rect[2];
        suitResults[i].rect.height = rect[3];
      }
    }
  }
}

PoliceSuitUnittest::PoliceSuitUnittest(std::string ymlGroundTruth,
                                       std::string ymlInput, std::string imgDir,
                                       std::string modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {}

PoliceSuitUnittest::~PoliceSuitUnittest() {}

std::shared_ptr<arctern::AlgorithmInterface>
PoliceSuitUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::PoliceSuit> ptr(new arctern::PoliceSuit);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> PoliceSuitUnittest::buildRunParameter(
    std::shared_ptr<BaseGroundTruthData> gdata, int startId, int len) {
  std::shared_ptr<arctern::PoliceSuitRunParameter> ptr(
      new arctern::PoliceSuitRunParameter);
  std::shared_ptr<PoliceSuitData> data =
      std::dynamic_pointer_cast<PoliceSuitData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->suitResults[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> PoliceSuitUnittest::generateGroundTruth() {
  std::shared_ptr<PoliceSuitData> ptr(new PoliceSuitData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void PoliceSuitUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                     int startId, int len,
                                     const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::PoliceSuitResult *>(p);
  std::shared_ptr<PoliceSuitData> data =
      std::dynamic_pointer_cast<PoliceSuitData>(gdata);

  EXPECT_EQ(result->m_vSuitResults.size(), len);

  for (int i = 0; i < len; i++) {
    EXPECT_EQ(result->m_vSuitResults[i].first,
              data->suitResults[i + startId].result.first);
    EXPECT_NEAR(result->m_vSuitResults[i].second,
                data->suitResults[i + startId].result.second, 0.01);
  }
}

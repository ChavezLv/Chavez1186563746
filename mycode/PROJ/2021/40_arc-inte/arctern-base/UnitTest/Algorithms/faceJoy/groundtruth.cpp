/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         kuanghongshen
 *  Last modified:  2021.06.01
 *  email:          kuang.hongshen@intellif.com
 ****************************************************************************/
#include "groundtruth.h"
#include "yaml-cpp/node/parse.h"
#include "yaml-cpp/yaml.h"
#include "gtest/gtest.h"
#include <iostream>

using arctern::ArcternRect;
using arctern::ErrorType;
using arctern::Result;

FaceJoyData::FaceJoyData() {}

FaceJoyData::~FaceJoyData() {}

void FaceJoyData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto result = node[1]["VALUE"][0].as<std::vector<float>>();

    onepicture.result.emplace_back(
        std::make_pair(static_cast<int>(result[0]), result[1]));

    m_vLandmarks.push_back(onepicture);
    files_.push_back(filename);
  }
}

void FaceJoyData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<int>>();
    auto landmarks = node[2]["VALUE"][0].as<std::vector<float>>();

    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        m_vLandmarks[i].rect.x = rect[0];
        m_vLandmarks[i].rect.y = rect[1];
        m_vLandmarks[i].rect.width = rect[2];
        m_vLandmarks[i].rect.height = rect[3];

        m_vLandmarks[i].landmarks = landmarks;
      }
    }
  }
}

FaceJoyUnittest::FaceJoyUnittest(std::string ymlGroundTruth,
                                 std::string ymlInput, std::string imgDir,
                                 std::string modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {}

FaceJoyUnittest::~FaceJoyUnittest() {}

std::shared_ptr<arctern::AlgorithmInterface>
FaceJoyUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceJoy> ptr(new arctern::FaceJoy);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter>
FaceJoyUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                   int startId, int len) {
  std::shared_ptr<arctern::FaceJoyRunParameter> ptr(
      new arctern::FaceJoyRunParameter);
  std::shared_ptr<FaceJoyData> data =
      std::dynamic_pointer_cast<FaceJoyData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->m_vLandmarks[i + startId].rect);
    ptr->faceLandmarksV_.push_back(data->m_vLandmarks[i + startId].landmarks);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FaceJoyUnittest::generateGroundTruth() {
  std::shared_ptr<FaceJoyData> ptr(new FaceJoyData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void FaceJoyUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                  int startId, int len,
                                  const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::FaceJoyResult *>(p);
  std::shared_ptr<FaceJoyData> data =
      std::dynamic_pointer_cast<FaceJoyData>(gdata);

  EXPECT_EQ(result->m_vJoyResults.size(), len);

  for (int i = 0; i < len; i++) {
    std::cout << "result first = " << result->m_vJoyResults[i][0].first
              << " data first = "
              << data->m_vLandmarks[i + startId].result[0].first << std::endl;
    std::cout << "result second = " << result->m_vJoyResults[i][0].second
              << " data second = "
              << data->m_vLandmarks[i + startId].result[0].second << std::endl;
    EXPECT_EQ(result->m_vJoyResults[i][0].first,
              data->m_vLandmarks[i + startId].result[0].first);
    EXPECT_NEAR(result->m_vJoyResults[i][0].second,
                data->m_vLandmarks[i + startId].result[0].second, 0.01);
  }
}

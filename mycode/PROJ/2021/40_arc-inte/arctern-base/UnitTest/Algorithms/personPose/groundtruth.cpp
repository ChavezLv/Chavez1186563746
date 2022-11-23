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
#include "yaml-cpp/node/parse.h"
#include "yaml-cpp/yaml.h"
#include "gtest/gtest.h"
#include <typeinfo>

void personPoseData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto score = node[1]["VALUE"][0].as<std::vector<float>>();

    //todo just test one rect in picture
    for (int i = 0; i < 17 * 3; i += 3) {
      std::pair<ArcternPoint , float> one_img =
          std::make_pair(ArcternPoint {score.at(i),
                                   score.at(i + 1)},
                         score.at(i + 2));

      onepicture.result.push_back(one_img);
    }

    m_vResults.push_back(onepicture);

    files_.push_back(filename);
  }
}

void personPoseData::addInputParam(YAML::Node &node, std::string filename) {
  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();

    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        m_vResults[i].rect.x = rect[0];
        m_vResults[i].rect.y = rect[1];
        m_vResults[i].rect.width = rect[2];
        m_vResults[i].rect.height = rect[3];
      }
    }
  }
}

personPoseUnittest::personPoseUnittest(std::string ymlGroundTruth,
                                       std::string ymlInput, std::string imgDir,
                                       std::string modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {}

personPoseUnittest::~personPoseUnittest() {}

std::shared_ptr<arctern::AlgorithmInterface>
personPoseUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::PersonPose> ptr(new arctern::PersonPose);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> personPoseUnittest::buildRunParameter(
    std::shared_ptr<BaseGroundTruthData> gdata, int startId, int len) {
  std::shared_ptr<arctern::PersonPoseRunParameter> ptr(
      new arctern::PersonPoseRunParameter);
  std::shared_ptr<personPoseData> data =
      std::dynamic_pointer_cast<personPoseData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->m_vResults[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> personPoseUnittest::generateGroundTruth() {
  std::shared_ptr<personPoseData> ptr(new personPoseData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void personPoseUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                     int startId, int len,
                                     const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::PersonPoseResult *>(p);
  std::shared_ptr<personPoseData> data =
      std::dynamic_pointer_cast<personPoseData>(gdata);

  EXPECT_EQ(result->results_.size(), len);

  for (int i = 0; i < len; i++) {
    auto &classRes = result->results_[i];
    auto &calssData = data->m_vResults[i + startId].result;

    EXPECT_EQ(classRes.size(), calssData.size());

    for (int j = 0; j < classRes.size(); j++) {
     // std::cout<<result->results_[i][j].first.x<<" "<<result->results_[i][j].first.y<<result->results_[i][j].second<<std::endl;
      EXPECT_NEAR(result->results_[i][j].second,
                  data->m_vResults[i + startId].result[j].second, 0.01);
      EXPECT_NEAR(result->results_[i][j].first.x,
                  data->m_vResults[i + startId].result[j].first.x, 2);
      EXPECT_NEAR(result->results_[i][j].first.y,
                  data->m_vResults[i + startId].result[j].first.y, 2);
    }

  }
}

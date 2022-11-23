/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.22
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/


#include "./groundtruth.h"
#include "yaml-cpp/yaml.h"
#include "yaml-cpp/node/parse.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "log/logger.h"

void FaceLivenessRgbData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto score = node[1]["VALUE"].as<std::vector<float>>();
    /// todo
    onepicture.score = score[0];

    faceLivenessRgb_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void FaceLivenessRgbData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();

    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        faceLivenessRgb_[i].rect.x = rect[0];
        faceLivenessRgb_[i].rect.y = rect[1];
        faceLivenessRgb_[i].rect.width = rect[2];
        faceLivenessRgb_[i].rect.height = rect[3];
      }
    }
  }
}

FaceLivenessRgbUnittest::FaceLivenessRgbUnittest(std::string
                                               ymlGroundTruth,
                                               std::string
                                               ymlInput,
                                               std::string
                                               imgDir,
                                               std::string
                                               modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {

}

FaceLivenessRgbUnittest::~FaceLivenessRgbUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> FaceLivenessRgbUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceLivenessRgb> ptr(new arctern::FaceLivenessRgb);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> FaceLivenessRgbUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                                 int startId,
                                                                                 int len) {
  std::shared_ptr<arctern::FaceLivenessRgbRunParameter> ptr(new arctern::FaceLivenessRgbRunParameter);
  std::shared_ptr<FaceLivenessRgbData> data = std::dynamic_pointer_cast<FaceLivenessRgbData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->faceLivenessRgb_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FaceLivenessRgbUnittest::generateGroundTruth() {
  std::shared_ptr<FaceLivenessRgbData> ptr(new FaceLivenessRgbData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void FaceLivenessRgbUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                         int startId,
                                         int len,
                                         const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::FaceLivenessRgbResult *>(p);
  std::shared_ptr<FaceLivenessRgbData> data = std::dynamic_pointer_cast<FaceLivenessRgbData>(gdata);

  EXPECT_EQ(result->faceLiveness_.size(), len);

  for (int i = 0; i < len; i++) {
    EXPECT_NEAR(result->faceLiveness_[i], data->faceLivenessRgb_[i + startId].score, 0.01);
  }
}

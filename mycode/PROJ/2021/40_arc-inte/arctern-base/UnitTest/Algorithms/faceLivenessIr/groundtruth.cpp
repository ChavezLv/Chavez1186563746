/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.15
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/


#include "./groundtruth.h"
#include "yaml-cpp/yaml.h"
#include "yaml-cpp/node/parse.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "log/logger.h"

void FaceLivenessIrData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
      auto score = node[1]["VALUE"].as<std::vector<float>>();
      /// todo
      onepicture.score = score[0];

    faceLivenessIr_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void FaceLivenessIrData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();

    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        faceLivenessIr_[i].rect.x = rect[0];
        faceLivenessIr_[i].rect.y = rect[1];
        faceLivenessIr_[i].rect.width = rect[2];
        faceLivenessIr_[i].rect.height = rect[3];
      }
    }
  }
}

FaceLivenessIrUnittest::FaceLivenessIrUnittest(std::string
                                           ymlGroundTruth,
                                           std::string
                                           ymlInput,
                                           std::string
                                           imgDir,
                                           std::string
                                           modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {

}

FaceLivenessIrUnittest::~FaceLivenessIrUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> FaceLivenessIrUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceLivenessIr> ptr(new arctern::FaceLivenessIr);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> FaceLivenessIrUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                               int startId,
                                                                               int len) {
  std::shared_ptr<arctern::FaceLivenessIrRunParameter> ptr(new arctern::FaceLivenessIrRunParameter);
  std::shared_ptr<FaceLivenessIrData> data = std::dynamic_pointer_cast<FaceLivenessIrData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->faceLivenessIr_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FaceLivenessIrUnittest::generateGroundTruth() {
  std::shared_ptr<FaceLivenessIrData> ptr(new FaceLivenessIrData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void FaceLivenessIrUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                       int startId,
                                       int len,
                                       const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::FaceLivenessIrResult *>(p);
  std::shared_ptr<FaceLivenessIrData> data = std::dynamic_pointer_cast<FaceLivenessIrData>(gdata);

  EXPECT_EQ(static_cast<int>(result->faceLiveness_.size()), len);

  for (int i = 0; i < len; i++) {
    EXPECT_NEAR(result->faceLiveness_[i], data->faceLivenessIr_[i + startId].score, 0.01);

  }
}

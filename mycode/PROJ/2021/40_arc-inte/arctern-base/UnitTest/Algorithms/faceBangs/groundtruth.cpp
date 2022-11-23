/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.24
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "yaml-cpp/yaml.h"
#include "log/logger.h"
#include "yaml-cpp/node/parse.h"

void FaceBangsData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto type = node[1]["VALUE"].as<std::vector<int>>();
    onepicture.bangs.status = arctern::BangsStatus(type[0]);

    auto score = node[2]["VALUE"].as<std::vector<float>>();
    onepicture.bangs.statusScore = score[0];

    bangs_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void FaceBangsData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();

    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        bangs_[i].rect.x = rect[0];
        bangs_[i].rect.y = rect[1];
        bangs_[i].rect.width = rect[2];
        bangs_[i].rect.height = rect[3];
      }
    }
  }
}

FaceBangsUnittest::FaceBangsUnittest(std::string
                                     ymlGroundTruth,
                                     std::string
                                     ymlInput,
                                     std::string
                                     imgDir,
                                     std::string
                                     modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {

}

FaceBangsUnittest::~FaceBangsUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> FaceBangsUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceBangs> ptr(new arctern::FaceBangs);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> FaceBangsUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                            int startId,
                                                                            int len) {
  std::shared_ptr<arctern::FaceBangsRunParameter> ptr(new arctern::FaceBangsRunParameter);
  std::shared_ptr<FaceBangsData> data = std::dynamic_pointer_cast<FaceBangsData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->bangs_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FaceBangsUnittest::generateGroundTruth() {
  std::shared_ptr<FaceBangsData> ptr(new FaceBangsData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void FaceBangsUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                    int startId,
                                    int len,
                                    const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::FaceBangsResult *>(p);
  std::shared_ptr<FaceBangsData> data = std::dynamic_pointer_cast<FaceBangsData>(gdata);

  EXPECT_EQ(result->faceBangss_.size(), len);

  for (int i = 0; i < len; i++) {
    EXPECT_EQ(result->faceBangss_[i].status, data->bangs_[i + startId].bangs.status);
    EXPECT_NEAR(result->faceBangss_[i].statusScore, data->bangs_[i + startId].bangs.statusScore, 0.015);
  }
}


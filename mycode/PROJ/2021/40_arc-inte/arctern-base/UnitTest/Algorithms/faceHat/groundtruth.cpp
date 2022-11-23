/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/


#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "yaml-cpp/yaml.h"
#include "log/logger.h"
#include "yaml-cpp/node/parse.h"

using arctern::Result;
using arctern::ErrorType;
using arctern::ArcternRect;

void FaceHatData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto type = node[1]["VALUE"].as<std::vector<int>>();
    onepicture.hat.hatStatus = arctern::HatStatus(type[0]);

    auto score = node[2]["VALUE"].as<std::vector<float>>();
    onepicture.hat.statusScore = score[0];

    hat_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void FaceHatData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();
#ifdef ATLAS_ALLOWED
    static int atlas_index = 0;
    hat_[atlas_index].rect.x = rect[0];
    hat_[atlas_index].rect.y = rect[1];
    hat_[atlas_index].rect.width = rect[2];
    hat_[atlas_index].rect.height = rect[3];
    atlas_index++;

#else
    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        hat_[i].rect.x = rect[0];
        hat_[i].rect.y = rect[1];
        hat_[i].rect.width = rect[2];
        hat_[i].rect.height = rect[3];
      }
    }
#endif
  }
}

FaceHatUnittest::FaceHatUnittest(std::string
                                 ymlGroundTruth,
                                 std::string
                                 ymlInput,
                                 std::string
                                 imgDir,
                                 std::string
                                 modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {

}

FaceHatUnittest::~FaceHatUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> FaceHatUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceHat> ptr(new arctern::FaceHat);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> FaceHatUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                          int startId,
                                                                          int len) {
  std::shared_ptr<arctern::FaceHatRunParameter> ptr(new arctern::FaceHatRunParameter);
  std::shared_ptr<FaceHatData> data = std::dynamic_pointer_cast<FaceHatData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->hat_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FaceHatUnittest::generateGroundTruth() {
  std::shared_ptr<FaceHatData> ptr(new FaceHatData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void FaceHatUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                  int startId,
                                  int len,
                                  const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::FaceHatResult *>(p);
  std::shared_ptr<FaceHatData> data = std::dynamic_pointer_cast<FaceHatData>(gdata);

  EXPECT_EQ(result->FaceHats_.size(), len);

  for (int i = 0; i < len; i++) {
    EXPECT_EQ(result->FaceHats_[i].hatStatus, data->hat_[i + startId].hat.hatStatus);
    EXPECT_NEAR(result->FaceHats_[i].statusScore, data->hat_[i + startId].hat.statusScore, 0.01);
  }
}

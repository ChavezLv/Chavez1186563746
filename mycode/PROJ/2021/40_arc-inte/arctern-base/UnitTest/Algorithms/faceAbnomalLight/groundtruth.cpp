/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.14
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include <include/autoTime.h>
#include "./groundtruth.h"
#include "yaml-cpp/yaml.h"
#include "yaml-cpp/node/parse.h"
#include "gtest/gtest.h"


void FaceAbnomalLightData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto type = node[1]["VALUE"].as<std::vector<int>>();
    onepicture.al.type = arctern::AbnomalLightType(type[0]);
    auto score = node[2]["VALUE"].as<std::vector<float>>();
    onepicture.al.score = score[0];

    abnomalLight_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void FaceAbnomalLightData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();

    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        abnomalLight_[i].rect.x = rect[0];
        abnomalLight_[i].rect.y = rect[1];
        abnomalLight_[i].rect.width = rect[2];
        abnomalLight_[i].rect.height = rect[3];
      }
    }
  }
}

FaceAbnomalLightUnittest::FaceAbnomalLightUnittest(std::string
                                                   ymlGroundTruth,
                                                   std::string
                                                   ymlInput,
                                                   std::string
                                                   imgDir,
                                                   std::string
                                                   modelpath,
                                                   arctern::Result *result)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,result) {

}

FaceAbnomalLightUnittest::~FaceAbnomalLightUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> FaceAbnomalLightUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceAbnomalLight> ptr(new arctern::FaceAbnomalLight);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> FaceAbnomalLightUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                                   int startId,
                                                                                   int len) {
  std::shared_ptr<arctern::FaceAbnomalLightRunParameter> ptr(new arctern::FaceAbnomalLightRunParameter);
  std::shared_ptr<FaceAbnomalLightData> data = std::dynamic_pointer_cast<FaceAbnomalLightData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->abnomalLight_[i + startId].rect);
  }


  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FaceAbnomalLightUnittest::generateGroundTruth() {
  std::shared_ptr<FaceAbnomalLightData> ptr(new FaceAbnomalLightData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void FaceAbnomalLightUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                           int startId,
                                           int len,
                                           const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::FaceAbnomalLightResult *>(p);
  std::shared_ptr<FaceAbnomalLightData> data = std::dynamic_pointer_cast<FaceAbnomalLightData>(gdata);

  EXPECT_EQ(result->faceAbnomalLights_.size(), len);

  for (int i = 0; i < len; i++) {
    EXPECT_EQ(result->faceAbnomalLights_[i].type, data->abnomalLight_[i + startId].al.type);
    EXPECT_NEAR(result->faceAbnomalLights_[i].score, data->abnomalLight_[i + startId].al.score, 0.01);
  }
}



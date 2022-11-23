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

void FaceNoiseMotionblurData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto type = node[1]["VALUE"].as<std::vector<int>>();
    onepicture.nm.type = arctern::NoiseType(type[0]);
    auto score = node[2]["VALUE"].as<std::vector<float>>();
    onepicture.nm.score = score[0];

    faceNoiseMotionblur_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void FaceNoiseMotionblurData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();

    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        faceNoiseMotionblur_[i].rect.x = rect[0];
        faceNoiseMotionblur_[i].rect.y = rect[1];
        faceNoiseMotionblur_[i].rect.width = rect[2];
        faceNoiseMotionblur_[i].rect.height = rect[3];
      }
    }
  }
}

FaceNoiseMotionblurUnittest::FaceNoiseMotionblurUnittest(std::string
                                                         ymlGroundTruth,
                                                         std::string
                                                         ymlInput,
                                                         std::string
                                                         imgDir,
                                                         std::string
                                                         modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {

}

FaceNoiseMotionblurUnittest::~FaceNoiseMotionblurUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> FaceNoiseMotionblurUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceNoiseMotionblur> ptr(new arctern::FaceNoiseMotionblur);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> FaceNoiseMotionblurUnittest::buildRunParameter(std::shared_ptr<
    BaseGroundTruthData> gdata,
                                                                                      int startId,
                                                                                      int len) {
  std::shared_ptr<arctern::FaceNoiseMotionblurRunParameter> ptr(new arctern::FaceNoiseMotionblurRunParameter);
  std::shared_ptr<FaceNoiseMotionblurData> data = std::dynamic_pointer_cast<FaceNoiseMotionblurData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->faceNoiseMotionblur_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FaceNoiseMotionblurUnittest::generateGroundTruth() {
  std::shared_ptr<FaceNoiseMotionblurData> ptr(new FaceNoiseMotionblurData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void FaceNoiseMotionblurUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                              int startId,
                                              int len,
                                              const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::FaceNoiseMotionblurResult *>(p);
  std::shared_ptr<FaceNoiseMotionblurData> data = std::dynamic_pointer_cast<FaceNoiseMotionblurData>(gdata);

  EXPECT_EQ(static_cast<int>(result->faceNoiseMotionblurs_.size()), len);

  for (int i = 0; i < len; i++) {
    EXPECT_EQ(result->faceNoiseMotionblurs_[i].type, data->faceNoiseMotionblur_[i + startId].nm.type);
    EXPECT_NEAR(result->faceNoiseMotionblurs_[i].score, data->faceNoiseMotionblur_[i + startId].nm.score, 0.01);
  }
}


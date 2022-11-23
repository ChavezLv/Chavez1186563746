/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.09
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "yaml-cpp/yaml.h"
#include "log/logger.h"
#include "yaml-cpp/node/parse.h"

using arctern::Result;
using arctern::FaceImageColorResult;
using arctern::ErrorType;
using arctern::ArcternRect;

void FaceImageColorData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto scores = node[1]["VALUE"].as<std::vector<float>>();
    onepicture.score = scores[0];
    faceImageColor_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void FaceImageColorData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();

    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        faceImageColor_[i].rect.x = rect[0];
        faceImageColor_[i].rect.y = rect[1];
        faceImageColor_[i].rect.width = rect[2];
        faceImageColor_[i].rect.height = rect[3];
      }
    }
  }
}

FaceImageColorUnittest::FaceImageColorUnittest(std::string
                                   ymlGroundTruth,
                                   std::string
                                   ymlInput,
                                   std::string
                                   imgDir,
                                   std::string
                                   modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {

}

FaceImageColorUnittest::~FaceImageColorUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> FaceImageColorUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceImageColor> ptr(new arctern::FaceImageColor);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> FaceImageColorUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                           int startId,
                                                                           int len) {
  std::shared_ptr<arctern::FaceImageColorRunParameter> ptr(new arctern::FaceImageColorRunParameter);
  std::shared_ptr<FaceImageColorData> data = std::dynamic_pointer_cast<FaceImageColorData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->faceImageColor_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FaceImageColorUnittest::generateGroundTruth() {
  std::shared_ptr<FaceImageColorData> ptr(new FaceImageColorData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void FaceImageColorUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                   int startId,
                                   int len,
                                   const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::FaceImageColorResult *>(p);
  std::shared_ptr<FaceImageColorData> data = std::dynamic_pointer_cast<FaceImageColorData>(gdata);

  EXPECT_EQ(static_cast<int>(result->faceImageColors_.size()), len);

  for (int i = 0; i < len; i++) {
    EXPECT_NEAR(result->faceImageColors_[i], data->faceImageColor_[i + startId].score, 0.01);
  }
}


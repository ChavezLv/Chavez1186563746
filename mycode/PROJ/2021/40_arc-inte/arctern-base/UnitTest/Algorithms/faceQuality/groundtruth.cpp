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

void FaceQualityData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
      auto score = node[1]["VALUE"].as<std::vector<float>>();
      onepicture.score = score[0];

    faceQuality_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void FaceQualityData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();
#ifndef ATLAS_ALLOWED
    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        faceQuality_[i].rect.x = rect[0];
        faceQuality_[i].rect.y = rect[1];
        faceQuality_[i].rect.width = rect[2];
        faceQuality_[i].rect.height = rect[3];
      }
    }
#else
    static int atlas_index = 0;
    faceQuality_[atlas_index].rect.x = rect[0];
    faceQuality_[atlas_index].rect.y = rect[1];
    faceQuality_[atlas_index].rect.width = rect[2];
    faceQuality_[atlas_index].rect.height = rect[3];
    atlas_index++;
#endif
  }
}

FaceQualityUnittest::FaceQualityUnittest(std::string
                                   ymlGroundTruth,
                                   std::string
                                   ymlInput,
                                   std::string
                                   imgDir,
                                   std::string
                                   modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {
  fout_ << "FaceQuality:imageDir imageName rect quality" <<std::endl;
}

FaceQualityUnittest::~FaceQualityUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> FaceQualityUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceQuality> ptr(new arctern::FaceQuality);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> FaceQualityUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                           int startId,
                                                                           int len) {
  std::shared_ptr<arctern::FaceQualityRunParameter> ptr(new arctern::FaceQualityRunParameter);
  std::shared_ptr<FaceQualityData> data = std::dynamic_pointer_cast<FaceQualityData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->faceQuality_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FaceQualityUnittest::generateGroundTruth() {
  std::shared_ptr<FaceQualityData> ptr(new FaceQualityData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void FaceQualityUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                   int startId,
                                   int len,
                                   const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::FaceQualityResult *>(p);
  std::shared_ptr<FaceQualityData> data = std::dynamic_pointer_cast<FaceQualityData>(gdata);

  EXPECT_EQ(static_cast<int>(result->faceQuality_.size()), len);

  for (int i = 0; i < len; i++) {
    fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
    fout_ << data->faceQuality_[i+startId].rect.x << " " <<  data->faceQuality_[i+startId].rect.y << " "
          << data->faceQuality_[i+startId].rect.width << " " << data->faceQuality_[i+startId].rect.height << " ";
    fout_ << result->faceQuality_[i] <<std::endl;
    EXPECT_NEAR(result->faceQuality_[i], data->faceQuality_[i + startId].score, 0.01);
  }
}


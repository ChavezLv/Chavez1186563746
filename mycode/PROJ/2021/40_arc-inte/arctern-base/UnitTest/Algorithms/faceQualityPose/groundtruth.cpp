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

void FaceQualityPoseData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto score = node[1]["VALUE"][0].as<std::vector<float>>();
    onepicture.qualityPose.quality = score[0];
    onepicture.qualityPose.pitch = score[1];
    onepicture.qualityPose.yaw = score[2];
    onepicture.qualityPose.roll = score[3];

    faceQualityPose_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void FaceQualityPoseData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();

    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        faceQualityPose_[i].rect.x = rect[0];
        faceQualityPose_[i].rect.y = rect[1];
        faceQualityPose_[i].rect.width = rect[2];
        faceQualityPose_[i].rect.height = rect[3];
      }
    }
  }
}

FaceQualityPoseUnittest::FaceQualityPoseUnittest(std::string
                                         ymlGroundTruth,
                                         std::string
                                         ymlInput,
                                         std::string
                                         imgDir,
                                         std::string
                                         modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {

}

FaceQualityPoseUnittest::~FaceQualityPoseUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> FaceQualityPoseUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceQualityPose> ptr(new arctern::FaceQualityPose);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> FaceQualityPoseUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                              int startId,
                                                                              int len) {
  std::shared_ptr<arctern::FaceQualityPoseRunParameter> ptr(new arctern::FaceQualityPoseRunParameter);
  std::shared_ptr<FaceQualityPoseData> data = std::dynamic_pointer_cast<FaceQualityPoseData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->faceQualityPose_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FaceQualityPoseUnittest::generateGroundTruth() {
  std::shared_ptr<FaceQualityPoseData> ptr(new FaceQualityPoseData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void FaceQualityPoseUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                      int startId,
                                      int len,
                                      const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::FaceQualityPoseResult *>(p);
  std::shared_ptr<FaceQualityPoseData> data = std::dynamic_pointer_cast<FaceQualityPoseData>(gdata);

  EXPECT_EQ(result->faceQualityPose_.size(), len);

  for (int i = 0; i < len; i++) {
    EXPECT_NEAR(result->faceQualityPose_[i].pitch, data->faceQualityPose_[i + startId].qualityPose.pitch, 0.01);
    EXPECT_NEAR(result->faceQualityPose_[i].yaw, data->faceQualityPose_[i + startId].qualityPose.yaw, 0.01);
    EXPECT_NEAR(result->faceQualityPose_[i].roll, data->faceQualityPose_[i + startId].qualityPose.roll, 0.01);
    EXPECT_NEAR(result->faceQualityPose_[i].quality, data->faceQualityPose_[i + startId].qualityPose.quality, 0.01);
  }
}


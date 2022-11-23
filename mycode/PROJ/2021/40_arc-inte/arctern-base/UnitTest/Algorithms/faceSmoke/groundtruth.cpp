/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.28
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
void FaceSmokeData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
      auto type = node[1]["VALUE"].as<std::vector<int>>();
      onepicture.smoke.smokeStatus = arctern::SmokeStatus(type[0]);

      auto score = node[2]["VALUE"].as<std::vector<float>>();
      onepicture.smoke.statusScore = score[0];

    faceSmoke_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void FaceSmokeData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();
#ifdef ATLAS_ALLOWED
    static int atlas_index = 0;
    faceSmoke_[atlas_index].rect.x = rect[0];
    faceSmoke_[atlas_index].rect.y = rect[1];
    faceSmoke_[atlas_index].rect.width = rect[2];
    faceSmoke_[atlas_index].rect.height = rect[3];
    atlas_index++;
#else
    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        faceSmoke_[i].rect.x = rect[0];
        faceSmoke_[i].rect.y = rect[1];
        faceSmoke_[i].rect.width = rect[2];
        faceSmoke_[i].rect.height = rect[3];
      }
    }
#endif
  }
}

FaceSmokeUnittest::FaceSmokeUnittest(std::string
                                                 ymlGroundTruth,
                                                 std::string
                                                 ymlInput,
                                                 std::string
                                                 imgDir,
                                                 std::string
                                                 modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {
  fout_ << "FaceSmoke:imageDir imageName rect smokeStatus  score"<<std::endl;
}

FaceSmokeUnittest::~FaceSmokeUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> FaceSmokeUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceSmoke> ptr(new arctern::FaceSmoke);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> FaceSmokeUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                                  int startId,
                                                                                  int len) {
  std::shared_ptr<arctern::FaceSmokeRunParameter> ptr(new arctern::FaceSmokeRunParameter);
  std::shared_ptr<FaceSmokeData> data = std::dynamic_pointer_cast<FaceSmokeData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->faceSmoke_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FaceSmokeUnittest::generateGroundTruth() {
  std::shared_ptr<FaceSmokeData> ptr(new FaceSmokeData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void FaceSmokeUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                          int startId,
                                          int len,
                                          const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::FaceSmokeResult *>(p);
  std::shared_ptr<FaceSmokeData> data = std::dynamic_pointer_cast<FaceSmokeData>(gdata);

  EXPECT_EQ(result->faceSmokes_.size(), len);

  for (int i = 0; i < len; i++) {
    fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
    fout_ << data->faceSmoke_[i+startId].rect.x<<" "<<data->faceSmoke_[i+startId].rect.y << " "
          <<data->faceSmoke_[i+startId].rect.width << " " <<data->faceSmoke_[i+startId].rect.height<<" ";
    fout_ <<(int) result->faceSmokes_[i].smokeStatus << " " <<result->faceSmokes_[i].statusScore<<std::endl;
    EXPECT_EQ(result->faceSmokes_[i].smokeStatus, data->faceSmoke_[i + startId].smoke.smokeStatus);
    EXPECT_NEAR(result->faceSmokes_[i].statusScore, data->faceSmoke_[i + startId].smoke.statusScore, 0.01);
  }
}

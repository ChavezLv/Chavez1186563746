/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.27
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/


#include "./groundtruth.h"
#include "yaml-cpp/yaml.h"
#include "yaml-cpp/node/parse.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "log/logger.h"

void FacePoseData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
      auto level = node[2]["VALUE"][0].as<std::vector<float>>();
    onepicture.pose.pitch = arctern::FacePoseLevel(level[0]);
    onepicture.pose.yaw = arctern::FacePoseLevel(level[1]);
    onepicture.pose.roll = arctern::FacePoseLevel(level[2]);

    facePose_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void FacePoseData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();
#ifdef ATLAS_ALLOWED
    static int atlas_index = 0;
    facePose_[atlas_index].rect.x = rect[0];
    facePose_[atlas_index].rect.y = rect[1];
    facePose_[atlas_index].rect.width = rect[2];
    facePose_[atlas_index].rect.height = rect[3];
    atlas_index++;
#else
    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        facePose_[i].rect.x = rect[0];
        facePose_[i].rect.y = rect[1];
        facePose_[i].rect.width = rect[2];
        facePose_[i].rect.height = rect[3];
      }
    }
  
#endif
  }
}

FacePoseUnittest::FacePoseUnittest(std::string
                                     ymlGroundTruth,
                                     std::string
                                     ymlInput,
                                     std::string
                                     imgDir,
                                     std::string
                                     modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {
  fout_ << "FacePose:imageDir imageName rect pitch roll yaw"<<std::endl;
}

FacePoseUnittest::~FacePoseUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> FacePoseUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FacePose> ptr(new arctern::FacePose);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> FacePoseUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                            int startId,
                                                                            int len) {
  std::shared_ptr<arctern::FacePoseRunParameter> ptr(new arctern::FacePoseRunParameter);
  std::shared_ptr<FacePoseData> data = std::dynamic_pointer_cast<FacePoseData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->facePose_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FacePoseUnittest::generateGroundTruth() {
  std::shared_ptr<FacePoseData> ptr(new FacePoseData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void FacePoseUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                    int startId,
                                    int len,
                                    const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::FacePoseResult *>(p);
  std::shared_ptr<FacePoseData> data = std::dynamic_pointer_cast<FacePoseData>(gdata);

  EXPECT_EQ(result->facePose_.size(), len);

  for (int i = 0; i < len; i++) {
    fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
    fout_ << data->facePose_[i+startId].rect.x << " " <<  data->facePose_[i+startId].rect.y << " "
    << data->facePose_[i+startId].rect.width << " " << data->facePose_[i+startId].rect.height << " ";
    fout_ << result->facePose_[i].pitch << " " << result->facePose_[i].roll << " "<< result->facePose_[i].yaw << std::endl;
    EXPECT_NEAR(result->facePose_[i].pitch, data->facePose_[i + startId].pose.pitch, 0.01);
    EXPECT_NEAR(result->facePose_[i].roll, data->facePose_[i + startId].pose.roll, 0.01);
    EXPECT_NEAR(result->facePose_[i].yaw, data->facePose_[i + startId].pose.yaw, 0.01);
  }
}

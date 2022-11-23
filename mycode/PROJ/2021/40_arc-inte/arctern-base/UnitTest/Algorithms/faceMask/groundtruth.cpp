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
using arctern::FaceMaskResult;
using arctern::ErrorType;
using arctern::ArcternRect;

void FaceMaskData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto type = node[1]["VALUE"].as<std::vector<int>>();
    onepicture.mask.maskStatus = type[0] == 0 ? arctern::NOMASK : arctern::MASK;

    auto score = node[2]["VALUE"].as<std::vector<float>>();
    onepicture.mask.statusScore = score[0];
    faceMask_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void FaceMaskData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();
#ifndef ATLAS_ALLOWED
    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        faceMask_[i].rect.x = rect[0];
        faceMask_[i].rect.y = rect[1];
        faceMask_[i].rect.width = rect[2];
        faceMask_[i].rect.height = rect[3];
      }
    }
#else
    static int atlas_index = 0;
    faceMask_[atlas_index].rect.x = rect[0];
    faceMask_[atlas_index].rect.y = rect[1];
    faceMask_[atlas_index].rect.width = rect[2];
    faceMask_[atlas_index].rect.height = rect[3];
    atlas_index++;
#endif
  }
}

FaceMaskUnittest::FaceMaskUnittest(std::string
                                   ymlGroundTruth,
                                   std::string
                                   ymlInput,
                                   std::string
                                   imgDir,
                                   std::string
                                   modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {
  fout_ << "FaceMask: imageDir imageName mask score" <<std::endl;
}

FaceMaskUnittest::~FaceMaskUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> FaceMaskUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceMask> ptr(new arctern::FaceMask);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> FaceMaskUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                           int startId,
                                                                           int len) {
  std::shared_ptr<arctern::FaceMaskRunParameter> ptr(new arctern::FaceMaskRunParameter);
  std::shared_ptr<FaceMaskData> data = std::dynamic_pointer_cast<FaceMaskData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->faceMask_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FaceMaskUnittest::generateGroundTruth() {
  std::shared_ptr<FaceMaskData> ptr(new FaceMaskData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void FaceMaskUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                   int startId,
                                   int len,
                                   const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::FaceMaskResult *>(p);
  std::shared_ptr<FaceMaskData> data = std::dynamic_pointer_cast<FaceMaskData>(gdata);

  EXPECT_EQ(static_cast<int>(result->FaceMasks_.size()), len);

  for (int i = 0; i < len; i++) {
    fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
    fout_ << data->faceMask_[i+startId].rect.x<<" "<<data->faceMask_[i+startId].rect.y << " "
          <<data->faceMask_[i+startId].rect.width << " " <<data->faceMask_[i+startId].rect.height<<" ";
    fout_ << result->FaceMasks_[i].maskStatus << " " << result->FaceMasks_[i].statusScore << std::endl;

    EXPECT_EQ(result->FaceMasks_[i].maskStatus, data->faceMask_[i + startId].mask.maskStatus);
    EXPECT_NEAR(result->FaceMasks_[i].statusScore, data->faceMask_[i + startId].mask.statusScore, 0.01);
  }
}


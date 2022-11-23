/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         kuanghongshen
 *  Last modified:  2021.06.01
 *  email:          kuang.hongshen@intellif.com
 ****************************************************************************/
#include "./groundtruth.h"
#include "yaml-cpp/node/parse.h"
#include "yaml-cpp/yaml.h"
#include "gtest/gtest.h"
#include <iostream>

using arctern::ArcternRect;
using arctern::ErrorType;
using arctern::FaceIdentifyResult;
using arctern::Result;

FaceIdentifyData::FaceIdentifyData() {}

FaceIdentifyData::~FaceIdentifyData() {}

void FaceIdentifyData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto type = node[1]["VALUE"][0].as<int>();
    onepicture.result.first = type;

    auto score = node[2]["VALUE"][0].as<float>();
    onepicture.result.second = score;

    typeResults.push_back(onepicture);
    files_.push_back(filename);
  }
}

void FaceIdentifyData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();

    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        typeResults[i].rect.x = rect[0];
        typeResults[i].rect.y = rect[1];
        typeResults[i].rect.width = rect[2];
        typeResults[i].rect.height = rect[3];
      }
    }
  }
}

FaceIdentifyUnittest::FaceIdentifyUnittest(std::string ymlGroundTruth,
                                   std::string ymlInput, std::string imgDir,
                                   std::string modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {
  fout_ << "FaceIdentity:imageDir imageName identityType score"<<std::endl;
}

FaceIdentifyUnittest::~FaceIdentifyUnittest() {}

std::shared_ptr<arctern::AlgorithmInterface>
FaceIdentifyUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceIdentify> ptr(new arctern::FaceIdentify);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter>
FaceIdentifyUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                    int startId, int len) {
  std::shared_ptr<arctern::FaceIdentifyRunParameter> ptr(
      new arctern::FaceIdentifyRunParameter);
  std::shared_ptr<FaceIdentifyData> data =
      std::dynamic_pointer_cast<FaceIdentifyData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->typeResults[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FaceIdentifyUnittest::generateGroundTruth() {
  std::shared_ptr<FaceIdentifyData> ptr(new FaceIdentifyData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void FaceIdentifyUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                   int startId, int len,
                                   const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::FaceIdentifyResult *>(p);
  std::shared_ptr<FaceIdentifyData> data =
      std::dynamic_pointer_cast<FaceIdentifyData>(gdata);

  EXPECT_EQ(result->m_vTypeResults.size(), len);

  for (int i = 0; i < len; i++) {
    fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
    fout_ << data->typeResults[i+startId].rect.x<<" "<<data->typeResults[i+startId].rect.y << " "
          <<data->typeResults[i+startId].rect.width << " " <<data->typeResults[i+startId].rect.height<<" ";
    fout_ << result->m_vTypeResults[i].first << " " <<result->m_vTypeResults[i].second << std::endl;

    EXPECT_EQ(result->m_vTypeResults[i].first,
              data->typeResults[i + startId].result.first);
    EXPECT_NEAR(result->m_vTypeResults[i].second,
                data->typeResults[i + startId].result.second, 0.01);
  }
}

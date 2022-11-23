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
using arctern::FaceTypeResult;
using arctern::Result;

faceTypeData::faceTypeData() {}

faceTypeData::~faceTypeData() {}

void faceTypeData::addGroundTruth(YAML::Node &node, std::string filename) {
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

void faceTypeData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();
#ifdef ATLAS_ALLOWED
    static int atlas_index = 0;
    typeResults[atlas_index].rect.x = rect[0];
    typeResults[atlas_index].rect.y = rect[1];
    typeResults[atlas_index].rect.width = rect[2];
    typeResults[atlas_index].rect.height = rect[3];
    atlas_index++;
#else
    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        typeResults[i].rect.x = rect[0];
        typeResults[i].rect.y = rect[1];
        typeResults[i].rect.width = rect[2];
        typeResults[i].rect.height = rect[3];
      }
    }
#endif
  }
}

faceTypeUnittest::faceTypeUnittest(std::string ymlGroundTruth,
                                   std::string ymlInput, std::string imgDir,
                                   std::string modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {
  fout_ << "FaceType: imageDir imageName rect type score" <<std::endl;
}

faceTypeUnittest::~faceTypeUnittest() {}

std::shared_ptr<arctern::AlgorithmInterface>
faceTypeUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceType> ptr(new arctern::FaceType);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter>
faceTypeUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                    int startId, int len) {
  std::shared_ptr<arctern::FaceTypeRunParameter> ptr(
      new arctern::FaceTypeRunParameter);
  std::shared_ptr<faceTypeData> data =
      std::dynamic_pointer_cast<faceTypeData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->typeResults[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> faceTypeUnittest::generateGroundTruth() {
  std::shared_ptr<faceTypeData> ptr(new faceTypeData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void faceTypeUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                   int startId, int len,
                                   const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::FaceTypeResult *>(p);
  std::shared_ptr<faceTypeData> data =
      std::dynamic_pointer_cast<faceTypeData>(gdata);

  EXPECT_EQ(result->m_vTypeResults.size(), len);

  for (int i = 0; i < len; i++) {
    fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
    fout_ << data->typeResults[i+startId].rect.x<<" "<<data->typeResults[i+startId].rect.y << " "
          <<data->typeResults[i+startId].rect.width << " " <<data->typeResults[i+startId].rect.height<<" ";
    fout_ << result->m_vTypeResults[i].first << " " << result->m_vTypeResults[i].second << std::endl;

    EXPECT_EQ(result->m_vTypeResults[i].first,
              data->typeResults[i + startId].result.first);
    EXPECT_NEAR(result->m_vTypeResults[i].second,
                data->typeResults[i + startId].result.second, 0.01);
  }
}

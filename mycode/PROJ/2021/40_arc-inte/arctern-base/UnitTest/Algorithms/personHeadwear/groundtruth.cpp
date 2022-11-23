/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.02
 *  email:          chen.bin1@intellif.com
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

void PersonHeadwearData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto type = node[1]["VALUE"].as<std::vector<std::string>>();
    onepicture.headwear.headwearType = str2type_map[type.at(0)];

    auto score = node[2]["VALUE"].as<std::vector<float>>();
    onepicture.headwear.typeScore = score.at(0);

    headwears_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void PersonHeadwearData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();
#ifdef ATLAS_ALLOWED
    static int atlas_index = 0;
    headwears_[atlas_index].rect.x = rect[0];
    headwears_[atlas_index].rect.y = rect[1];
    headwears_[atlas_index].rect.width = rect[2];
    headwears_[atlas_index++].rect.height = rect[3];
#else
    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        headwears_[i].rect.x = rect[0];
        headwears_[i].rect.y = rect[1];
        headwears_[i].rect.width = rect[2];
        headwears_[i].rect.height = rect[3];
      }
    }
#endif
  }
}

PersonHeadwearUnittest::PersonHeadwearUnittest(std::string
                                 ymlGroundTruth,
                                 std::string
                                 ymlInput,
                                 std::string
                                 imgDir,
                                 std::string
                                 modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {
  fout_ << "PersonHeadwear: imageDir imageName headwear score"<<std::endl;
}

PersonHeadwearUnittest::~PersonHeadwearUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> PersonHeadwearUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::PersonHeadwear> ptr(new arctern::PersonHeadwear);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> PersonHeadwearUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                          int startId,
                                                                          int len) {
  std::shared_ptr<arctern::PersonHeadwearRunParameter> ptr(new arctern::PersonHeadwearRunParameter);
  std::shared_ptr<PersonHeadwearData> data = std::dynamic_pointer_cast<PersonHeadwearData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->headwears_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> PersonHeadwearUnittest::generateGroundTruth() {
  std::shared_ptr<PersonHeadwearData> ptr(new PersonHeadwearData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void PersonHeadwearUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                  int startId,
                                  int len,
                                  const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::PersonHeadwearResult *>(p);
  std::shared_ptr<PersonHeadwearData> data = std::dynamic_pointer_cast<PersonHeadwearData>(gdata);

  EXPECT_EQ(result->PersonHeadwears_.size(), len);

  for (int i = 0; i < len; i++) {
    fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
    fout_ << data->headwears_[i+startId].rect.x<<" "<<data->headwears_[i+startId].rect.y << " "
          << data->headwears_[i+startId].rect.width << " " <<data->headwears_[i+startId].rect.height<<" ";
    fout_ <<(int) result->PersonHeadwears_[i].headwearType <<" " << result->PersonHeadwears_[i].typeScore<<std::endl;

    EXPECT_EQ(result->PersonHeadwears_[i].headwearType, data->headwears_[i + startId].headwear.headwearType);
    EXPECT_NEAR(result->PersonHeadwears_[i].typeScore, data->headwears_[i + startId].headwear.typeScore, 0.02);
  }
}

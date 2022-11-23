/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.09
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "yaml-cpp/yaml.h"
#include "yaml-cpp/node/parse.h"

using arctern::Result;
using arctern::AgeGenderResult;
using arctern::ErrorType;
using arctern::ArcternRect;

AgeGenderData::AgeGenderData() {
}

AgeGenderData::~AgeGenderData() {
}

void AgeGenderData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto type = node[1]["VALUE"][0].as<std::vector<int>>();
    onepicture.ageGender.gender = type[0] == 0 ? arctern::MALE : arctern::FEMALE;
    onepicture.ageGender.age = type[1];

    auto score = node[2]["VALUE"][0].as<std::vector<float>>();
    onepicture.ageGender.genderScore = score[0];
    onepicture.ageGender.ageScore = score[1];

    ageGender_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void AgeGenderData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();
#ifdef ATLAS_ALLOWED
    static int atlas_index = 0;
    ageGender_[atlas_index].rect.x = rect[0];
    ageGender_[atlas_index].rect.y = rect[1];
    ageGender_[atlas_index].rect.width = rect[2];
    ageGender_[atlas_index].rect.height = rect[3];
    atlas_index++;
#else
    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        ageGender_[i].rect.x = rect[0];
        ageGender_[i].rect.y = rect[1];
        ageGender_[i].rect.width = rect[2];
        ageGender_[i].rect.height = rect[3];
      }
    }
#endif
  }
}

AgeGenderUnittest::AgeGenderUnittest(std::string
                                     ymlGroundTruth,
                                     std::string
                                     ymlInput,
                                     std::string
                                     imgDir,
                                     std::string
                                     modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {

}

AgeGenderUnittest::~AgeGenderUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> AgeGenderUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::AgeGender> ptr(new arctern::AgeGender);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> AgeGenderUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                            int startId,
                                                                            int len) {
  std::shared_ptr<arctern::AgeGenderRunParameter> ptr(new arctern::AgeGenderRunParameter);
  std::shared_ptr<AgeGenderData> data = std::dynamic_pointer_cast<AgeGenderData>(gdata);

  for(int i = 0 ; i < len; i++) {
    ptr->rectsV_.push_back(data->ageGender_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> AgeGenderUnittest::generateGroundTruth() {
  std::shared_ptr<AgeGenderData> ptr(new AgeGenderData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void AgeGenderUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                    int startId,
                                    int len,
                                    const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::AgeGenderResult *>(p);
  std::shared_ptr<AgeGenderData> data = std::dynamic_pointer_cast<AgeGenderData>(gdata);

  EXPECT_EQ(result->ageGenders_.size(), len);

  for (int i = 0; i < len; i++) {
    EXPECT_EQ(result->ageGenders_[i].age, data->ageGender_[i + startId].ageGender.age);
    EXPECT_EQ(result->ageGenders_[i].gender, data->ageGender_[i + startId].ageGender.gender);
    EXPECT_NEAR(result->ageGenders_[i].ageScore, data->ageGender_[i + startId].ageGender.ageScore, 0.01);
    EXPECT_NEAR(result->ageGenders_[i].genderScore, data->ageGender_[i + startId].ageGender.genderScore, 0.01);
  }
}



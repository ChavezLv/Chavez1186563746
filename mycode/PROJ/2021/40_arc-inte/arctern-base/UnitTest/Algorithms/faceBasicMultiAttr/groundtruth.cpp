/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.11.25
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "yaml-cpp/yaml.h"
#include "log/logger.h"
#include "yaml-cpp/node/parse.h"

void FaceBasicMultiAttrData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto type  = node[1]["VALUE"][0].as<std::vector<int>>();
    auto score = node[2]["VALUE"][0].as<std::vector<float>>();
    onepicture.attrs.gender   = static_cast<arctern::BasicGender>(type[1]);
    onepicture.attrs.age      = type[3];
    onepicture.attrs.glasses  = static_cast<arctern::BasicGlasses>(type[5]);
    onepicture.attrs.mask     = static_cast<arctern::BasicMask>(type[7]);
    onepicture.attrs.headwear = static_cast<arctern::BasicHeadwear>(type[9]);
    onepicture.attrs.genderScore   = score[0];
    onepicture.attrs.ageScore      = score[1];
    onepicture.attrs.glassesScore  = score[2];
    onepicture.attrs.maskScore     = score[3];
    onepicture.attrs.headwearScore = score[4];

    faceBasicMultiAttr_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void FaceBasicMultiAttrData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();
#ifdef ATLAS_ALLOWED
    static int atlas_index = 0;
    faceBasicMultiAttr_[atlas_index].rect.x = rect[0];
    faceBasicMultiAttr_[atlas_index].rect.y = rect[1];
    faceBasicMultiAttr_[atlas_index].rect.width = rect[2];
    faceBasicMultiAttr_[atlas_index].rect.height = rect[3];
    atlas_index++;
#else
    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        faceBasicMultiAttr_[i].rect.x = rect[0];
        faceBasicMultiAttr_[i].rect.y = rect[1];
        faceBasicMultiAttr_[i].rect.width = rect[2];
        faceBasicMultiAttr_[i].rect.height = rect[3];
      }
    }
#endif
  }
}

FaceBasicMultiAttrUnittest::FaceBasicMultiAttrUnittest(std::string ymlGroundTruth,
                                                       std::string ymlInput,
                                                       std::string imgDir,
                                                       std::string modelpath,
                                                       arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {
  fout_ << "FaceBasicMultiAttr:imageDir imageName rect gender genderScore age ageScore glasses glasserScore mask maskScore headwear headwearScore"<<std::endl;
}

FaceBasicMultiAttrUnittest::~FaceBasicMultiAttrUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> FaceBasicMultiAttrUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceBasicMultiAttr> ptr(new arctern::FaceBasicMultiAttr);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> FaceBasicMultiAttrUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                               int startId,
                                                                               int len) {
  std::shared_ptr<arctern::FaceBasicMultiAttrRunParameter> ptr(new arctern::FaceBasicMultiAttrRunParameter);
  std::shared_ptr<FaceBasicMultiAttrData> data = std::dynamic_pointer_cast<FaceBasicMultiAttrData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->faceBasicMultiAttr_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FaceBasicMultiAttrUnittest::generateGroundTruth() {
  std::shared_ptr<FaceBasicMultiAttrData> ptr(new FaceBasicMultiAttrData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void FaceBasicMultiAttrUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                       int startId,
                                       int len,
                                       const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::FaceBasicMultiAttrResult *>(p);
  std::shared_ptr<FaceBasicMultiAttrData> data = std::dynamic_pointer_cast<FaceBasicMultiAttrData>(gdata);

  EXPECT_EQ(result->faceBasicMultiAttrs_.size(), len);

  for (int i = 0; i < len; i++) {
    fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
    fout_ << data->faceBasicMultiAttr_[i+startId].rect.x<<" "<<data->faceBasicMultiAttr_[i+startId].rect.y << " "
          <<data->faceBasicMultiAttr_[i+startId].rect.width << " " <<data->faceBasicMultiAttr_[i+startId].rect.height<<" ";
    fout_ <<(int) result->faceBasicMultiAttrs_[i].gender << " " << result->faceBasicMultiAttrs_[i].genderScore << " ";
    fout_ << result->faceBasicMultiAttrs_[i].age <<" " <<result->faceBasicMultiAttrs_[i].ageScore<<" ";
    fout_ <<(int) result->faceBasicMultiAttrs_[i].glasses <<" "<<result->faceBasicMultiAttrs_[i].glassesScore << " ";
    fout_ <<(int) result->faceBasicMultiAttrs_[i].mask <<" "<<result->faceBasicMultiAttrs_[i].maskScore << " ";
    fout_ <<(int) result->faceBasicMultiAttrs_[i].headwear << " "<<result->faceBasicMultiAttrs_[i].headwearScore<<std::endl;

    EXPECT_EQ(result->faceBasicMultiAttrs_[i].gender, data->faceBasicMultiAttr_[i + startId].attrs.gender);
    EXPECT_EQ(result->faceBasicMultiAttrs_[i].age, data->faceBasicMultiAttr_[i + startId].attrs.age);
    EXPECT_EQ(result->faceBasicMultiAttrs_[i].glasses, data->faceBasicMultiAttr_[i + startId].attrs.glasses);
    EXPECT_EQ(result->faceBasicMultiAttrs_[i].mask, data->faceBasicMultiAttr_[i + startId].attrs.mask);
    EXPECT_EQ(result->faceBasicMultiAttrs_[i].headwear, data->faceBasicMultiAttr_[i + startId].attrs.headwear);
    EXPECT_NEAR(result->faceBasicMultiAttrs_[i].genderScore, data->faceBasicMultiAttr_[i + startId].attrs.genderScore, 0.01);
    EXPECT_NEAR(result->faceBasicMultiAttrs_[i].ageScore, data->faceBasicMultiAttr_[i + startId].attrs.ageScore, 0.01);
    EXPECT_NEAR(result->faceBasicMultiAttrs_[i].glassesScore, data->faceBasicMultiAttr_[i + startId].attrs.glassesScore, 0.01);
    EXPECT_NEAR(result->faceBasicMultiAttrs_[i].maskScore, data->faceBasicMultiAttr_[i + startId].attrs.maskScore, 0.01);
    EXPECT_NEAR(result->faceBasicMultiAttrs_[i].headwearScore, data->faceBasicMultiAttr_[i + startId].attrs.headwearScore, 0.01);
  }
}


/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.08
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "yaml-cpp/yaml.h"
#include "log/logger.h"
#include "yaml-cpp/node/parse.h"

void PersonAttrData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto attrtype  = node[1]["VALUE"][0].as<std::vector<std::string>>();
    auto scores = node[2]["VALUE"][0].as<std::vector<float>>();
    
    for (int i = 0; i < scores.size()*2; i += 2) {
      int attrTypeId = attrString2int(attrtype.at(i), attrtype.at(i+1));
      auto &attr = onepicture.attrs;
      int id = i/2;
      std::pair<int, float> attr_res = {attrTypeId, scores.at(id)};
      switch (id) {
        case 0 : {
          attr.age = PersonAgeType(attr_res.first);
          attr.ageScore = attr_res.second;
          break;
        }
        case 1 : {
          attr.hasBackpack = attr_res.first;
          attr.hasBackpackScore = attr_res.second;
          break;
        }
        case 2 : {
          attr.hasCart = attr_res.first;
          attr.hasCartScore = attr_res.second;
          break;
        }
        case 3 : {
          attr.isFrontHold = attr_res.first;
          attr.isFrontHoldScore = attr_res.second;
          break;
        }
        case 4 : {
          attr.gender = PersonGenderType(attr_res.first);
          attr.genderScore = attr_res.second;
          break;
        }
        case 5 : {
          attr.glasses = PersonGlassesType(attr_res.first);
          attr.glassesScore = attr_res.second;
          break;
        }
        case 6 : {
          attr.hairColor = PersonHairColorType(attr_res.first);
          attr.hairColorScore = attr_res.second;
          break;
        }
        case 7 : {
          attr.hairLength = PersonHairLengthType(attr_res.first);
          attr.hairLengthScore = attr_res.second;
          break;
        }
        case 8 : {
          attr.hasHandbag = attr_res.first;
          attr.hasHandbagScore = attr_res.second;
          break;
        }
        case 9 : {
          attr.headwear = PersonHeadwearType(attr_res.first);
          attr.headwearScore = attr_res.second;
          break;
        }
        case 10 : {
          attr.lowerColor = PersonClothesColorType(attr_res.first);
          attr.lowerColorScore = attr_res.second;
          break;
        }
        case 11 : {
          attr.lowerTexture = PersonClothesTextureType(attr_res.first);
          attr.lowerTextureScore = attr_res.second;
          break;
        }
        case 12 : {
          attr.lowerType = PersonLowerClothesType(attr_res.first);
          attr.lowerTypeScore = attr_res.second;
          break;
        }
        case 13 : {
          attr.hasMask = Mask( attr_res.first);
          attr.hasMaskScore = attr_res.second;
          break;
        }
        case 14 : {
          attr.orientation = PersonOrientationType(attr_res.first);
          attr.orientationScore = attr_res.second;
          break;
        }
        case 15 : {
          attr.hasOvercoat = Overcoat( attr_res.first);
          attr.hasOvercoatScore = attr_res.second;
          break;
        }
        case 16 : {
          attr.personType = PersonType(attr_res.first);
          attr.personTypeScore = attr_res.second;
          break;
        }
        case 17 : {
          attr.hasSingleBag = SingleBag( attr_res.first);
          attr.hasSingleBagScore = attr_res.second;
          break;
        }
        case 18 : {
          attr.hasTrolleyCase = attr_res.first;
          attr.hasTrolleyCaseScore = attr_res.second;
          break;
        }
        case 19 : {
          attr.upperColor = PersonClothesColorType(attr_res.first);
          attr.upperColorScore = attr_res.second;
          break;
        }
        case 20 : {
          attr.upperTexture = PersonClothesTextureType(attr_res.first);
          attr.upperTextureScore = attr_res.second;
          break;
        }
        case 21 : {
          attr.upperType = PersonUpperClothesType(attr_res.first);
          attr.upperTypeScore = attr_res.second;
          break;
        }
        case 22 : {
          attr.usePhone = attr_res.first;
          attr.usePhoneScore = attr_res.second;
          break;
        }
        case 23:{
          attr.quality = Quality(attr_res.first);
          attr.qualityScore = attr_res.second;
          break;
        }
        default: {
          break;
        }
      }
    }

    personAttrs_.push_back(onepicture);
    files_.push_back(filename);
  }
}

int PersonAttrData::attrString2int(const std::string &attrname, const std::string &attrtype) {
  int attr_id = 0, attrtype_id = 0;
  for (size_t i = 0; i < attrs_map.size(); ++i) {
    if (attrname == attrs_map.at(i).first) {
      attr_id = i;
      break;
    }
  }
  auto &attrtypesV = attrs_map.at(attr_id).second;
  for (size_t j = 0; j < attrtypesV.size(); ++j) {
    if (attrtype == attrtypesV.at(j)) {
      attrtype_id = j;
      break;
    }
  }
  return attrtype_id;
}

void PersonAttrData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();
    auto truncat = node[2]["VALUE"][0].as<std::vector<float>>();
#ifdef ATLAS_ALLOWED
    static int atlas_index = 0;
    personAttrs_[atlas_index].rect.x = rect[0];
    personAttrs_[atlas_index].rect.y = rect[1];
    personAttrs_[atlas_index].rect.width = rect[2];
    personAttrs_[atlas_index].rect.height = rect[3];
    personAttrs_[atlas_index].truncation.push_back(truncat.at(0));
    personAttrs_[atlas_index].truncation.push_back(truncat.at(1));
    atlas_index++;
#else
    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        personAttrs_[i].rect.x = rect[0];
        personAttrs_[i].rect.y = rect[1];
        personAttrs_[i].rect.width = rect[2];
        personAttrs_[i].rect.height = rect[3];
        personAttrs_[i].truncation.push_back(truncat.at(0));
        personAttrs_[i].truncation.push_back(truncat.at(1));
      }
    }
#endif
  }
}

PersonAttrUnittest::PersonAttrUnittest(std::string
                                           ymlGroundTruth,
                                           std::string
                                           ymlInput,
                                           std::string
                                           imgDir,
                                           std::string
                                           modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {
  fout_ << "PersonAttr:imageDir imageName rect age score hasBackpack score hasCart score isFrontHold score gender score glasses score ";
  fout_ << "hairColor score hairLength score hasHandbag score headwear score lowerColor score  lowerTexture score lowerType score ";
  fout_ << "hasMask score orientation score hasOvercoat score personType score hasSingleBag score hasTrolleyCase score ";
  fout_ << "upperColor score upperTexture score upperType score usePhone score "<<std::endl;
}

PersonAttrUnittest::~PersonAttrUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> PersonAttrUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::PersonAttr> ptr(new arctern::PersonAttr);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> PersonAttrUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                               int startId,
                                                                               int len) {
  std::shared_ptr<arctern::PersonAttrRunParameter> ptr(new arctern::PersonAttrRunParameter);
  std::shared_ptr<PersonAttrData> data = std::dynamic_pointer_cast<PersonAttrData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->personAttrs_[i + startId].rect);
    ptr->personTruncationV_.push_back(data->personAttrs_[i + startId].truncation);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> PersonAttrUnittest::generateGroundTruth() {
  std::shared_ptr<PersonAttrData> ptr(new PersonAttrData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void PersonAttrUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                       int startId,
                                       int len,
                                       const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::PersonAttrResult *>(p);
  std::shared_ptr<PersonAttrData> data = std::dynamic_pointer_cast<PersonAttrData>(gdata);

  EXPECT_EQ(result->personAttrs_.size(), len);

  for (int i = 0; i < len; i++) {
    auto &attrRes  = result->personAttrs_[i];
    auto &attrData = data->personAttrs_[i + startId].attrs;
    fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
    fout_ << data->personAttrs_[i+startId].rect.x<<" "<<data->personAttrs_[i+startId].rect.y << " "
          <<data->personAttrs_[i+startId].rect.width << " " <<data->personAttrs_[i+startId].rect.height<<" ";
    fout_ <<(int)attrRes.age <<" " <<attrRes.ageScore <<" ";
    fout_ <<attrRes.hasBackpack <<" " <<attrRes.hasBackpackScore <<" ";
    fout_ <<attrRes.hasCart <<" " <<attrRes.hasCartScore <<" ";
    fout_ <<attrRes.isFrontHold <<" " <<attrRes.isFrontHoldScore <<" ";
    fout_ <<(int)attrRes.gender <<" " <<attrRes.genderScore <<" ";
    fout_ <<(int)attrRes.glasses <<" " <<attrRes.glassesScore <<" ";
    fout_ <<(int)attrRes.hairColor <<" " <<attrRes.hairColorScore <<" ";
    fout_ <<(int)attrRes.hairLength <<" " <<attrRes.hairLengthScore <<" ";
    fout_ <<attrRes.hasHandbag <<" " <<attrRes.hasHandbagScore <<" ";
    fout_ <<(int)attrRes.headwear <<" " <<attrRes.headwearScore <<" ";
    fout_ <<(int)attrRes.lowerColor <<" " <<attrRes.lowerColorScore <<" ";
    fout_ <<(int)attrRes.lowerTexture <<" " <<attrRes.lowerTextureScore <<" ";
    fout_ <<(int)attrRes.lowerType <<" " <<attrRes.lowerTypeScore <<" ";
    fout_ <<(int)attrRes.hasMask <<" " <<attrRes.hasMaskScore <<" ";
    fout_ <<(int)attrRes.orientation <<" " <<attrRes.orientationScore <<" ";
    fout_ <<(int)attrRes.hasOvercoat <<" " <<attrRes.hasOvercoatScore <<" ";
    fout_ <<(int)attrRes.personType <<" " <<attrRes.personTypeScore <<" ";
    fout_ <<(int)attrRes.hasSingleBag <<" " <<attrRes.hasSingleBagScore <<" ";
    fout_ <<attrRes.hasTrolleyCase <<" " <<attrRes.hasTrolleyCaseScore <<" ";
    fout_ <<(int)attrRes.upperColor <<" " <<attrRes.upperColorScore <<" ";
    fout_ <<(int)attrRes.upperTexture <<" " <<attrRes.upperTextureScore <<" ";
    fout_ <<(int)attrRes.upperType <<" " <<attrRes.upperTypeScore <<" ";
    fout_ <<attrRes.usePhone <<" " <<attrRes.usePhoneScore <<std::endl;

    EXPECT_EQ(attrRes.age, attrData.age);
    EXPECT_EQ(attrRes.hasBackpack, attrData.hasBackpack);
    EXPECT_EQ(attrRes.hasCart, attrData.hasCart);
    EXPECT_EQ(attrRes.isFrontHold, attrData.isFrontHold);
    EXPECT_EQ(attrRes.gender, attrData.gender);
    EXPECT_EQ(attrRes.glasses, attrData.glasses);
    EXPECT_EQ(attrRes.hairColor, attrData.hairColor);
    EXPECT_EQ(attrRes.hairLength, attrData.hairLength);
    EXPECT_EQ(attrRes.hasHandbag, attrData.hasHandbag);
    EXPECT_EQ(attrRes.headwear, attrData.headwear);
    EXPECT_EQ(attrRes.lowerColor, attrData.lowerColor);
    EXPECT_EQ(attrRes.lowerTexture, attrData.lowerTexture);
    EXPECT_EQ(attrRes.lowerType, attrData.lowerType);
    EXPECT_EQ(attrRes.hasMask, attrData.hasMask);
    EXPECT_EQ(attrRes.orientation, attrData.orientation);
    EXPECT_EQ(attrRes.hasOvercoat, attrData.hasOvercoat);
    EXPECT_EQ(attrRes.personType, attrData.personType);
    EXPECT_EQ(attrRes.hasSingleBag, attrData.hasSingleBag);
    EXPECT_EQ(attrRes.hasTrolleyCase, attrData.hasTrolleyCase);
    EXPECT_EQ(attrRes.upperColor, attrData.upperColor);
    EXPECT_EQ(attrRes.upperTexture, attrData.upperTexture);
    EXPECT_EQ(attrRes.upperType, attrData.upperType);
    EXPECT_EQ(attrRes.usePhone, attrData.usePhone);
    EXPECT_NEAR(attrRes.ageScore, attrData.ageScore, 0.02);
    EXPECT_NEAR(attrRes.hasBackpackScore, attrData.hasBackpackScore, 0.02);
    EXPECT_NEAR(attrRes.hasCartScore, attrData.hasCartScore, 0.02);
    EXPECT_NEAR(attrRes.isFrontHoldScore, attrData.isFrontHoldScore, 0.02);
    EXPECT_NEAR(attrRes.genderScore, attrData.genderScore, 0.02);
    EXPECT_NEAR(attrRes.glassesScore, attrData.glassesScore, 0.02);
    EXPECT_NEAR(attrRes.hairColorScore, attrData.hairColorScore, 0.02);
    EXPECT_NEAR(attrRes.hairLengthScore, attrData.hairLengthScore, 0.02);
    EXPECT_NEAR(attrRes.hasHandbagScore, attrData.hasHandbagScore, 0.02);
    EXPECT_NEAR(attrRes.headwearScore, attrData.headwearScore, 0.02);
    EXPECT_NEAR(attrRes.lowerColorScore, attrData.lowerColorScore, 0.02);
    EXPECT_NEAR(attrRes.lowerTextureScore, attrData.lowerTextureScore, 0.02);
    EXPECT_NEAR(attrRes.lowerTypeScore, attrData.lowerTypeScore, 0.02);
    EXPECT_NEAR(attrRes.hasMaskScore, attrData.hasMaskScore, 0.02);
    EXPECT_NEAR(attrRes.orientationScore, attrData.orientationScore, 0.02);
    EXPECT_NEAR(attrRes.hasOvercoatScore, attrData.hasOvercoatScore, 0.02);
    EXPECT_NEAR(attrRes.personTypeScore, attrData.personTypeScore, 0.02);
    EXPECT_NEAR(attrRes.hasSingleBagScore, attrData.hasSingleBagScore, 0.02);
    EXPECT_NEAR(attrRes.hasTrolleyCaseScore, attrData.hasTrolleyCaseScore, 0.02);
    EXPECT_NEAR(attrRes.upperColorScore, attrData.upperColorScore, 0.02);
    EXPECT_NEAR(attrRes.upperTextureScore, attrData.upperTextureScore, 0.02);
    EXPECT_NEAR(attrRes.upperTypeScore, attrData.upperTypeScore, 0.02);
    EXPECT_NEAR(attrRes.usePhoneScore, attrData.usePhoneScore, 0.02);
    EXPECT_NEAR(attrRes.qualityScore,attrData.qualityScore,0.02);
  }
}


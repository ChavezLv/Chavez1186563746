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
#include "src/common/json.hpp"

using arctern::Result;
using arctern::ErrorType;
using arctern::ArcternRect;

using json = nlohmann::json;
void  LicenseRecogData::addGroundTruth(std::string filePath){
  json parser;
  std::ifstream fin(filePath);
  fin >> parser;
  for(int i=0;i<parser.size();i++) {

    ONEPICTURE onepicture;
    auto vLicense = parser.at(i)["license"].get<std::vector<string>>();
    onepicture.license = "";

    for(int j=0;j<vLicense.size();j++){
      onepicture.license += vLicense[j];
    }
    onepicture.scores = parser.at(i)["score"].get<std::vector<float>>();
    files_.push_back(parser.at(i)["filepath"]);
    licenses_.push_back(onepicture);
  }
  fin.close();
}

void  LicenseRecogData::addInputParam(std::string filePath){
  //do nothing
}
void LicenseRecogData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto licenses = node[1]["VALUE"].as<std::vector<std::string>>();
    onepicture.license = licenses.at(0);

    licenses_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void LicenseRecogData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();
#ifdef ATLAS_ALLOWED
    static int atlas_index = 0;
    licenses_[atlas_index].rect.x = rect[0];
    licenses_[atlas_index].rect.y = rect[1];
    licenses_[atlas_index].rect.width = rect[2];
    licenses_[atlas_index++].rect.height = rect[3];
#else
    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        licenses_[i].rect.x = rect[0];
        licenses_[i].rect.y = rect[1];
        licenses_[i].rect.width = rect[2];
        licenses_[i].rect.height = rect[3];
      }
    }
#endif
  }
}

LicenseRecogUnittest::LicenseRecogUnittest(std::string
                                 ymlGroundTruth,
                                 std::string
                                 ymlInput,
                                 std::string
                                 imgDir,
                                 std::string
                                 modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {
  fout_ << "LicenseRecog:imageDir imageName license"<<std::endl;
}

LicenseRecogUnittest::~LicenseRecogUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> LicenseRecogUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::LicenseRecog> ptr(new arctern::LicenseRecog);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> LicenseRecogUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                          int startId,
                                                                          int len) {
  std::shared_ptr<arctern::LicenseRecogRunParameter> ptr(new arctern::LicenseRecogRunParameter);
  std::shared_ptr<LicenseRecogData> data = std::dynamic_pointer_cast<LicenseRecogData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->licenses_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> LicenseRecogUnittest::generateGroundTruth() {
  std::shared_ptr<LicenseRecogData> ptr(new LicenseRecogData);

  auto pos = ymlGroundTruthPath_.find_last_of('.');
  auto suffix = ymlGroundTruthPath_.substr(pos+1);
  if(suffix == "json"){
    ptr->addGroundTruth(ymlGroundTruthPath_);
    ptr->addInputParam(ymlInputPath_);
  }
  else {
    BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
    groundTruth.getGroundTruth(ptr);
    groundTruth.getInputParam(ptr);
  }
  return ptr;
}

void LicenseRecogUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                  int startId,
                                  int len,
                                  const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::LicenseRecogResult *>(p);
  std::shared_ptr<LicenseRecogData> data = std::dynamic_pointer_cast<LicenseRecogData>(gdata);

  EXPECT_EQ(result->LicenseRecogs_.size(), len);

  for (int i = 0; i < len; i++) {
    fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
    fout_ << data->licenses_[i+startId].rect.x<<" "<<data->licenses_[i+startId].rect.y << " "
          << data->licenses_[i+startId].rect.width << " " <<data->licenses_[i+startId].rect.height<<" ";
    fout_ << result->LicenseRecogs_[i] <<std::endl;

    EXPECT_EQ(result->LicenseRecogs_[i], data->licenses_[i + startId].license);
    if(!result->score_.empty()){
      for(int j=0;j<result->score_[i].size();j++){
        EXPECT_NEAR(result->score_[i][j], data->licenses_[i + startId].scores[j],0.005);
      }
    }
  }
}

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "yaml-cpp/yaml.h"
#include "log/logger.h"
#include "yaml-cpp/node/parse.h"
#include "src/common/json.hpp"
using json = nlohmann::json;
void PreventionAttrData::addGroundTruth( std::string filename) {
 json parser;
  std::ifstream fin(filename);
  fin >> parser;
  for(int i=0;i<parser.size();i++) {

    ONEPICTURE onepicture;
    auto attrs = parser[i]["Attributes"];
    
    
    onepicture.attrs.exposeNeckband =arctern::ExposeNeckband( attrs[0][0].get<int>());
    onepicture.attrs.exposeNeckbandScore = attrs[0][1].get<float>();
    onepicture.attrs.touchMask =arctern::TouchMask( attrs[1][0]);
    onepicture.attrs.touchMaskScore = attrs[1][1].get<float>(); 
    onepicture.attrs.playPhone =arctern::PlayPhone( attrs[2][0]);
    onepicture.attrs.playPhoneScore = attrs[2][1].get<float>();
    
    PreventionAttr_.push_back(onepicture);

    files_.push_back(parser.at(i)["filepath"]);
  }
  fin.close();
}

void PreventionAttrData::addInputParam(std::string filename) {

  json parser;
  std::ifstream fin(filename);
  fin >> parser;

  for(int i=0;i<parser.size();i++) {
    int len = files_.size();
    for (int j = 0; j < len; j++) {
      if (files_[j] == parser.at(i)["filepath"]) {
        auto rect = parser[i]["rect"].get<std::vector<int>>();
        PreventionAttr_[j].rect.x = rect[0];
        PreventionAttr_[j].rect.y = rect[1];
        PreventionAttr_[j].rect.width = rect[2];
        PreventionAttr_[j].rect.height = rect[3];
      }
    }
  }
  fin.close();
}

PreventionAttrUnittest::PreventionAttrUnittest(std::string ymlGroundTruth,
                                                       std::string ymlInput,
                                                       std::string imgDir,
                                                       std::string modelpath,
                                                       arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {
  fout_ << "PreventionAttr:imageDir imageName rect gender genderScore age ageScore glasses glasserScore mask maskScore headwear headwearScore"<<std::endl;
}

PreventionAttrUnittest::~PreventionAttrUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> PreventionAttrUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::PreventionAttr> ptr(new arctern::PreventionAttr);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> PreventionAttrUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                               int startId,
                                                                               int len) {
  std::shared_ptr<arctern::PreventionAttrRunParameter> ptr(new arctern::PreventionAttrRunParameter);
  std::shared_ptr<PreventionAttrData> data = std::dynamic_pointer_cast<PreventionAttrData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->PreventionAttr_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> PreventionAttrUnittest::generateGroundTruth() {
  std::shared_ptr<PreventionAttrData> ptr(new PreventionAttrData);

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

void PreventionAttrUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                       int startId,
                                       int len,
                                       const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::PreventionAttrResult *>(p);
  std::shared_ptr<PreventionAttrData> data = std::dynamic_pointer_cast<PreventionAttrData>(gdata);

  EXPECT_EQ(result->PreventionAttrs_.size(), len);

  for (int i = 0; i < len; i++) {
    fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
    fout_ << data->PreventionAttr_[i+startId].rect.x<<" "<<data->PreventionAttr_[i+startId].rect.y << " "
          <<data->PreventionAttr_[i+startId].rect.width << " " <<data->PreventionAttr_[i+startId].rect.height<<" ";
    fout_ <<(int) result->PreventionAttrs_[i].exposeNeckband << " " << result->PreventionAttrs_[i].exposeNeckbandScore << " ";
    fout_ <<(int) result->PreventionAttrs_[i].playPhone <<" "<<result->PreventionAttrs_[i].playPhoneScore << " ";
    fout_ <<(int) result->PreventionAttrs_[i].touchMaskScore << " "<<result->PreventionAttrs_[i].touchMaskScore<<std::endl;

    EXPECT_EQ((int)result->PreventionAttrs_[i].exposeNeckband,(int) data->PreventionAttr_[i + startId].attrs.exposeNeckband);
    EXPECT_EQ((int)result->PreventionAttrs_[i].playPhone,(int) data->PreventionAttr_[i + startId].attrs.playPhone);
    EXPECT_EQ((int)result->PreventionAttrs_[i].touchMask,(int) data->PreventionAttr_[i + startId].attrs.touchMask);
   
    EXPECT_NEAR(result->PreventionAttrs_[i].exposeNeckbandScore, data->PreventionAttr_[i + startId].attrs.exposeNeckbandScore, 0.01);
    EXPECT_NEAR(result->PreventionAttrs_[i].playPhoneScore, data->PreventionAttr_[i + startId].attrs.playPhoneScore, 0.01);
    EXPECT_NEAR(result->PreventionAttrs_[i].touchMaskScore, data->PreventionAttr_[i + startId].attrs.touchMaskScore, 0.01);
  }
}


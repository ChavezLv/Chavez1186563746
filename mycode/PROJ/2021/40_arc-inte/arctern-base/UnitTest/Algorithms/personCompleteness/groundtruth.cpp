/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.07
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

void PersonCompletenessData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
      
    auto visible_softmask = node[1]["VALUE"][0].as<std::vector<int>>();
    auto truncations = node[2]["VALUE"][0].as<std::vector<float>>();
    auto visible_softmask_score = node[3]["VALUE"][0].as<std::vector<float>>();
    assert(visible_softmask.size() == 133); // 128+5
    //assert(visible_softmask_score.size() == 133); // 128+5
    assert(visible_softmask_score.size() == visible_softmask.size()); // 128+5
    assert(truncations.size() == 2);
    
    onepicture.completeness.truncation.assign(truncations.begin(), truncations.end());
    onepicture.completeness.visibleParts.assign(visible_softmask.begin(),
            visible_softmask.begin()+5);
    onepicture.completeness.visiblePartsScore.assign(visible_softmask_score.begin(),
            visible_softmask_score.begin()+5);
    for (int i = 0; i < 16; i++) {
      std::vector<bool> tmp;
      std::vector<float> tmp_score;
      tmp.assign(visible_softmask.begin()+5+i*8,
            visible_softmask.begin()+5+(i+1)*8);
      tmp_score.assign(visible_softmask_score.begin()+5+i*8,
            visible_softmask_score.begin()+5+(i+1)*8);
      onepicture.completeness.softMasks.push_back(tmp);
      onepicture.completeness.softMasksScore.push_back(tmp_score);
    }

    completeness_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void PersonCompletenessData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();
#ifdef ATLAS_ALLOWED
    static int atlas_index = 0;
    completeness_[atlas_index].rect.x = rect[0];
    completeness_[atlas_index].rect.y = rect[1];
    completeness_[atlas_index].rect.width = rect[2];
    completeness_[atlas_index++].rect.height = rect[3];
#else
    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        completeness_[i].rect.x = rect[0];
        completeness_[i].rect.y = rect[1];
        completeness_[i].rect.width = rect[2];
        completeness_[i].rect.height = rect[3];
      }
    }
#endif
  }
}

PersonCompletenessUnittest::PersonCompletenessUnittest(std::string
                                 ymlGroundTruth,
                                 std::string
                                 ymlInput,
                                 std::string
                                 imgDir,
                                 std::string
                                 modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {
  fout_ << "PersonCompleteness:imageDir imageName rect truncation[0] truncation[1]"<<std::endl;
}

PersonCompletenessUnittest::~PersonCompletenessUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> PersonCompletenessUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::PersonCompleteness> ptr(new arctern::PersonCompleteness);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> PersonCompletenessUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                          int startId,
                                                                          int len) {
  std::shared_ptr<arctern::PersonCompletenessRunParameter> ptr(new arctern::PersonCompletenessRunParameter);
  std::shared_ptr<PersonCompletenessData> data = std::dynamic_pointer_cast<PersonCompletenessData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->completeness_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> PersonCompletenessUnittest::generateGroundTruth() {
  std::shared_ptr<PersonCompletenessData> ptr(new PersonCompletenessData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void PersonCompletenessUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                  int startId,
                                  int len,
                                  const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::PersonCompletenessResult *>(p);
  std::shared_ptr<PersonCompletenessData> data = std::dynamic_pointer_cast<PersonCompletenessData>(gdata);

  EXPECT_EQ(result->personCompleteness_.size(), len);

  for (int i = 0; i < len; i++) {
    fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
    fout_ << data->completeness_[i+startId].rect.x<<" "<<data->completeness_[i+startId].rect.y << " "
          << data->completeness_[i+startId].rect.width << " " <<data->completeness_[i+startId].rect.height<<" ";
    fout_ << result->personCompleteness_[i].truncation[0] <<" " << result->personCompleteness_[i].truncation[1]<<std::endl;

    EXPECT_EQ(result->personCompleteness_[i].truncation.size(), 2);
    EXPECT_EQ(result->personCompleteness_[i].visibleParts.size(), 5);
    EXPECT_EQ(result->personCompleteness_[i].visiblePartsScore.size(), 5);
    EXPECT_EQ(data->completeness_[i + startId].completeness.truncation.size(), 2);
    EXPECT_EQ(data->completeness_[i + startId].completeness.visibleParts.size(), 5);
    EXPECT_EQ(data->completeness_[i + startId].completeness.visiblePartsScore.size(), 5);

    EXPECT_NEAR(result->personCompleteness_[i].truncation.at(0),
            data->completeness_[i + startId].completeness.truncation.at(0), 0.01);
    EXPECT_NEAR(result->personCompleteness_[i].truncation.at(1),
            data->completeness_[i + startId].completeness.truncation.at(1), 0.01);
  }
}

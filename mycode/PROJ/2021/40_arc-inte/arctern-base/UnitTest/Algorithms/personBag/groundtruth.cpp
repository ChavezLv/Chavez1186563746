/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.09
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "include/autoTime.h"
#include "log/logger.h"
#include "yaml-cpp/node/parse.h"
#include "yaml-cpp/yaml.h"
#include "gtest/gtest.h"

void personBagData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto type = node[1]["VALUE"][0].as<int>();
    onepicture.result.first = type;

    auto score = node[2]["VALUE"][0].as<float>();
    onepicture.result.second = score;

    m_vResults.push_back(onepicture);
    files_.push_back(filename);
  }
}

void personBagData::addInputParam(YAML::Node &node, std::string filename) {
  if (node.size() > 1) {
    auto rect = node[2]["VALUE"].as<std::vector<float>>();

    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        m_vResults[i].rect.x = rect[0];
        m_vResults[i].rect.y = rect[1];
        m_vResults[i].rect.width = rect[2];
        m_vResults[i].rect.height = rect[3];
      }
    }
  }
}

personBagUnittest::personBagUnittest(std::string ymlGroundTruth,
                                     std::string ymlInput, std::string imgDir,
                                     std::string modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {
  fout_ << "PersonBag:imageDir imageName rect bag score"<<std::endl;
}

personBagUnittest::~personBagUnittest() {}

std::shared_ptr<arctern::AlgorithmInterface>
personBagUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::PersonBag> ptr(new arctern::PersonBag);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter>
personBagUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                     int startId, int len) {
  std::shared_ptr<arctern::PersonBagRunParameter> ptr(
      new arctern::PersonBagRunParameter);
  std::shared_ptr<personBagData> data =
      std::dynamic_pointer_cast<personBagData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->m_vResults[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> personBagUnittest::generateGroundTruth() {
  std::shared_ptr<personBagData> ptr(new personBagData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void personBagUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                    int startId, int len,
                                    const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::PersonBagResult *>(p);
  std::shared_ptr<personBagData> data =
      std::dynamic_pointer_cast<personBagData>(gdata);

  EXPECT_EQ(result->m_vClassifyResults.size(), len);

  for (int i = 0; i < len; i++) {
    auto &classRes = result->m_vClassifyResults[i];
    auto &calssData = data->m_vResults[i + startId].result;

    fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
    fout_ << data->m_vResults[i+startId].rect.x<<" "<<data->m_vResults[i+startId].rect.y << " "
          << data->m_vResults[i+startId].rect.width << " " <<data->m_vResults[i+startId].rect.height<<" ";
    fout_ << result->m_vClassifyResults[i].first <<" " << result->m_vClassifyResults[i].second<<std::endl;

    EXPECT_EQ(classRes.first, calssData.first);
    EXPECT_NEAR(classRes.second, calssData.second, 0.01);
  }
}

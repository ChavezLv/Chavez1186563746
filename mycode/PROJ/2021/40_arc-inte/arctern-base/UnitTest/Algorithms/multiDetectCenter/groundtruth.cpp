/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.11.27
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "yaml-cpp/yaml.h"
#include "log/logger.h"
#include "yaml-cpp/node/parse.h"
#include "src/common/json.hpp"
using json = nlohmann::json;
void  MultiDetectCenterData::addGroundTruth(std::string filePath){
  json parser;
  std::ifstream fin(filePath);
  fin >> parser;
  for(int i=0;i<parser.size();i++) {

    ONEPICTURE onepicture;

    auto rects = parser[i]["rects"];
    for(int j=0;j < rects.size();j++){
      onepicture.types.push_back(rects[j][0].get<int>() + 3);
      onepicture.confidences.push_back(rects[j][1].get<float>());
      onepicture.rects.push_back({(float )rects[j][3].get<int>(),(float )rects[j][4].get<int>(),
          (float )rects[j][5].get<int>() - (float )rects[j][3].get<int>(),
          (float )rects[j][6].get<int>() - (float )rects[j][4].get<int>()});
    }

    files_.push_back(parser.at(i)["filepath"]);
    multicatRects_.push_back(onepicture);
  }
  fin.close();
}
void MultiDetectCenterData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();
    auto type = node[2]["VALUE"].as<std::vector<int>>();
    auto confidence = node[3]["VALUE"].as<std::vector<float>>();
    int len = confidence.size();

    for (int i = 0; i < len; i++) {
      onepicture.confidences.emplace_back(confidence[i]);
      onepicture.types.emplace_back(type[i]);
      onepicture.rects.emplace_back(arctern::ArcternRect{rect[0 + 4 * i], rect[1 + 4 * i], rect[2 + 4 * i],
                                                         rect[3 + 4 * i]});
    }
  }
  multicatRects_.push_back(onepicture);
  files_.push_back(filename);
}

MultiDetectCenterUnittest::MultiDetectCenterUnittest(std::string ymlPath, std::string imgDir, std::string modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlPath, "", imgDir, modelpath,p) {
  fout_ << "MultiDetectCenter:imageDir imageName rect type" <<std::endl;
}

MultiDetectCenterUnittest::~MultiDetectCenterUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> MultiDetectCenterUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::MultiDetectCenter> ptr(new arctern::MultiDetectCenter);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;
  initParam.thresh_ = 0.3;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> MultiDetectCenterUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                                 int startId,
                                                                                 int len) {
  std::shared_ptr<arctern::RunParameter> ptr(new arctern::MultiDetectCenterRunParameter);
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> MultiDetectCenterUnittest::generateGroundTruth() {
  std::shared_ptr<MultiDetectCenterData> ptr(new MultiDetectCenterData);
  auto pos = ymlGroundTruthPath_.find_last_of('.');
  auto suffix = ymlGroundTruthPath_.substr(pos+1);
  if(suffix == "json"){
    ptr->addGroundTruth(ymlGroundTruthPath_);
  }
  else {
    BaseGroundTruth groundTruth(ymlGroundTruthPath_, "");
    groundTruth.getGroundTruth(ptr);
  }
  return ptr;
}

void MultiDetectCenterUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                         int startId,
                                         int len,
                                         const arctern::Result *p) {
  EXPECT_NE(p, nullptr);

  auto result = dynamic_cast<const arctern::MultiDetectCenterResult *>(p);
  std::shared_ptr<MultiDetectCenterData> data = std::dynamic_pointer_cast<MultiDetectCenterData>(gdata);

  EXPECT_EQ(static_cast<int>(result->detInfos_.size()), len);

  for (int i = 0; i < len; i++) {
    size_t rectNum = result->detInfos_[i].size();
    EXPECT_EQ(rectNum, data->multicatRects_[i + startId].rects.size());
    for (size_t j = 0; j < rectNum; j++) {
      spdlog::info("filename={0}\n",data->files_[i+startId]);
      spdlog::info("rect = [{0}, {1}, {2}, {3}]  {4}  {5}\n", result->detInfos_[i][j].rect.x, result->detInfos_[i][j].rect.y,
                   result->detInfos_[i][j].rect.width, result->detInfos_[i][j].rect.height,
                   static_cast<int>(result->detInfos_[i][j].type), result->detInfos_[i][j].score);

      fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
      fout_ << result->detInfos_[i][j].rect.x << " " <<result->detInfos_[i][j].rect.y << " " <<
            result->detInfos_[i][j].rect.width << " " <<result->detInfos_[i][j].rect.height<<" "<< result->detInfos_[i][j].score<<" ";
      fout_ <<(int) result->detInfos_[i][j].type<<std::endl;

      EXPECT_NEAR(result->detInfos_[i][j].score, data->multicatRects_[i + startId].confidences[j], 0.005);
      EXPECT_EQ(static_cast<int>(result->detInfos_[i][j].type), data->multicatRects_[i + startId].types[j]);
      EXPECT_NEAR(result->detInfos_[i][j].rect.x, data->multicatRects_[i + startId].rects[j].x, 2);
      EXPECT_NEAR(result->detInfos_[i][j].rect.y, data->multicatRects_[i + startId].rects[j].y, 2);
      EXPECT_NEAR(result->detInfos_[i][j].rect.width, data->multicatRects_[i + startId].rects[j].width, 2);
      EXPECT_NEAR(result->detInfos_[i][j].rect.height, data->multicatRects_[i + startId].rects[j].height, 2);
    }
  }
}

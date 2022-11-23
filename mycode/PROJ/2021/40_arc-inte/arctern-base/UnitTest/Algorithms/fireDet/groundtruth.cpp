/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "yaml-cpp/yaml.h"
#include "log/logger.h"
#include "yaml-cpp/node/parse.h"
#include "src/common/json.hpp"
using json = nlohmann::json;
void FireDetData::addGroundTruth(std::string filePath){
  json parser;
  std::ifstream fin(filePath);
  fin >> parser;
  for(int i=0;i<parser.size();i++) {

    ONEPICTURE onepicture;

    auto rects = parser[i]["rects"];
    for(int j=0;j < rects.size();j++){
      if(filePath.find("smoke-det") == std::string::npos)
      {
        onepicture.types.push_back(rects[j][5].get<int>());
      } else{
        onepicture.types.push_back(rects[j][5].get<int>() + 1);
      }

      onepicture.confidences.push_back(rects[j][4].get<float>());
      onepicture.rects.push_back({(float )rects[j][0].get<int>(),(float )rects[j][1].get<int>(),
                                  (float )rects[j][2].get<int>() - (float )rects[j][0].get<int>(),
                                  (float )rects[j][3].get<int>() - (float )rects[j][1].get<int>()});
    }

    files_.push_back(parser.at(i)["filepath"]);
    rects_.push_back(onepicture);
  }
  fin.close();
}
void FireDetData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();
    auto confidence = node[2]["VALUE"].as<std::vector<float>>();
    auto type = node[3]["VALUE"].as<std::vector<int>>();
    int len = confidence.size();

    for (int i = 0; i < len; i++) {
      onepicture.confidences.emplace_back(confidence[i]);
      onepicture.rects.emplace_back(arctern::ArcternRect{rect[0 + 4 * i], rect[1 + 4 * i], rect[2 + 4 * i],
                                                         rect[3 + 4 * i]});
      onepicture.types.emplace_back(type[i]);
    }
  }
  rects_.push_back(onepicture);
  files_.push_back(filename);
}

FireDetUnittest::FireDetUnittest(std::string ymlPath, std::string imgDir, std::string modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlPath, "", imgDir, modelpath,p) {
}

FireDetUnittest::~FireDetUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> FireDetUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FireDet> ptr(new arctern::FireDet);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;
  initParam.thresh_ = 0.001;

  auto ret = ptr->Init(&initParam);
  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> FireDetUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                                 int startId,
                                                                                 int len) {
  std::shared_ptr<arctern::RunParameter> ptr(new arctern::FireDetRunParameter);
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FireDetUnittest::generateGroundTruth() {
  std::shared_ptr<FireDetData> ptr(new FireDetData);
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

pair<int,float> GetNearestRectIndex(const ONEPICTURE& data,arctern::ArcternRect rect){
  float maxIOU = 0;
  int index = 0;
  for(int i=0;i<data.rects.size();i++){
    float minx = std::max(data.rects[i].x, rect.x);
    float miny = std::max(data.rects[i].y, rect.y);
    float maxx = std::min(data.rects[i].x + data.rects[i].width, rect.x + rect.width);
    float maxy = std::min(data.rects[i].y + data.rects[i].height, rect.y + rect.height);
    float w = std::max(float(0), maxx - minx + 1);
    float h = std::max(float(0), maxy - miny + 1);
    float interarea = w * h;
    float iou = interarea / (data.rects[i].width*data.rects[i].height +
        rect.width * rect.height- interarea);
    if(iou > maxIOU){
      maxIOU = iou;
      index = i;
    }
  }
  return make_pair(index,maxIOU);
}

void FireDetUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                         int startId,
                                         int len,
                                         const arctern::Result *p) {
  EXPECT_NE(p, nullptr);

  auto result = dynamic_cast<const arctern::FireDetResult *>(p);
  std::shared_ptr<FireDetData> data = std::dynamic_pointer_cast<FireDetData>(gdata);

  EXPECT_EQ(static_cast<int>(result->detInfos_.size()), len);

  for (int i = 0; i < len; i++) {
    spdlog::info("[filename = {0}]\n", data->files_[i + startId]);
    size_t rectNum = result->detInfos_[i].size();

    EXPECT_EQ(rectNum, data->rects_[i + startId].rects.size());
    for (size_t j = 0; j < rectNum; j++) {
      spdlog::info("rect = [{0}, {1}, {2}, {3}]  {4}\n", result->detInfos_[i][j].rect.x, result->detInfos_[i][j].rect.y,
                   result->detInfos_[i][j].rect.width, result->detInfos_[i][j].rect.height, result->detInfos_[i][j].score);
      auto ret = GetNearestRectIndex(data->rects_[i+startId],result->detInfos_[i][j].rect);
      int index = j;
      spdlog::info("[index = {0},iou = {1}]\n", index,ret.second);
      if(ret.second <= 0) continue;
      EXPECT_NEAR(result->detInfos_[i][j].score, data->rects_[i + startId].confidences[index], 0.005);
      EXPECT_NEAR(result->detInfos_[i][j].rect.x, data->rects_[i + startId].rects[index].x, 2);
      EXPECT_NEAR(result->detInfos_[i][j].rect.y, data->rects_[i + startId].rects[index].y, 2);
      EXPECT_NEAR(result->detInfos_[i][j].rect.width, data->rects_[i + startId].rects[index].width, 2);
      EXPECT_NEAR(result->detInfos_[i][j].rect.height, data->rects_[i + startId].rects[index].height, 2);
      EXPECT_EQ(result->detInfos_[i][j].type,data->rects_[i+startId].types[index]);
    }
  }
}

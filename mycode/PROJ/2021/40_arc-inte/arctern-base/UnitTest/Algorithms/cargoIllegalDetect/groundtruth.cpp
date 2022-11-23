/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         author
 *  Last modified:  2021.12.21
 *  email:          nullptr
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "yaml-cpp/yaml.h"
#include "log/logger.h"
#include "common/featureUtil.h"

#include "yaml-cpp/node/parse.h"
#include "src/common/json.hpp"

#include "../../../src/Algorithms/cargoIllegalDetect/cargoIllegalDetect.h"

#include <fstream>
#include <functional>

using json = nlohmann::json;

void CargoIllegalDetectData::addGroundTruth(YAML::Node &node, std::string filename) {

  if(node.Type() == YAML::NodeType::Null){
    json parser;
    std::ifstream fin(filename);
    fin >> parser;
    /* for(size_t i = 0; i < parser.size(); ++i) { */
    for(size_t i = 0; i < 1; ++i) {
      ONEPICTURE onepicture;
      for(size_t rectStart = 0; rectStart < parser[i]["rects"].size();\
          ++ rectStart){
        auto rect = parser[i]["rects"][rectStart].get<std::vector<float> >();
        onepicture.rects.emplace_back(arctern::ArcternRect{\
                    rect[0], rect[1], rect[2] - rect[0], rect[3] - rect[1]});
        onepicture.confidences.emplace_back(rect[4]);
        onepicture.types.emplace_back(rect[5]);
      }
      rects_.emplace_back(onepicture);
      files_.emplace_back(parser[i]["filepath"].get<std::string>());

    }//end of for(size_t i = 0; i < parser.size(); ++i)
    fin.close();

  }else{
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

  }//end of if(node.Type() == YAML::NodeType::Null)
}

CargoIllegalDetectUnittest::CargoIllegalDetectUnittest(std::string ymlPath, std::string imgDir, std::string modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlPath, "", imgDir, modelpath,p) {
  fout_ << "CargoIllegalDetect:imageDir imageName rect score" <<std::endl;

}

CargoIllegalDetectUnittest::~CargoIllegalDetectUnittest() {

}

std::shared_ptr<arctern::AlgorithmInterface> CargoIllegalDetectUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::CargoIllegalDetect> ptr(new arctern::CargoIllegalDetect);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;
  initParam.thresh_ = 0.001;
  auto ret = ptr->Init(&initParam);
  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> CargoIllegalDetectUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                                 int startId,
                                                                                 int len) {
  std::shared_ptr<arctern::RunParameter> ptr(new arctern::CargoIllegalDetectRunParameter);
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> CargoIllegalDetectUnittest::generateGroundTruth() {
  std::shared_ptr<CargoIllegalDetectData> ptr(new CargoIllegalDetectData);
  YAML::Node node(YAML::NodeType::Null);

  string flag(".");
  string::size_type nposRet = ymlGroundTruthPath_.find_last_of(flag);
  assert(nposRet != string::npos);
  std::string suffix = ymlGroundTruthPath_.substr(nposRet + string(flag).size());

  if(suffix == "json"){
      std::bind(&CargoIllegalDetectData::addGroundTruth, ptr, node, ymlGroundTruthPath_)();
  }else if(suffix == "yml"){
    BaseGroundTruth groundTruth(ymlGroundTruthPath_, "");
    groundTruth.getGroundTruth(ptr);

  }
  return ptr;

}

pair<int,float> GetNearestRectIndex(const ONEPICTURE& data,arctern::ArcternRect rect){
  float maxIOU = 0;
  int index = 0;
  for(int i=0;i<(int)data.rects.size();i++){
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

void CargoIllegalDetectUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                         int startId,
                                         int len,
                                         const arctern::Result *p) {
  EXPECT_NE(p, nullptr);

  auto result = dynamic_cast<const arctern::CargoIllegalDetectResult *>(p);
  std::shared_ptr<CargoIllegalDetectData> data = std::dynamic_pointer_cast<CargoIllegalDetectData>(gdata);
  EXPECT_EQ(static_cast<int>(result->detInfos_.size()), len);



  for (int i = 0; i < len; i++) {
    spdlog::info("[filename = {0}]\n", data->files_[i + startId]);
    size_t rectNum = result->detInfos_[i].size();

    EXPECT_EQ(rectNum, data->rects_[i + startId].rects.size());
    for (size_t j = 0; j < rectNum; j++) {
      spdlog::info("rect = [{0}, {1}, {2}, {3}]  {4}\n", result->detInfos_[i][j].rect.x, result->detInfos_[i][j].rect.y,
                   result->detInfos_[i][j].rect.width, result->detInfos_[i][j].rect.height, result->detInfos_[i][j].score);

      fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
      fout_ << result->detInfos_[i][j].rect.x << " " <<result->detInfos_[i][j].rect.y << " " <<
          result->detInfos_[i][j].rect.width << " " <<result->detInfos_[i][j].rect.height<<" "<< result->detInfos_[i][j].score;
      fout_ << std::endl;

      auto ret = GetNearestRectIndex(data->rects_[i+startId],result->detInfos_[i][j].rect);
      int index=ret.first;
      spdlog::info("[index = {0},iou = {1}]\n", index,ret.second);
      if(ret.second <= 0) continue;
      EXPECT_NEAR(result->detInfos_[i][j].score, data->rects_[i + startId].confidences[index], 0.005);
      EXPECT_NEAR(result->detInfos_[i][j].rect.x, data->rects_[i + startId].rects[index].x, 2);
      EXPECT_NEAR(result->detInfos_[i][j].rect.y, data->rects_[i + startId].rects[index].y, 2);
      EXPECT_NEAR(result->detInfos_[i][j].rect.width, data->rects_[i + startId].rects[index].width, 2);
      EXPECT_NEAR(result->detInfos_[i][j].rect.height, data->rects_[i + startId].rects[index].height, 2);
    }
  }
}

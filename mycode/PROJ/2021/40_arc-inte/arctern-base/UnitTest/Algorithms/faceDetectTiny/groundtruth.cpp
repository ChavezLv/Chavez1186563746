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
#include "common/featureUtil.h"

#include "yaml-cpp/node/parse.h"
#include "src/common/json.hpp"

#include "../../../src/Algorithms/faceDetectTiny/faceDetectTiny.h"

#include <fstream>
#include <functional>

using json = nlohmann::json;

void FaceDetectTinyData::addGroundTruth(YAML::Node &node, std::string filename) {
  if(node.Type() == YAML::NodeType::Null){
    json parser;
    std::ifstream fin(filename);
    fin >> parser;
    for(size_t i = 0; i < parser.size(); ++i) {
      ONEPICTURE onepicture;

      files_.emplace_back(parser[i]["filepath"].get<std::string>());
      for(size_t rectStart = 0; rectStart < parser[i]["rects"].size();\
          ++ rectStart){
        auto rect = parser[i]["rects"][rectStart].get<std::vector<float> >();
        onepicture.rects.emplace_back(arctern::ArcternRect{\
                    rect[0], rect[1], rect[2] - rect[0], rect[3] - rect[1]});
        onepicture.confidences.emplace_back(rect[4]);
      }
      faceRects_.emplace_back(onepicture);
    }
    fin.close();
  }else{
    ONEPICTURE onepicture;
    if (node.size() > 1) {
      auto rect = node[1]["VALUE"].as<std::vector<float>>();
      auto confidence = node[2]["VALUE"].as<std::vector<float>>();
      int len = confidence.size();

      for (int i = 0; i < len; i++) {
        onepicture.confidences.emplace_back(confidence[i]);
        onepicture.rects.emplace_back(arctern::ArcternRect{rect[0 + 4 * i], rect[1 + 4 * i], rect[2 + 4 * i],
                                                           rect[3 + 4 * i]});
      }
    }
    faceRects_.push_back(onepicture);
    files_.push_back(filename);
  }
}

FaceDetectTinyUnittest::FaceDetectTinyUnittest(std::string ymlPath, std::string imgDir, std::string modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlPath, "", imgDir, modelpath,p) {
  fout_ << "FaceDetectTiny:imageDir imageName rect score" <<std::endl;
}

FaceDetectTinyUnittest::~FaceDetectTinyUnittest() {

}

std::shared_ptr<arctern::AlgorithmInterface> FaceDetectTinyUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceDetectTiny> ptr(new arctern::FaceDetectTiny);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;
  initParam.thresh_ = 0.38;

  auto ret = ptr->Init(&initParam);
  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> FaceDetectTinyUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                                 int startId,
                                                                                 int len) {
  std::shared_ptr<arctern::RunParameter> ptr(new arctern::FaceDetectTinyRunParameter);
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FaceDetectTinyUnittest::generateGroundTruth() {
  std::shared_ptr<FaceDetectTinyData> ptr(new FaceDetectTinyData);

  auto pos = ymlGroundTruthPath_.find_last_of('.');
  auto suffix = ymlGroundTruthPath_.substr(pos+1);
  YAML::Node node(YAML::NodeType::Null);
  std::shared_ptr<YAML::Node> spNode(new YAML::Node);
  if(suffix == "json"){
    std::bind(&FaceDetectTinyData::addGroundTruth, ptr, node, ymlGroundTruthPath_)();
  }
  else if(suffix == "yml") {
    BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
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

void FaceDetectTinyUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                         int startId,
                                         int len,
                                         const arctern::Result *p) {
  EXPECT_NE(p, nullptr);

  auto result = dynamic_cast<const arctern::FaceDetectTinyResult *>(p);
  std::shared_ptr<FaceDetectTinyData> data = std::dynamic_pointer_cast<FaceDetectTinyData>(gdata);
  EXPECT_EQ(static_cast<int>(result->detInfos_.size()), len);



  for (int i = 0; i < len; i++) {
    spdlog::info("[filename = {0}]\n", data->files_[i + startId]);
    size_t rectNum = result->detInfos_[i].size();

    EXPECT_EQ(rectNum, data->faceRects_[i + startId].rects.size());
    for (size_t j = 0; j < rectNum; j++) {
      spdlog::info("rect = [{0}, {1}, {2}, {3}]  {4}\n", result->detInfos_[i][j].rect.x, result->detInfos_[i][j].rect.y,
                   result->detInfos_[i][j].rect.width, result->detInfos_[i][j].rect.height, result->detInfos_[i][j].score);

      fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
      fout_ << result->detInfos_[i][j].rect.x << " " <<result->detInfos_[i][j].rect.y << " " <<
          result->detInfos_[i][j].rect.width << " " <<result->detInfos_[i][j].rect.height<<" "<< result->detInfos_[i][j].score;
      fout_ << std::endl;

      auto ret = GetNearestRectIndex(data->faceRects_[i+startId],result->detInfos_[i][j].rect);
      int index=ret.first;
      spdlog::info("[index = {0},iou = {1}]\n", index,ret.second);
      if(ret.second <= 0) continue;
      EXPECT_NEAR(result->detInfos_[i][j].score, data->faceRects_[i + startId].confidences[index], 0.005);
      EXPECT_NEAR(result->detInfos_[i][j].rect.x, data->faceRects_[i + startId].rects[index].x, 2);
      EXPECT_NEAR(result->detInfos_[i][j].rect.y, data->faceRects_[i + startId].rects[index].y, 2);
      EXPECT_NEAR(result->detInfos_[i][j].rect.width, data->faceRects_[i + startId].rects[index].width, 2);
      EXPECT_NEAR(result->detInfos_[i][j].rect.height, data->faceRects_[i + startId].rects[index].height, 2);
    }
  }
}

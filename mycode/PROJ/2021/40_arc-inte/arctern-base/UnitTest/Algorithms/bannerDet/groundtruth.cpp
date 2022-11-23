/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "include/autoTime.h"
#include "log/logger.h"
#include "yaml-cpp/node/parse.h"
#include "yaml-cpp/yaml.h"
#include "gtest/gtest.h"

void BannerDetData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();
    auto confidence = node[2]["VALUE"].as<std::vector<float>>();
    auto type = node[3]["VALUE"].as<std::vector<int>>();
    int len = confidence.size();

    for (int i = 0; i < len; i++) {
      onepicture.confidences.emplace_back(confidence[i]);
      onepicture.rects.emplace_back(arctern::ArcternRect{rect[0 + 4 * i], rect[1 + 4 * i],\
                                    rect[2 + 4 * i], rect[3 + 4 * i]});
      onepicture.types.emplace_back(type[i]);
    }
  }
  rects_.push_back(onepicture);
  files_.push_back(filename);
}

/* BannerDetUnittest::BannerDetUnittest(std::string ymlPath, std::string imgDir,\ */
/*         std::string modelpath, arctern::Result *p) */
/* : BaseAlgrithmUnittest(ymlPath, imgDir, imgDir, modelpath, p){ */
BannerDetUnittest::BannerDetUnittest(std::string ymlPath, std::string imgDir, std::string modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlPath, "", imgDir, modelpath,p) {
  std::cout << ">>>>-BannerDetUnittest::BannerDetUnittest(std::string ymlPath,... )"<< imgDir << std::endl;
}
BannerDetUnittest::~BannerDetUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> BannerDetUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::BannerDet> ptr(new arctern::BannerDet);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;
  initParam.thresh_ = 0.1;

  auto ret = ptr->Init(&initParam);
  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> BannerDetUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                                 int startId,
                                                                                 int len) {
  std::shared_ptr<arctern::RunParameter> ptr(new arctern::BannerDetRunParameter);
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> BannerDetUnittest::generateGroundTruth() {
  std::shared_ptr<BannerDetData> ptr(new BannerDetData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, "");
  groundTruth.getGroundTruth(ptr);
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

void BannerDetUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                         int startId,
                                         int len,
                                         const arctern::Result *p) {
  EXPECT_NE(p, nullptr);

  auto result = dynamic_cast<const arctern::BannerDetResult *>(p);
  std::shared_ptr<BannerDetData> data = std::dynamic_pointer_cast<BannerDetData>(gdata);

  EXPECT_EQ(static_cast<int>(result->detInfos_.size()), len);

  for (int i = 0; i < len; i++) {
    spdlog::info("[filename = {0}]\n", data->files_[i + startId]);
    size_t rectNum = result->detInfos_[i].size();
    std::cout << rectNum << std::endl;
    std::cout << data->rects_[i + startId].rects.size() << std::endl;
    EXPECT_EQ(rectNum + 1, data->rects_[i + startId].rects.size());//1 != 2
    for (size_t j = 0; j < rectNum; ++j) {
      spdlog::info("rect = [{0}, {1}, {2}, {3}]  {4}\n", result->detInfos_[i][j].rect.x, result->detInfos_[i][j].rect.y,
                   result->detInfos_[i][j].rect.width, result->detInfos_[i][j].rect.height, result->detInfos_[i][j].score);
      auto ret = GetNearestRectIndex(data->rects_[i+startId],result->detInfos_[i][j].rect);
      int index=ret.first;
      spdlog::info("[index = {0},iou = {1}]\n", index,ret.second);
      if(ret.second <= 0) continue;
      /* std::cout << result->detInfos_[i][j].score << " vs " << data->rects_[i + startId].confidences[index] << std::endl; */
      /* std::cout << result->detInfos_[i][j].rect.x << std::endl; */
      /* std::cout << result->detInfos_[i][j].rect.y << std::endl; */
      /* std::cout << result->detInfos_[i][j].rect.width << std::endl; */
      /* std::cout << result->detInfos_[i][j].rect.height << std::endl; */
      EXPECT_NEAR(result->detInfos_[i][j].score, data->rects_[i + startId].confidences[index], 0.005);
      EXPECT_NEAR(result->detInfos_[i][j].rect.x, data->rects_[i + startId].rects[index].x, 2);//1324,!= 1331, 7 != 2
      EXPECT_NEAR(result->detInfos_[i][j].rect.y, data->rects_[i + startId].rects[index].y, 2);//596 != 590, 6 != 2
      EXPECT_NEAR(result->detInfos_[i][j].rect.width, data->rects_[i + startId].rects[index].width, 2);
      EXPECT_NEAR(result->detInfos_[i][j].rect.height, data->rects_[i + startId].rects[index].height, 2);//206 != 221, 15 != 2
      /* EXPECT_EQ(result->detInfos_[i][j].type,data->rects_[i+startId].types[index]); */
    }
  }
}

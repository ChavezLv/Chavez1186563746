/****************************************************************************
 *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         kuanghongshen
 *  Last modified:  2021.06.07
 *  email:          kuang.hongshen@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "include/autoTime.h"
#include "log/logger.h"
#include "yaml-cpp/node/parse.h"
#include "yaml-cpp/yaml.h"
#include "gtest/gtest.h"

void dogDetTinyData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();
    auto confidence = node[2]["VALUE"].as<std::vector<float>>();
    int len = confidence.size();

    for (int i = 0; i < len; i++) {
      onepicture.confidences.emplace_back(confidence[i]);
      int i_offset = i * 4;
      onepicture.rects.emplace_back(
          arctern::ArcternRect{rect[0 + i_offset], rect[1 + i_offset],
                               rect[2 + i_offset], rect[3 + i_offset]});
    }
  }
  faceRects_.push_back(onepicture);
  files_.push_back(filename);
}

dogDetTinyUnittest::dogDetTinyUnittest(std::string ymlPath, std::string imgDir,
                                       std::string modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlPath, "", imgDir, modelpath,p) {}

dogDetTinyUnittest::~dogDetTinyUnittest() {}

std::shared_ptr<arctern::AlgorithmInterface>
dogDetTinyUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::dogDetTiny> ptr(new arctern::dogDetTiny);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;
  initParam.thresh_ = 0.4;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> dogDetTinyUnittest::buildRunParameter(
    std::shared_ptr<BaseGroundTruthData> gdata, int startId, int len) {
  std::shared_ptr<arctern::RunParameter> ptr(new arctern::dogDetTinyRunParameter);
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> dogDetTinyUnittest::generateGroundTruth() {
  std::shared_ptr<dogDetTinyData> ptr(new dogDetTinyData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, "");
  groundTruth.getGroundTruth(ptr);
  return ptr;
}

void dogDetTinyUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                     int startId, int len,
                                     const arctern::Result *p) {
  EXPECT_NE(p, nullptr);

  auto result = dynamic_cast<const arctern::dogDetTinyResult *>(p);
  std::shared_ptr<dogDetTinyData> data =
      std::dynamic_pointer_cast<dogDetTinyData>(gdata);

  EXPECT_EQ(static_cast<int>(result->detInfos_.size()), len);

  for (int i = 0; i < len; i++) {
    size_t rectNum = result->detInfos_[i].size();

    EXPECT_EQ(rectNum, data->faceRects_[i + startId].rects.size());
    for (size_t j = 0; j < rectNum; j++) {

      spdlog::info(
          "rect = [{0}, {1}, {2}, {3}]  {4}\n", result->detInfos_[i][j].rect.x,
          result->detInfos_[i][j].rect.y, result->detInfos_[i][j].rect.width,
          result->detInfos_[i][j].rect.height, result->detInfos_[i][j].score);
      EXPECT_NEAR(result->detInfos_[i][j].score,
                  data->faceRects_[i + startId].confidences[j], 0.005);
      EXPECT_NEAR(result->detInfos_[i][j].rect.x,
                  data->faceRects_[i + startId].rects[j].x, 2);
      EXPECT_NEAR(result->detInfos_[i][j].rect.y,
                  data->faceRects_[i + startId].rects[j].y, 2);
      EXPECT_NEAR(result->detInfos_[i][j].rect.width,
                  data->faceRects_[i + startId].rects[j].width, 2);
      EXPECT_NEAR(result->detInfos_[i][j].rect.height,
                  data->faceRects_[i + startId].rects[j].height, 2);
    }
  }
}

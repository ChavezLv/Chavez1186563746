/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.13
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"

#include "gtest/gtest.h"
#include "src/Algorithms/faceDetectRTN/faceDetectRTN.h"
#include "yaml-cpp/yaml.h"
#include "yaml-cpp/node/parse.h"

FaceDetectRTNData::FaceDetectRTNData() {
}

FaceDetectRTNData::~FaceDetectRTNData() {
}

void FaceDetectRTNData::addGroundTruth(YAML::Node &node, std::string filename) {
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

FaceDetectRTNUnittest::FaceDetectRTNUnittest(std::string ymlPath, std::string imgDir, std::string modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlPath, "", imgDir, modelpath,p) {
}

FaceDetectRTNUnittest::~FaceDetectRTNUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> FaceDetectRTNUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceDetectRTN> ptr(new arctern::FaceDetectRTN);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;
//  initParam.deviceType_ = deviceType_;
//  initParam.gpuId_ = gpuId_;
//  initParam.typeBits_ = typeBits_;
  initParam.thresh_ = 0.6;

  auto ret = ptr->Init(&initParam);

  std::cout<<"--------------------->ret ="<<ret<<std::endl;

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> FaceDetectRTNUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                                int startId,
                                                                                int len) {
  std::shared_ptr<arctern::RunParameter> ptr(new arctern::FaceDetectRTNRunParameter);
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FaceDetectRTNUnittest::generateGroundTruth() {
  std::shared_ptr<FaceDetectRTNData> ptr(new FaceDetectRTNData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, "");
  groundTruth.getGroundTruth(ptr);
  return ptr;
}

void FaceDetectRTNUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                        int startId,
                                        int len,
                                        const arctern::Result *p) {
  EXPECT_NE(p, nullptr);

  auto result = dynamic_cast<const arctern::FaceDetectRTNResult *>(p);
  std::shared_ptr<FaceDetectRTNData> data = std::dynamic_pointer_cast<FaceDetectRTNData>(gdata);

  EXPECT_EQ(result->detInfos_.size(), len);

  for (int i = 0; i < len; i++) {
    int rectNum = result->detInfos_[i].size();
    for (int j = 0; j < rectNum; j++) {
      EXPECT_NEAR(result->detInfos_[i][j].score, data->faceRects_[i + startId].confidences[j], 0.005);
      EXPECT_NEAR(result->detInfos_[i][j].rect.x, data->faceRects_[i + startId].rects[j].x, 2);
      EXPECT_NEAR(result->detInfos_[i][j].rect.y, data->faceRects_[i + startId].rects[j].y, 2);
      EXPECT_NEAR(result->detInfos_[i][j].rect.width, data->faceRects_[i + startId].rects[j].width, 2);
      EXPECT_NEAR(result->detInfos_[i][j].rect.height, data->faceRects_[i + startId].rects[j].height, 2);
    }
  }
}

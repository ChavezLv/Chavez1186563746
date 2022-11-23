/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.24
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "yaml-cpp/yaml.h"
#include "log/logger.h"
#include "yaml-cpp/node/parse.h"

void FaceExpressionData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
      auto type = node[1]["VALUE"].as<std::vector<int>>();
      onepicture.expression.type = arctern::ExpressionType(type[0]);

      auto score = node[2]["VALUE"].as<std::vector<float>>();
      onepicture.expression.score = score[0];

    expression_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void FaceExpressionData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();

    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        expression_[i].rect.x = rect[0];
        expression_[i].rect.y = rect[1];
        expression_[i].rect.width = rect[2];
        expression_[i].rect.height = rect[3];
      }
    }
  }
}

FaceExpressionUnittest::FaceExpressionUnittest(std::string
                                     ymlGroundTruth,
                                     std::string
                                     ymlInput,
                                     std::string
                                     imgDir,
                                     std::string
                                     modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {

}

FaceExpressionUnittest::~FaceExpressionUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> FaceExpressionUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceExpression> ptr(new arctern::FaceExpression);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> FaceExpressionUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                            int startId,
                                                                            int len) {
  std::shared_ptr<arctern::FaceExpressionRunParameter> ptr(new arctern::FaceExpressionRunParameter);
  std::shared_ptr<FaceExpressionData> data = std::dynamic_pointer_cast<FaceExpressionData>(gdata);

  for(int i = 0 ; i < len; i++) {
    ptr->rectsV_.push_back(data->expression_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FaceExpressionUnittest::generateGroundTruth() {
  std::shared_ptr<FaceExpressionData> ptr(new FaceExpressionData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void FaceExpressionUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                    int startId,
                                    int len,
                                    const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::FaceExpressionResult *>(p);
  std::shared_ptr<FaceExpressionData> data = std::dynamic_pointer_cast<FaceExpressionData>(gdata);

  EXPECT_EQ(result->faceExpressions_.size(), len);

  for (int i = 0; i < len; i++) {
    EXPECT_EQ(result->faceExpressions_[i].type, data->expression_[i + startId].expression.type);
    EXPECT_NEAR(result->faceExpressions_[i].score, data->expression_[i + startId].expression.score, 0.001);
  }
}

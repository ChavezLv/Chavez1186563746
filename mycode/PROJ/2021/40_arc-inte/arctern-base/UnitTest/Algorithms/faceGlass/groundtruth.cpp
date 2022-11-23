/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "yaml-cpp/yaml.h"
#include "yaml-cpp/node/parse.h"

void FaceGlassData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto type = node[1]["VALUE"].as<std::vector<int>>();
    onepicture.glass.glassesStatus = arctern::GlassesStatus(type[0]);

    auto score = node[2]["VALUE"].as<std::vector<float>>();
    onepicture.glass.statusScore = score[0];

    glass_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void FaceGlassData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();

#ifdef ATLAS_ALLOWED
    static int atlas_index = 0;
    glass_[atlas_index].rect.x = rect[0];
    glass_[atlas_index].rect.y = rect[1];
    glass_[atlas_index].rect.width = rect[2];
    glass_[atlas_index].rect.height = rect[3];
    atlas_index++;

#else
    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        glass_[i].rect.x = rect[0];
        glass_[i].rect.y = rect[1];
        glass_[i].rect.width = rect[2];
        glass_[i].rect.height = rect[3];
      }
    }
#endif
  }
}

FaceGlassUnittest::FaceGlassUnittest(std::string
                                     ymlGroundTruth,
                                     std::string
                                     ymlInput,
                                     std::string
                                     imgDir,
                                     std::string
                                     modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {

}

FaceGlassUnittest::~FaceGlassUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> FaceGlassUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceGlass> ptr(new arctern::FaceGlass);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> FaceGlassUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                            int startId,
                                                                            int len) {
  std::shared_ptr<arctern::FaceGlassRunParameter> ptr(new arctern::FaceGlassRunParameter);
  std::shared_ptr<FaceGlassData> data = std::dynamic_pointer_cast<FaceGlassData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->glass_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FaceGlassUnittest::generateGroundTruth() {
  std::shared_ptr<FaceGlassData> ptr(new FaceGlassData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void FaceGlassUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                    int startId,
                                    int len,
                                    const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::FaceGlassResult *>(p);
  std::shared_ptr<FaceGlassData> data = std::dynamic_pointer_cast<FaceGlassData>(gdata);

  EXPECT_EQ(result->FaceGlasss_.size(), len);

  for (int i = 0; i < len; i++) {
    EXPECT_EQ(result->FaceGlasss_[i].glassesStatus, data->glass_[i + startId].glass.glassesStatus);
    EXPECT_NEAR(result->FaceGlasss_[i].statusScore, data->glass_[i + startId].glass.statusScore, 0.01);
  }
}


/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.18
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "yaml-cpp/yaml.h"
#include "yaml-cpp/node/parse.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "log/logger.h"

void FaceOcclusionData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto types = node[1]["VALUE"][0].as<std::vector<int>>();
    auto scores = node[3]["VALUE"][0].as<std::vector<float>>();
    onepicture.faceOcclusion.leftEyeOpen.first = (types[0] == 1);
    onepicture.faceOcclusion.leftEyeOpen.second = scores[0];
    onepicture.faceOcclusion.rightEyeOpen.first = (types[1] == 1);
    onepicture.faceOcclusion.rightEyeOpen.second = scores[1];
    onepicture.faceOcclusion.leftEyeOcc.first = (types[2] == 1);
    onepicture.faceOcclusion.leftEyeOcc.second = scores[2];
    onepicture.faceOcclusion.rightEyeOcc.first = (types[3] == 1);
    onepicture.faceOcclusion.rightEyeOcc.second = scores[3];
    onepicture.faceOcclusion.leftCheekOcc.first = (types[4] == 1);
    onepicture.faceOcclusion.leftCheekOcc.second = scores[4];
    onepicture.faceOcclusion.rightCheekOcc.first = (types[5] == 1);
    onepicture.faceOcclusion.rightEyeOcc.second = scores[5];
    onepicture.faceOcclusion.mouthOcc.first = (types[6] == 1);
    onepicture.faceOcclusion.mouthOcc.second = scores[6];
    onepicture.faceOcclusion.noseOcc.first = (types[7] == 1);
    onepicture.faceOcclusion.noseOcc.second = scores[7];
    onepicture.faceOcclusion.chincontourOcc.first = (types[8] == 1);
    onepicture.faceOcclusion.chincontourOcc.second = scores[8];

    faceOcclusion_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void FaceOcclusionData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();

    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        faceOcclusion_[i].rect.x = rect[0];
        faceOcclusion_[i].rect.y = rect[1];
        faceOcclusion_[i].rect.width = rect[2];
        faceOcclusion_[i].rect.height = rect[3];
      }
    }
  }
}

FaceOcclusionUnittest::FaceOcclusionUnittest(std::string
                                             ymlGroundTruth,
                                             std::string
                                             ymlInput,
                                             std::string
                                             imgDir,
                                             std::string
                                             modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {

}

FaceOcclusionUnittest::~FaceOcclusionUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> FaceOcclusionUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FaceOcclusion> ptr(new arctern::FaceOcclusion);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> FaceOcclusionUnittest::buildRunParameter(std::shared_ptr<
    BaseGroundTruthData> gdata,
                                                                                int startId,
                                                                                int len) {
  std::shared_ptr<arctern::FaceOcclusionRunParameter> ptr(new arctern::FaceOcclusionRunParameter);
  std::shared_ptr<FaceOcclusionData> data = std::dynamic_pointer_cast<FaceOcclusionData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->faceOcclusion_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FaceOcclusionUnittest::generateGroundTruth() {
  std::shared_ptr<FaceOcclusionData> ptr(new FaceOcclusionData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void FaceOcclusionUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                        int startId,
                                        int len,
                                        const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::FaceOcclusionResult *>(p);
  std::shared_ptr<FaceOcclusionData> data = std::dynamic_pointer_cast<FaceOcclusionData>(gdata);

  EXPECT_EQ(result->faceOcclusion_.size(), len);

  for (int i = 0; i < len; i++) {
        EXPECT_NEAR(result->faceOcclusion_[i].noseOcc.second,
                    data->faceOcclusion_[i + startId].faceOcclusion.noseOcc.second, 0.015);
    EXPECT_NEAR(result->faceOcclusion_[i].leftEyeOpen.second,
                data->faceOcclusion_[i + startId].faceOcclusion.leftEyeOpen.second, 0.015);
    EXPECT_NEAR(result->faceOcclusion_[i].leftEyeOcc.second,
                data->faceOcclusion_[i + startId].faceOcclusion.leftEyeOcc.second, 0.015);
    EXPECT_NEAR(result->faceOcclusion_[i].leftCheekOcc.second,
                data->faceOcclusion_[i + startId].faceOcclusion.leftCheekOcc.second, 0.015);
  }
}

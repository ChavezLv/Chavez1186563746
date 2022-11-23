/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.20
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "yaml-cpp/yaml.h"
#include "log/logger.h"
#include "yaml-cpp/node/parse.h"

void FacialLandmarkData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto points = node[1]["VALUE"][0].as<std::vector<float>>();
    int pointNum = points.size() / 2;

    for (int i = 0; i < pointNum; i++) {
      onepicture.points.push_back(arctern::ArcternPoint{points[2 * i], points[2 * i + 1]});
    }
    landmarks_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void FacialLandmarkData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();
#ifdef ATLAS_ALLOWED
    static int atlas_index = 0;
    landmarks_[atlas_index].rect.x = rect[0];
    landmarks_[atlas_index].rect.y = rect[1];
    landmarks_[atlas_index].rect.width = rect[2];
    landmarks_[atlas_index].rect.height = rect[3];
    atlas_index++;
#else
    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        landmarks_[i].rect.x = rect[0];
        landmarks_[i].rect.y = rect[1];
        landmarks_[i].rect.width = rect[2];
        landmarks_[i].rect.height = rect[3];
      }
    }
#endif
  }
}

FacialLandmarkUnittest::FacialLandmarkUnittest(std::string
                                               ymlGroundTruth,
                                               std::string
                                               ymlInput,
                                               std::string
                                               imgDir,
                                               std::string
                                               modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {
  fout_ << "FacialLandmark: imageDir imageName rect(x,y,width,height) landmark(point(x,y))" <<std::endl;
}

FacialLandmarkUnittest::~FacialLandmarkUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> FacialLandmarkUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FacialLandmark> ptr(new arctern::FacialLandmark);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> FacialLandmarkUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                                 int startId,
                                                                                 int len) {
  std::shared_ptr<arctern::FacialLandmarkRunParameter> ptr(new arctern::FacialLandmarkRunParameter);
  std::shared_ptr<FacialLandmarkData> data = std::dynamic_pointer_cast<FacialLandmarkData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->landmarks_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FacialLandmarkUnittest::generateGroundTruth() {
  std::shared_ptr<FacialLandmarkData> ptr(new FacialLandmarkData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void FacialLandmarkUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                         int startId,
                                         int len,
                                         const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::FacialLandmarkResult *>(p);
  std::shared_ptr<FacialLandmarkData> data = std::dynamic_pointer_cast<FacialLandmarkData>(gdata);

  EXPECT_EQ(static_cast<int>(result->landmarks_.size()), len);

  for (int i = 0; i < len; i++) {
    fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
    fout_ << data->landmarks_[i+startId].rect.x<<" "<<data->landmarks_[i+startId].rect.y << " "
          <<data->landmarks_[i+startId].rect.width << " " <<data->landmarks_[i+startId].rect.height<<" ";

    auto size = result->landmarks_[i].size();
    for (size_t j = 0; j < size; j++) {
      fout_ << result->landmarks_[i][j].x  << " " <<result->landmarks_[i][j].y << " ";
      EXPECT_NEAR(result->landmarks_[i][j].x, data->landmarks_[i + startId].points[j].x, 1);
      EXPECT_NEAR(result->landmarks_[i][j].y, data->landmarks_[i + startId].points[j].y, 1);
    }
    fout_ << std::endl;
  }
}


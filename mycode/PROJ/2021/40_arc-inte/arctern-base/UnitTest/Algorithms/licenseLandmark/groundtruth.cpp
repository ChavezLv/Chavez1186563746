
#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "yaml-cpp/yaml.h"
#include "log/logger.h"
#include "yaml-cpp/node/parse.h"
#include "src/common/json.hpp"
#include <fstream>
using arctern::Result;
using arctern::ErrorType;
using arctern::ArcternRect;
using json = nlohmann::json;

void LicenseLandmarkData::addGroundTruth(std::string filename) {
  json parser;
  std::ifstream fin(filename);
  cout << "__func__ " << __func__ << ": " << __LINE__ << ": " << filename << endl;
  fin >> parser;
  /* for(int i=0;i<parser.size();i++) { */
  for(int i=0;i<2;i++) {

    ONEPICTURE onepicture;

    auto points = parser[i]["landmark"].get<std::vector<int>>();
    int pointNum = points.size() / 2;

    for (int i = 0; i < pointNum; i++) {
      onepicture.points.push_back(arctern::ArcternPoint{(float )points[2 * i], (float )points[2 * i + 1]});
    }
    landmarks_.push_back(onepicture);

    files_.push_back(parser.at(i)["filepath"]);
  cout << "__func__ " << __func__ << ": " << __LINE__ << ": " << parser.at(i)["filepath"] << endl;
  }
  fin.close();
}

LicenseLandmarkUnittest::LicenseLandmarkUnittest(std::string
                                 ymlGroundTruth,
                                 std::string
                                 ymlInput,
                                 std::string
                                 imgDir,
                                 std::string
                                 modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {
  fout_ << "LicenseLandmark:imageDir imageName landmark"<<std::endl;
}

LicenseLandmarkUnittest::~LicenseLandmarkUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> LicenseLandmarkUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::LicenseLandmark> ptr(new arctern::LicenseLandmark);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> LicenseLandmarkUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                          int startId,
                                                                          int len) {
  std::shared_ptr<arctern::LicenseLandmarkRunParameter> ptr(new arctern::LicenseLandmarkRunParameter);
  std::shared_ptr<LicenseLandmarkData> data = std::dynamic_pointer_cast<LicenseLandmarkData>(gdata);
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> LicenseLandmarkUnittest::generateGroundTruth() {
  std::shared_ptr<LicenseLandmarkData> ptr(new LicenseLandmarkData);
  ptr->addGroundTruth(ymlGroundTruthPath_);
  return ptr;
}

void LicenseLandmarkUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                  int startId,
                                  int len,
                                  const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::LicenseLandmarkResult *>(p);
  std::shared_ptr<LicenseLandmarkData> data = std::dynamic_pointer_cast<LicenseLandmarkData>(gdata);

  EXPECT_EQ(result->LicenseLandmarks_.size(), len);

  for (int i = 0; i < len; i++) {
    fout_ << imgDir_ << " " << data->files_[i + startId] << " ";
    for (int j = 0; j < (int)result->LicenseLandmarks_[i].size(); j++) {
      fout_ << result->LicenseLandmarks_[i][j].x << " " << result->LicenseLandmarks_[i][j].y <<" ";
      cout << "func:" << __func__ << ": " << result->LicenseLandmarks_[i][j].x << " " << result->LicenseLandmarks_[i][j].y <<" ";
      /* EXPECT_NEAR(result->LicenseLandmarks_[i][j].x, data->landmarks_[i + startId].points[j].x,1); */
      /* EXPECT_NEAR(result->LicenseLandmarks_[i][j].y, data->landmarks_[i + startId].points[j].y,1); */
    }
    fout_ << std::endl;
  }
}

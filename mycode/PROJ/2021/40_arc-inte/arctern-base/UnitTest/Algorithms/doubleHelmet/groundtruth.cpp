
#include "groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "yaml-cpp/yaml.h"
#include "log/logger.h"
#include "yaml-cpp/node/parse.h"

using arctern::Result;
using arctern::ErrorType;
using arctern::ArcternRect;

void DoubleHelmetData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto type = node[1]["VALUE"][0].as<std::vector<int>>();
    onepicture.doubleHelmet.first = type.at(0);

    auto score = node[2]["VALUE"].as<std::vector<float>>();
    onepicture.doubleHelmet.second = score.at(0);

    doubleHelmets_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void DoubleHelmetData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();
#ifdef ATLAS_ALLOWED
    static int atlas_index = 0;
    doubleHelmets_[atlas_index].rect.x = rect[0];
    doubleHelmets_[atlas_index].rect.y = rect[1];
    doubleHelmets_[atlas_index].rect.width = rect[2];
    doubleHelmets_[atlas_index++].rect.height = rect[3];
#else
    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        doubleHelmets_[i].rect.x = rect[0];
        doubleHelmets_[i].rect.y = rect[1];
        doubleHelmets_[i].rect.width = rect[2];
        doubleHelmets_[i].rect.height = rect[3];
      }
    }
#endif
  }
}

DoubleHelmetUnittest::DoubleHelmetUnittest(std::string
                                 ymlGroundTruth,
                                 std::string
                                 ymlInput,
                                 std::string
                                 imgDir,
                                 std::string
                                 modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {
  fout_ << "DoubleHelmet: imageDir imageName headwear score"<<std::endl;
}

DoubleHelmetUnittest::~DoubleHelmetUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> DoubleHelmetUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::DoubleHelmet> ptr(new arctern::DoubleHelmet);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> DoubleHelmetUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                          int startId,
                                                                          int len) {
  std::shared_ptr<arctern::DoubleHelmetRunParameter> ptr(new arctern::DoubleHelmetRunParameter);
  std::shared_ptr<DoubleHelmetData> data = std::dynamic_pointer_cast<DoubleHelmetData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->doubleHelmets_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> DoubleHelmetUnittest::generateGroundTruth() {
  std::shared_ptr<DoubleHelmetData> ptr(new DoubleHelmetData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void DoubleHelmetUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                  int startId,
                                  int len,
                                  const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::DoubleHelmetResult *>(p);
  std::shared_ptr<DoubleHelmetData> data = std::dynamic_pointer_cast<DoubleHelmetData>(gdata);

  EXPECT_EQ(result->m_vClassifyResults.size(), len);

  for (int i = 0; i < len; i++) {
    fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
    fout_ << data->doubleHelmets_[i+startId].rect.x<<" "<<data->doubleHelmets_[i+startId].rect.y << " "
          << data->doubleHelmets_[i+startId].rect.width << " " <<data->doubleHelmets_[i+startId].rect.height<<" ";
    fout_ <<(int) result->m_vClassifyResults[i].first <<" " << result->m_vClassifyResults[i].second<<std::endl;

    EXPECT_EQ(result->m_vClassifyResults[i].first, data->doubleHelmets_[i + startId].doubleHelmet.first);
    EXPECT_NEAR(result->m_vClassifyResults[i].second, data->doubleHelmets_[i + startId].doubleHelmet.second, 0.02);
  }
}

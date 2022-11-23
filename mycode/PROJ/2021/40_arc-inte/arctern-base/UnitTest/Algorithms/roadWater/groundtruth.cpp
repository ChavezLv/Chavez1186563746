

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "yaml-cpp/yaml.h"
#include "log/logger.h"
#include "yaml-cpp/node/parse.h"

void RoadWaterData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
  
    auto confidence = node[1]["VALUE"].as<std::vector<float>>();
    auto type = node[2]["VALUE"].as<std::vector<int>>();
 
    onepicture.confidences = confidence[0];
    onepicture.type = type[0];
    
  }
  data_.push_back(onepicture);
  files_.push_back(filename);
}

RoadWaterUnittest::RoadWaterUnittest(std::string ymlPath, std::string imgDir, std::string modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlPath, "", imgDir, modelpath,p) {
}

RoadWaterUnittest::~RoadWaterUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> RoadWaterUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::RoadWater> ptr(new arctern::RoadWater);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);
  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> RoadWaterUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                                 int startId,
                                                                                 int len) {
  std::shared_ptr<arctern::RunParameter> ptr(new arctern::RoadWaterRunParameter);
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> RoadWaterUnittest::generateGroundTruth() {
  std::shared_ptr<RoadWaterData> ptr(new RoadWaterData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, "");
  groundTruth.getGroundTruth(ptr);
  return ptr;
}

void RoadWaterUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                         int startId,
                                         int len,
                                         const arctern::Result *p) {
  EXPECT_NE(p, nullptr);

  auto result = dynamic_cast<const arctern::RoadWaterResult *>(p);
  std::shared_ptr<RoadWaterData> data = std::dynamic_pointer_cast<RoadWaterData>(gdata);

  EXPECT_EQ(static_cast<int>(result->roadWaterInfos_.size()), len);

  for (int i = 0; i < len; i++) {
   
    EXPECT_NEAR(result->roadWaterInfos_[i].confidence, data->data_[i + startId].confidences,0.005);
    EXPECT_EQ(result->roadWaterInfos_[i].type,data->data_[i + startId].type);
    
  }
}

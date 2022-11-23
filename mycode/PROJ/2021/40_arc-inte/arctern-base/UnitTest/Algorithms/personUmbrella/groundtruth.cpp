
#include "groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "yaml-cpp/yaml.h"
#include "log/logger.h"
#include "yaml-cpp/node/parse.h"

using arctern::Result;
using arctern::ErrorType;
using arctern::ArcternRect;

void PersonUmbrellaData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
    auto type = node[1]["VALUE"][0].as<std::vector<int>>();
    onepicture.personUmbrella.first = type.at(0);

    auto score = node[2]["VALUE"].as<std::vector<float>>();
    onepicture.personUmbrella.second = score.at(0);

    personUmbrellas_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void PersonUmbrellaData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();
#ifdef ATLAS_ALLOWED
    static int atlas_index = 0;
    personUmbrellas_[atlas_index].rect.x = rect[0];
    personUmbrellas_[atlas_index].rect.y = rect[1];
    personUmbrellas_[atlas_index].rect.width = rect[2];
    personUmbrellas_[atlas_index++].rect.height = rect[3];
#else
    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        personUmbrellas_[i].rect.x = rect[0];
        personUmbrellas_[i].rect.y = rect[1];
        personUmbrellas_[i].rect.width = rect[2];
        personUmbrellas_[i].rect.height = rect[3];
      }
    }
#endif
  }
}

PersonUmbrellaUnittest::PersonUmbrellaUnittest(std::string
                                 ymlGroundTruth,
                                 std::string
                                 ymlInput,
                                 std::string
                                 imgDir,
                                 std::string
                                 modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {
  fout_ << "PersonUmbrella: imageDir imageName headwear score"<<std::endl;
}

PersonUmbrellaUnittest::~PersonUmbrellaUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> PersonUmbrellaUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::PersonUmbrella> ptr(new arctern::PersonUmbrella);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> PersonUmbrellaUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                          int startId,
                                                                          int len) {
  std::shared_ptr<arctern::PersonUmbrellaRunParameter> ptr(new arctern::PersonUmbrellaRunParameter);
  std::shared_ptr<PersonUmbrellaData> data = std::dynamic_pointer_cast<PersonUmbrellaData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->personUmbrellas_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> PersonUmbrellaUnittest::generateGroundTruth() {
  std::shared_ptr<PersonUmbrellaData> ptr(new PersonUmbrellaData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void PersonUmbrellaUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                  int startId,
                                  int len,
                                  const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::PersonUmbrellaResult *>(p);
  std::shared_ptr<PersonUmbrellaData> data = std::dynamic_pointer_cast<PersonUmbrellaData>(gdata);

  EXPECT_EQ(result->m_vClassifyResults.size(), len);

  for (int i = 0; i < len; i++) {
    fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
    fout_ << data->personUmbrellas_[i+startId].rect.x<<" "<<data->personUmbrellas_[i+startId].rect.y << " "
          << data->personUmbrellas_[i+startId].rect.width << " " <<data->personUmbrellas_[i+startId].rect.height<<" ";
    fout_ <<(int) result->m_vClassifyResults[i].first <<" " << result->m_vClassifyResults[i].second<<std::endl;

    EXPECT_EQ(result->m_vClassifyResults[i].first, data->personUmbrellas_[i + startId].personUmbrella.first);
    EXPECT_NEAR(result->m_vClassifyResults[i].second, data->personUmbrellas_[i + startId].personUmbrella.second, 0.02);
  }
}

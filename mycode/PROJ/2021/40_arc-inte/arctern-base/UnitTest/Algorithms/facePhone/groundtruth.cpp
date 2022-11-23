/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.28
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/


#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "yaml-cpp/yaml.h"
#include "log/logger.h"
#include "yaml-cpp/node/parse.h"

using arctern::Result;
using arctern::ErrorType;
using arctern::ArcternRect;

void FacePhoneData::addGroundTruth(YAML::Node &node, std::string filename) {
  ONEPICTURE onepicture;
  if (node.size() > 1) {
      auto type = node[1]["VALUE"].as<std::vector<int>>();
    onepicture.phone.phoneStatus = arctern::PhoneStatus(type[0]);

      auto score = node[2]["VALUE"].as<std::vector<float>>();
    onepicture.phone.statusScore = score[0];

    facePhone_.push_back(onepicture);
    files_.push_back(filename);
  }
}

void FacePhoneData::addInputParam(YAML::Node &node, std::string filename) {

  if (node.size() > 1) {
    auto rect = node[1]["VALUE"].as<std::vector<float>>();
#ifdef ATLAS_ALLOWED
    static int atlas_index = 0;
    facePhone_[atlas_index].rect.x = rect[0];
    facePhone_[atlas_index].rect.y = rect[1];
    facePhone_[atlas_index].rect.width = rect[2];
    facePhone_[atlas_index].rect.height = rect[3];
    atlas_index++;
#else
    int len = files_.size();
    for (int i = 0; i < len; i++) {
      if (files_[i] == filename) {
        facePhone_[i].rect.x = rect[0];
        facePhone_[i].rect.y = rect[1];
        facePhone_[i].rect.width = rect[2];
        facePhone_[i].rect.height = rect[3];
      }
    }
#endif
  }
}

FacePhoneUnittest::FacePhoneUnittest(std::string
                                     ymlGroundTruth,
                                     std::string
                                     ymlInput,
                                     std::string
                                     imgDir,
                                     std::string
                                     modelpath,arctern::Result *p)
    : BaseAlgrithmUnittest(ymlGroundTruth, ymlInput, imgDir, modelpath,p) {
  fout_ << "FacePhone:imageDir imagName rect status score"<<std::endl;
}

FacePhoneUnittest::~FacePhoneUnittest() {
}

std::shared_ptr<arctern::AlgorithmInterface> FacePhoneUnittest::createAlgHandle(int batch) {
  std::shared_ptr<arctern::FacePhone> ptr(new arctern::FacePhone);
  arctern::InitParameter initParam;
  initParam.batchNum_ = batch;
  initParam.model_path = modelPath_;

  auto ret = ptr->Init(&initParam);

  EXPECT_EQ(ret, arctern::ErrorType::ERR_SUCCESS);

  return ptr;
}

std::shared_ptr<arctern::RunParameter> FacePhoneUnittest::buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                                            int startId,
                                                                            int len) {
  std::shared_ptr<arctern::FacePhoneRunParameter> ptr(new arctern::FacePhoneRunParameter);
  std::shared_ptr<FacePhoneData> data = std::dynamic_pointer_cast<FacePhoneData>(gdata);

  for (int i = 0; i < len; i++) {
    ptr->rectsV_.push_back(data->facePhone_[i + startId].rect);
  }
  return ptr;
}

std::shared_ptr<BaseGroundTruthData> FacePhoneUnittest::generateGroundTruth() {
  std::shared_ptr<FacePhoneData> ptr(new FacePhoneData);
  BaseGroundTruth groundTruth(ymlGroundTruthPath_, ymlInputPath_);
  groundTruth.getGroundTruth(ptr);
  groundTruth.getInputParam(ptr);
  return ptr;
}

void FacePhoneUnittest::checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                                    int startId,
                                    int len,
                                    const arctern::Result *p) {
  auto result = dynamic_cast<const arctern::FacePhoneResult *>(p);
  std::shared_ptr<FacePhoneData> data = std::dynamic_pointer_cast<FacePhoneData>(gdata);

  EXPECT_EQ(result->facePhones_.size(), len);

  for (int i = 0; i < len; i++) {
    fout_ <<imgDir_<<" " <<data->files_[i+startId] <<" ";
    fout_ << data->facePhone_[i+startId].rect.x<<" "<<data->facePhone_[i+startId].rect.y << " "
          <<data->facePhone_[i+startId].rect.width << " " <<data->facePhone_[i+startId].rect.height<<" ";
    fout_ << result->facePhones_[i].phoneStatus << " " << result->facePhones_[i].statusScore <<std::endl;

    EXPECT_EQ(result->facePhones_[i].phoneStatus, data->facePhone_[i + startId].phone.phoneStatus);
    EXPECT_NEAR(result->facePhones_[i].statusScore, data->facePhone_[i + startId].phone.statusScore, 0.01);
  }
}



//FacePhoneGroundTruth::FacePhoneGroundTruth(std::string ymlGroundTruth,
//                                         std::string ymlInput,
//                                         std::string imgDir,
//                                         std::string modelpath)
//    : ymlGroundTruthPath_(ymlGroundTruth), ymlInputPath_(ymlInput), imgDir_(imgDir), modelPath_(modelpath) {
//}
//
//void FacePhoneGroundTruth::checkOnePicture(arctern::FacePhone &ag, ONEPICTURE picture) {
//  const std::string dir = imgDir_;
//  std::string fullPath = dir + picture.file;
//
//  cv::Mat image = cv::imread(fullPath);
//  EXPECT_FALSE(image.empty());
//
//  ::arctern::ArcternImage img;
//  img.width = image.cols;
//  img.height = image.rows;
//  img.imgdata.gdata = image.data;
//  img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;
//
//  arctern::FacePhoneRunParameter runParameter;
//  // runParameter.imageFormat_ = arctern::BGR;
//  runParameter.imageV_.push_back(img);
//  runParameter.rectsV_.push_back({picture.rect.x, picture.rect.y, picture.rect.width, picture.rect.height});
//
//  const Result *p;
//
//  {
//    AUTOTIME;
//    spdlog::info("\n {2} image size = [{0},{1}]", img.width, img.height, fullPath);
//    p = ag.Process(&runParameter);
//  }
//
//  EXPECT_NE(p, nullptr);
//
//  auto result = dynamic_cast<const arctern::FacePhoneResult *>(p);
//
//  EXPECT_EQ(p->errorType_, ErrorType::ERR_SUCCESS);
//
//  EXPECT_EQ(result->facePhones_[0].phoneStatus, picture.phone.phoneStatus);
//
//  EXPECT_NEAR(result->facePhones_[0].statusScore, picture.phone.statusScore, 0.01);
//}
//
//void FacePhoneGroundTruth::checkBatchPicture(arctern::FacePhone &lmk, std::vector<ONEPICTURE> pictures) {
//  const std::string dir = imgDir_;
//
//  arctern::FacePhoneRunParameter runParameter;
//
//  std::vector<cv::Mat> mats;
//  int len = pictures.size();
//  mats.resize(len);
//
//  for (int i = 0; i < len; i++) {
//    std::string fullPath = dir + pictures[i].file;
//
//    mats[i] = cv::imread(fullPath);
//    // mats.push_back(image);
//    EXPECT_FALSE(mats[i].empty());
//
//    ::arctern::ArcternImage img;
//    img.width = mats[i].cols;
//    img.height = mats[i].rows;
//    img.imgdata.gdata = mats[i].data;
//    img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;
//
//    runParameter.imageV_.push_back(img);
//
//    ArcternRect rect{pictures[i].rect.x, pictures[i].rect.y, pictures[i].rect.width, pictures[i].rect.height};
//    runParameter.rectsV_.push_back(rect);
//  }
//
//  const Result *p;
//
//  {
//    AUTOTIME;
//    p = lmk.Process(&runParameter);
//  }
//
//  EXPECT_NE(p, nullptr);
//  EXPECT_EQ(p->errorType_, ErrorType::ERR_SUCCESS);
//
//  auto result = dynamic_cast<const arctern::FacePhoneResult *>(p);
//
//  EXPECT_EQ(result->facePhones_.size(), pictures.size());
//
//  int batchlen = pictures.size();
//
//  for (int idx = 0; idx < batchlen; idx++) {
//    EXPECT_EQ(result->facePhones_[idx].phoneStatus, pictures[idx].phone.phoneStatus);
//    EXPECT_NEAR(result->facePhones_[idx].statusScore, pictures[idx].phone.statusScore, 0.01);
//  }
//}
//
//void FacePhoneGroundTruth::singleTest() {
//  arctern::FacePhone ag;
//  arctern::InitParameter initParam;
//  initParam.batchNum_ = 1;
//  initParam.model_path = modelPath_;
//  auto ret = ag.Init(&initParam);
//  assert(ret == ErrorType::ERR_SUCCESS);
//
//  auto groundtruths = getGroundTruth();
//  getInputParam(groundtruths);
//
//  spdlog::error("test pictures num = {}", groundtruths.size());
//
//  for (auto &one :  groundtruths) {
//    checkOnePicture(ag, one);
//  }
//}
//
//void FacePhoneGroundTruth::batchTest() {
//  unsigned int seed = time(nullptr);
//
//  arctern::FacePhone ag;
//  arctern::InitParameter initParam;
//  initParam.batchNum_ = rand_r(&seed) % 3 + 1;   /// 1 - 10
//  initParam.model_path = modelPath_;
//  auto ret = ag.Init(&initParam);
//
//  EXPECT_EQ(ret, ErrorType::ERR_SUCCESS);
//
//  auto groundtruths = getGroundTruth();
//  getInputParam(groundtruths);
//
//  int picturesnum = groundtruths.size();
//
//  for (int idx = 0; idx < picturesnum; idx++) {
//    int randnum = rand_r(&seed) % 9 + 1;   /// 1 - 10
//    int left = picturesnum - idx;
//    int minNum = std::min(randnum, left);
//
//    std::vector<ONEPICTURE> someGroundTruths;
//    for (int tmp = 0; tmp < minNum; tmp++) {
//      someGroundTruths.push_back(groundtruths[idx + tmp]);
//    }
//
//    idx += minNum;
//    checkBatchPicture(ag, someGroundTruths);
//  }
//}
//
//std::vector<ONEPICTURE> FacePhoneGroundTruth::getGroundTruth() {
//  std::cout << " get ground truth start " << ymlGroundTruthPath_ << std::endl;
//  std::vector<ONEPICTURE> result;
//  YAML::Node node = YAML::LoadFile(ymlGroundTruthPath_);
//
//  if (node.IsNull()) {
//    std::cout << "node is null" << std::endl;
//  }
//
//  YAML::Node root_node = node["ROOT"];
//  for (auto it = root_node.begin(); it != root_node.end(); it++) {
//    YAML::Node element_node = (*it)["ELEMENT"];
//
//    std::string file = element_node[0]["VALUE"].as<std::string>();
//
//    ONEPICTURE one;
//    one.file = file;
//
//    if (element_node.size() > 1) {
//      auto type = element_node[1]["VALUE"].as<std::vector<int>>();
//      one.phone.phoneStatus = arctern::PhoneStatus(type[0]);
//
//      auto score = element_node[2]["VALUE"].as<std::vector<float>>();
//      one.phone.statusScore = score[0];
//
//      result.emplace_back(one);
//    }
//  }
//
//  std::cout << " get ground truth end" << std::endl;
//  return result;
//}
//
//void FacePhoneGroundTruth::getInputParam(std::vector<ONEPICTURE> &groundTruth) {
//  YAML::Node node = YAML::LoadFile(ymlInputPath_);
//
//  YAML::Node root_node = node["ROOT"];
//  for (auto it = root_node.begin(); it != root_node.end(); it++) {
//    YAML::Node element_node = (*it)["ELEMENT"];
//
//    std::string file = element_node[0]["VALUE"].as<std::string>();
//
//    if (element_node.size() > 1) {
//      auto rect = element_node[1]["VALUE"].as<std::vector<float>>();
//
//      for (auto &one : groundTruth) {
//        if (one.file == file) {
//          one.rect.x = rect[0];
//          one.rect.y = rect[1];
//          one.rect.width = rect[2];
//          one.rect.height = rect[3];
//        }
//      }
//    }
//  }
//}

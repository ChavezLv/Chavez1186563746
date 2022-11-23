/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.08
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#ifndef UNITTEST_ALGORITHMS_PERSONATTR_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_PERSONATTR_GROUNDTRUTH_H_

#include "src/Algorithms/personAttr/personAttr.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"
using namespace arctern;

typedef struct {
  arctern::ArcternRect rect;
  std::vector<float> truncation;
  arctern::SinglePersonAttr attrs;
} ONEPICTURE;

class PersonAttrData : public BaseGroundTruthData {
 public:
  PersonAttrData() = default;
  ~PersonAttrData() override = default;

  void addGroundTruth(YAML::Node &, std::string) override;

  void addInputParam(YAML::Node &, std::string) override;

 public:
  std::vector<ONEPICTURE> personAttrs_;
  int attrString2int(const std::string &attrname, const std::string &attrtype);
  const std::vector<std::pair<std::string, std::vector<std::string>>> attrs_map = {
    {"Age",           {"adult",  "child", "old"}},
    {"Backpack",      {"no",     "yes"}},
    {"Cart",          {"no",     "yes"}},
    {"FrontHold",     {"no",     "yes"}},
    {"Gender",        {"female", "male"}},
    {"Glasses",       {"no",     "glasses", "sunglasses", "unknown"}},
    {"HairColor",     {"black",  "brown", "gray",   "white", "yellow", "others", "unknown"}},
    {"HairLength",    {"bald",   "long",  "short", "unknown"}},
    {"Handbag",       {"no",     "yes"}},
    {"HeadWear",      {"no",     "hat",    "helmet", "others"}},
    {"LowerColor",    {"black",  "blue",   "gray",    "green", "orange", "others", "purple", "red", "white", "yellow", "pink", "brown", "unknown"}},
    {"LowerTexture",  {"grid",   "joint",  "others",  "pattern",  "pure",    "stripe",    "uniform", "unknown"}},
    {"LowerType",     {"dress",   "pants", "shorts", "unknown"}},
    {"Mask",          {"no",     "yes", "unknown"}},
    {"Orientation",   {"back",   "front", "side"}},
    {"Overcoat",      {"no",     "yes", "unknown"}},
    {"Person",        {"others", "pedestrian", "rider"}},
    {"SingleBag",     {"no",     "yes", "unknown"}},
    {"TrolleyCase",   {"no",     "yes"}},
    {"UpperColor",    {"black",  "blue",  "gray", "green", "orange", "others", "purple",  "red", "white", "yellow", "pink", "brown"}},
    {"UpperTexture",  {"grid",   "joint", "others",  "pattern",  "pure",    "stripe",    "uniform"}},
    {"UpperType",     {"longsleeve", "shortsleeve"}},
    {"UsePhone",      {"no",     "yes"}},
    {"Quality",       {"bad",    "good"}}
  };

};

class PersonAttrUnittest : public BaseAlgrithmUnittest {
 public:
  PersonAttrUnittest(std::string ymlGroundTruth, std::string ymlInput, std::string imgDir, std::string modelpath, arctern::Result *p);

  ~PersonAttrUnittest() override ;

  std::shared_ptr<arctern::AlgorithmInterface> createAlgHandle(int batch) override;

  std::shared_ptr<arctern::RunParameter> buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                           int startId,
                                                           int len) override;

  std::shared_ptr<BaseGroundTruthData> generateGroundTruth() override;

  void checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                   int startId,
                   int len,
                   const arctern::Result *p) override;
};

#endif  // UNITTEST_ALGORITHMS_PERSONATTR_GROUNDTRUTH_H_


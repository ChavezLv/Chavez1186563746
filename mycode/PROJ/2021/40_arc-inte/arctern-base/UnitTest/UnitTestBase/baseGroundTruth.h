/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.13
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#pragma once
#include <string>
#include <functional>
#include "yaml-cpp/yaml.h"
#include "yaml-cpp/node/parse.h"

class BaseGroundTruthData {
 public:
  BaseGroundTruthData() ;
  virtual ~BaseGroundTruthData() ;

  virtual void addGroundTruth(YAML::Node &, std::string) {

  };

  virtual void addInputParam(YAML::Node &, std::string) {}

  virtual int size() {
    return files_.size();
  }
 public:
  std::vector<std::string> files_;
};

class BaseGroundTruth {
 public:
  explicit BaseGroundTruth(std::string groundtruthPath, std::string inputParamPath );
  void getGroundTruth(std::shared_ptr<BaseGroundTruthData> callback);
  void getInputParam(std::shared_ptr<BaseGroundTruthData> callback);
 private:
  std::string groundtruthPath_;
  std::string inputParamPath_;
};



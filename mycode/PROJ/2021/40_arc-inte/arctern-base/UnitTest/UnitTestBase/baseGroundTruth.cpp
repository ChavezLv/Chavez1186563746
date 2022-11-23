/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.13
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "baseGroundTruth.h"
#include <iostream>

BaseGroundTruthData::BaseGroundTruthData() {

}

BaseGroundTruthData::~BaseGroundTruthData() {

}

BaseGroundTruth::BaseGroundTruth(std::string groundtruthPath, std::string inputParamPath) : groundtruthPath_(
    groundtruthPath), inputParamPath_(inputParamPath) {

}

void BaseGroundTruth::getGroundTruth(std::shared_ptr<BaseGroundTruthData> callback) {
  std::cout << " get ground truth start " << groundtruthPath_ << std::endl;

  YAML::Node node = YAML::LoadFile(groundtruthPath_);
  if (node.IsNull()) {
    std::cout << "node is null" << std::endl;
  }
  YAML::Node root_node = node["ROOT"];
  for (auto it = root_node.begin(); it != root_node.end(); it++) {
    YAML::Node element_node = (*it)["ELEMENT"];
    std::string file = element_node[0]["VALUE"].as<std::string>();
    callback->addGroundTruth(element_node, file);
  }
  std::cout << " get ground truth end" << std::endl;
}

void BaseGroundTruth::getInputParam(std::shared_ptr<BaseGroundTruthData> callback) {

  std::cout << "getInputParam start = " << inputParamPath_ << std::endl;
  YAML::Node node = YAML::LoadFile(inputParamPath_);
  YAML::Node root_node = node["ROOT"];
  for (auto it = root_node.begin(); it != root_node.end(); it++) {
    YAML::Node element_node = (*it)["ELEMENT"];
    std::string file = element_node[0]["VALUE"].as<std::string>();
    callback->addInputParam(element_node, file);
  }
  std::cout << "getInputParam end" << std::endl;
}


//
// Created by Admin on 2020/6/9.
//

#include "common.h"

std::string int2string(int num){
  std::stringstream sst;
  sst << num;
  return sst.str();
}

int string2int(std::string str){
  int ret = 0;
  std::stringstream stm(str);
  stm  >> ret;
  return ret;
}



bool isExist(const std::string& name) {
  //todo
  if(name.empty()) return false;
  return (access(name.c_str(),F_OK) != -1);
}

std::vector<std::string> split_string(const std::string &str, char delimiter) {
  std::vector<std::string> result;
  if (str.size() > 0) {
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
      result.push_back(item);
    }
  }
  return result;
}
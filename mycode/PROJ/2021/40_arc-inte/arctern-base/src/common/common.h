//
// Created by Admin on 2020/6/9.
//

#ifndef ARCTERN_BASE_SRC_COMMON_COMMON_H_
#define ARCTERN_BASE_SRC_COMMON_COMMON_H_
#include <string>
#include <sstream>
#include <unistd.h>
#include <vector>
//using namespace std;
template<typename T>
T string2T(std::string str){
  T ret = 0;
  std::stringstream stm(str);
  stm  >> ret;
  return ret;
}
extern std::string int2string(int num);
extern int string2int(std::string str);
extern bool isExist(const std::string& name);
extern std::vector<std::string> split_string(const std::string &str, char delimiter); 
#endif //ARCTERN_BASE_SRC_COMMON_COMMON_H_

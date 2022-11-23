/****************************************************************************
 *  Filename:       unittestalgrithmbase.h
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.02.21
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/
#ifndef UNITTESTALGRITHMBASE_H
#define UNITTESTALGRITHMBASE_H
#include <vector>
#include <string>
#include <map>
#include "src/Base/arcternalgorithm.h"
#include "./src/Processors/Algorithms/preprocesser.h"
#include "./include/configparameter.h"
namespace arctern {
class UnitTestBaseRunParameter
    : public RunParameter {
 public:
  std::string modelName_;
  std::vector<std::string> inputDataNames_;
};

class UnitTestAlgrithmBase {
 public:
  UnitTestAlgrithmBase();
  virtual ~UnitTestAlgrithmBase();
  virtual ErrorType Init(InitParameter *p);
  virtual void Release();
  virtual const Result *Process(RunParameter *p);
  virtual void Test(RunParameter *p) {}
  virtual ErrorType SetParameter(ConfigParameter *p);
  virtual const StatusParameter *GetParameter(StatusParameter *p);

 public:
  std::vector<std::string> modelNames_;  // 入参出参相同的一类模型名字
  std::string modelPath_ = "";  // 模型路径
  // groundtruth 文件名字，可选
  std::vector<std::string> gtNames_;
  std::string gtPath_ = "";
  std::string dataPath_ = "";
  // 每个模型对应需要验证的数据
  std::map<std::string, std::vector<std::string>> dataNames_;
  // 使用该类的单元测试唯一标识，比如cudaTest123
  // 类在使用该测试类则testName_="比如cudaTest123";
  std::string testName_ = "";
};
}  // namespace arctern
#endif  // UNITTESTALGRITHMBASE_H

/****************************************************************************
 *  Filename:       enginmanager.h
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.02.21
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/
#ifndef SRC_ENGIN_SRC_ENGINMANAGER_H_
#define SRC_ENGIN_SRC_ENGINMANAGER_H_


#include "src/Base/algorithmbase.h"
#include "src/Engin/src/enginBase.h"
#include "src/common/arcterndefs_generated.h"
#include "src/common/file_buffer.h"

namespace arctern {
class EngineManager {
 public:
  EngineManager();
  virtual ~EngineManager();

  virtual ErrorType CreateEngine(InitParameter *p, std::vector<std::shared_ptr<EnginBase>> &engines);

  virtual void Release();

 public:
  int parseMxnetModels(std::string modelPath);

 public:
  ///> note MxnetModels is used in mxnet, tensorflow, tvm, trt, cuda etc;
  const arctern::MxnetModels *mxnet_models_ = nullptr;
  FileBuffer *fileBuffer_ = nullptr;
};
}  // namespace arctern
#endif  // SRC_ENGIN_SRC_ENGINMANAGER_H_

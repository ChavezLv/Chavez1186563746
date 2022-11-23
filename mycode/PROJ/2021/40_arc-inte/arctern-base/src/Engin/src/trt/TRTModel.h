//
// Created by chengaohua on 2021/6/1.
//

#ifndef ARCTERN_BASE_SRC_ENGIN_SRC_TRT_TRTMODEL_H_
#define ARCTERN_BASE_SRC_ENGIN_SRC_TRT_TRTMODEL_H_

#include "src/common/file_buffer.h"
#include <memory>
#include <map>
#include "cnnModelTrtContext.h"
#include <src/common/ThreadSafeMap.h>
#include "src/common/ReadWriteLock.h"
namespace arctern {

typedef struct {
  int gpuId;
  int batch;
  FileBuffer * filebuffer;
  ArcternTypeBits dtype;
  std::string key;
  std::string modelsPath;
  std::string trtModelsPath;
}TrtModelInfo;

class TRTModelResource {
 private:
  static TRTModelResource * instance_;
  ReadWriteLock lock_;
  TRTModelResource(){};

 public:
  static TRTModelResource * getInstance(){
    return instance_;
  }

  int registerMxnetModel(TrtModelInfo & info) {
    WriteLock_Guard lock_guard(&lock_);
    if(engineMap_.find(info.key)) return 0;
    shared_ptr<CnnModelTRTEngine> engine(new CnnModelTRTEngine(info.gpuId,info.dtype,info.batch),
                                         [](CnnModelTRTEngine *p){p->destory();});
    engine->init(*info.filebuffer, info.trtModelsPath);

    engineMap_[info.key] = engine;
    return 0;
  }

  int registerTrtModel(TrtModelInfo & info){
    WriteLock_Guard lock_guard(&lock_);
    if(engineMap_.find(info.key)) return 0;
    shared_ptr<CnnModelTRTEngine> engine(new CnnModelTRTEngine(info.gpuId,info.dtype,info.batch),
                                         [](CnnModelTRTEngine *p){p->destory();});
    engine->init(*info.filebuffer);

    engineMap_[info.key] = engine;
    return 0;
  }

  int registerTrtOnnxModel(TrtModelInfo & info){
    WriteLock_Guard lock_guard(&lock_);
    if(engineMap_.find(info.key)) return 0;
    shared_ptr<CnnModelTRTEngine> engine(new CnnModelTRTEngine(info.gpuId,info.dtype,info.batch),
                                         [](CnnModelTRTEngine *p){p->destory();});
    engine->initOnnx(info.modelsPath,info.trtModelsPath,*info.filebuffer);

    engineMap_[info.key] = engine;
    return 0;
  }

  std::shared_ptr<CnnModelTRTEngine> getEngine(std::string name) {
    WriteLock_Guard lock_guard(&lock_);
    return engineMap_[name];
  }

  ThreadSafeMap<std::string , std::shared_ptr<CnnModelTRTEngine>> engineMap_;
};
}

#endif //ARCTERN_BASE_SRC_ENGIN_SRC_TRT_TRTMODEL_H_

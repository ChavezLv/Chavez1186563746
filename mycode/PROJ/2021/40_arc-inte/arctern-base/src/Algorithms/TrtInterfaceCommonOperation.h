//
// Created by chengaohua on 2021/5/22.
//

#ifndef ARCTERN_BASE_TRTINTERFACECOMMONOPERATION_H
#define ARCTERN_BASE_TRTINTERFACECOMMONOPERATION_H

#include "include/arctern.h"
#include "include/initparameter.h"
namespace arctern {
class ArcternAlgorithmEx;
class TrtInterfaceCommonOperation {
 public:
  TrtInterfaceCommonOperation() = default;
  static ErrorType Init(ArcternAlgorithmEx* impl,const InitParameter *param);
 protected:
  static bool isOnnxModel(const char * const buffer);
};
}

#endif //ARCTERN_BASE_TRTINTERFACECOMMONOPERATION_H

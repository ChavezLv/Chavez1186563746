//
// Created by gh on 2020/12/8.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_ATLASINTERFACECOMMONOPERATION_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_ATLASINTERFACECOMMONOPERATION_H_

#include "include/arctern.h"
#include "src/Base/atlas/ArcternAlgorithmEx.h"

namespace arctern {
class ArcternAlgorithm;
class AtlasInterfaceCommonOperation {
 public:
  AtlasInterfaceCommonOperation() = delete;
  ~AtlasInterfaceCommonOperation();
  static ErrorType Init(ArcternAlgorithmEx* impl,const InitParameter *param);
};
}  // namespace arctern

#endif //ARCTERN_BASE_SRC_ALGORITHMS_ATLASINTERFACECOMMONOPERATION_H_

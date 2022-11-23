//
// Created by gh on 2021/8/25.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_MNNINTERFACECOMMONOPERATION_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_MNNINTERFACECOMMONOPERATION_H_

namespace arctern {
class ArcternAlgorithm;
class MnnInterfaceCommonOperation {
 public:
  MnnInterfaceCommonOperation() = delete;
  ~MnnInterfaceCommonOperation() = default;
  static ErrorType Init(ArcternAlgorithm* impl,const InitParameter *param);
};
}


#endif //ARCTERN_BASE_SRC_ALGORITHMS_MNNINTERFACECOMMONOPERATION_H_

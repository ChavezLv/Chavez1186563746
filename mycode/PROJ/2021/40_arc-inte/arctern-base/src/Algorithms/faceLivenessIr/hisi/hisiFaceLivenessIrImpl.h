/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.27
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_SRC_ALGORITHMS_FACELIVENESSIR_HISI_HISIFACELIVENESSIR_H_
#define ARCTERN_SRC_ALGORITHMS_FACELIVENESSIR_HISI_HISIFACELIVENESSIR_H_

#include "../faceLivenessIrImpl.h"

namespace arctern {
class HisiFaceLivenessIrImpl: public FaceLivenessIrImpl {
 public:
  HisiFaceLivenessIrImpl();
  ~HisiFaceLivenessIrImpl() override;
  ErrorType Init(const InitParameter *p) override;
};
}


#endif //ARCTERN_SRC_ALGORITHMS_FACELIVENESSIR_HISI_HISIFACELIVENESSIR_H_

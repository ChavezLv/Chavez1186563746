/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.28
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_HISIFACEMASKIMPL_H
#define ARCTERN_HISIFACEMASKIMPL_H

#include "../faceMaskImpl.h"

namespace arctern {
class HisiFaceMaskImpl : public FaceMaskImpl {
 public:
  HisiFaceMaskImpl();
  ~HisiFaceMaskImpl() override;
  ErrorType Init(const InitParameter *p) override;
};
}

#endif //ARCTERN_HISIFACEMASKIMPL_H

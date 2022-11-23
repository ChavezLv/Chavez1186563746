/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.11.25
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_BASE_TVMFACEMASKIMPL_H
#define ARCTERN_BASE_TVMFACEMASKIMPL_H

#include "../faceMaskImpl.h"

namespace arctern {
class TvmFaceMaskImpl : public FaceMaskImpl {
 public:
  TvmFaceMaskImpl() = default;
  ~TvmFaceMaskImpl() override = default;
  ErrorType Init(const InitParameter *p) override;
};
}

#endif //ARCTERN_BASE_TVMFACEMASKIMPL_H

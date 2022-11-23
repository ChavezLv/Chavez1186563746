/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.11.26
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_BASE_TVMFACIALLANDMARKIMPL_H
#define ARCTERN_BASE_TVMFACIALLANDMARKIMPL_H

#include "../facialLandmarkImpl.h"

namespace arctern {
class TvmFacialLandmarkImpl : public FacialLandmarkImpl {
 public:
  TvmFacialLandmarkImpl() = default;
  ~TvmFacialLandmarkImpl() = default;
  ErrorType Init(const InitParameter *p) override;
};
}


#endif //ARCTERN_BASE_TVMFACIALLANDMARKIMPL_H

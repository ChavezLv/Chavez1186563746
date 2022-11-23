/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.12.02
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_RKNNFACEQUALITYIMPL_H
#define ARCTERN_RKNNFACEQUALITYIMPL_H

#include "src/Algorithms/faceQuality/faceQualityImpl.h"

namespace arctern {
class RknnFaceQualityImpl : public FaceQualityImpl {
 public:
    RknnFaceQualityImpl();
    ~RknnFaceQualityImpl();

    ErrorType Init(const InitParameter *p) override;
    int GetConfigInfo(arctern::MxnetModel *model) override;
};
} // namespace

#endif //ARCTERN_RKNNFACEQUALITYIMPL_H

//
// Created by Admin on 2021/11/1.
//

#ifndef ARCTERN_BASE_SRC_IMGPREPROCESS_AFFINE_H_
#define ARCTERN_BASE_SRC_IMGPREPROCESS_AFFINE_H_
#include "buffers.h"
using namespace ImgPreprocessEx;
namespace arctern {namespace cuda {
extern void Affine(const DeviceBuffer &_src, DeviceBuffer &_dst,  double transform[2][3],cudaStream_t *m_pStream);
}}
#endif //ARCTERN_BASE_SRC_IMGPREPROCESS_AFFINE_H_

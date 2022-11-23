//
// Created by Admin on 2021/11/1.
//

#ifndef ARCTERN_BASE_SRC_IMGPREPROCESS_CUDA_CROP_CUH_
#define ARCTERN_BASE_SRC_IMGPREPROCESS_CUDA_CROP_CUH_
#include "common.hpp"
#include "cuda_types.hpp"
#include "util.hpp"
#include "include/arctern.h"
namespace arctern{ namespace cuda{ namespace device {
template <typename T>
void crop(const PtrStepSzb& src, PtrStepSzb& dst,  ArcternRect rROI,cudaStream_t *m_pStream);
}}}
#endif //ARCTERN_BASE_SRC_IMGPREPROCESS_CUDA_CROP_CUH_

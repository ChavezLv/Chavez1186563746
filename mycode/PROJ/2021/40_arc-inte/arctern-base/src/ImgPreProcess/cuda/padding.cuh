//
// Created by Admin on 2021/7/9.
//

#ifndef CUDA_RESIZE_CUDA_PADDING_CUH_
#define CUDA_RESIZE_CUDA_PADDING_CUH_
#include "common.hpp"
#include "cuda_types.hpp"
#include "util.hpp"
namespace arctern{ namespace cuda{ namespace device {
template <typename T>
void padding(const PtrStepSzb& src, PtrStepSzb& dst, int top,int left,T borderValue,cudaStream_t *m_pStream);
}}}
#endif //CUDA_RESIZE_CUDA_PADDING_CUH_

//
// Created by Admin on 2021/7/5.
//

#ifndef CUDA_RESIZE_CUDA_NORMALIZED_CUH_
#define CUDA_RESIZE_CUDA_NORMALIZED_CUH_
#include "common.hpp"
#include "cuda_types.hpp"
#include "util.hpp"
namespace arctern{ namespace cuda{ namespace device {
template <typename T,typename U>
void normalized(const PtrStepSzb& src, PtrStepSzb& dst, const float4 alpha, const float4 beta, cudaStream_t *m_pStream);
}}}
#endif //CUDA_RESIZE_CUDA_NORMALIZED_CUH_

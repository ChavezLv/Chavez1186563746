//
// Created by Admin on 2021/11/1.
//

#ifndef ARCTERN_BASE_SRC_IMGPREPROCESS_CUDA_AFFINE_CUH_
#define ARCTERN_BASE_SRC_IMGPREPROCESS_CUDA_AFFINE_CUH_
#include "cuda_types.hpp"
namespace arctern {
namespace cuda{
namespace device {
template<typename ST, typename WT>
void affine(const PtrStepSzb &src, PtrStepSzb &dst,  double transform[2][3],cudaStream_t *m_pStream);
}}}
#endif //ARCTERN_BASE_SRC_IMGPREPROCESS_CUDA_AFFINE_CUH_

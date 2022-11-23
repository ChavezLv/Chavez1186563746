//
// Created by Admin on 2021/7/7.
//

#ifndef CUDA_RESIZE_CUDA_CVTCOLOR_CUH_
#define CUDA_RESIZE_CUDA_CVTCOLOR_CUH_
#include "cuda_types.hpp"
namespace arctern {
namespace cuda{
namespace device {

template<typename T, typename U>
void cvt4CTo3C(const PtrStepSzb &src, PtrStepSzb &dst);

template<typename T, typename U>
void cvt3CTo4C(const PtrStepSzb &src, PtrStepSzb &dst);

template<typename T, typename U>
void ExtractChannel(const PtrStepSzb &src, PtrStepSzb &dst,cudaStream_t *m_pStream);

template<typename T>
void cvtBGR2RGB(const PtrStepSzb &src, PtrStepSzb &dst,cudaStream_t *m_pStream);

template<typename T, typename U>
void cvtBGR2GRAY(const PtrStepSzb &src, PtrStepSzb &dst,cudaStream_t *m_pStream);

template<typename T, typename U>
void cvtRGB2GRAY(const PtrStepSzb &src, PtrStepSzb &dst,cudaStream_t *m_pStream);

}
}
}
#endif //CUDA_RESIZE_CUDA_CVTCOLOR_CUH_

//
// Created by Admin on 2021/7/5.
//

#ifndef CUDA_NORMALIZED_H_
#define CUDA_NORMALIZED_H_
#include "buffers.h"
#include "cuda.h"
using namespace ImgPreprocessEx;
namespace arctern{
namespace cuda{
extern void normalized(const DeviceBuffer& _src,
                      DeviceBuffer& _dst,
                      const float4 alpha,
                      const float4 beta,
                      cudaStream_t *m_pStream);
}
}
#endif //CUDA_NORMALIZED_H_

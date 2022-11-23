//
// Created by Admin on 2021/7/9.
//

#ifndef CUDA_PADDING_H_
#define CUDA_PADDING_H_
#include "buffers.h"
#include "cuda.h"
using namespace ImgPreprocessEx;
namespace arctern{
namespace cuda{
extern void padding(const DeviceBuffer& _src,DeviceBuffer& _dst,int top,int left, float3 borderValue,cudaStream_t *m_pStream);
}
}
#endif //CUDA_PADDING_H_

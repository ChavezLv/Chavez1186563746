//
// Created by Admin on 2021/6/29.
//

#ifndef CUDA_RESIZE_CUDA_RESIZE_H_
#define CUDA_RESIZE_CUDA_RESIZE_H_
#include "buffers.h"
#include "cuda.h"
#include "include/arctern.h"
using namespace ImgPreprocessEx;
namespace arctern{
namespace cuda{

extern void resize(const DeviceBuffer& _src,
                   DeviceBuffer& _dst,
                   double fy = 0,
                   double fx = 0,
                   int interpolation = 1,
                   cudaStream_t *pStream = nullptr);
}
}

#endif //CUDA_RESIZE_CUDA_RESIZE_H_

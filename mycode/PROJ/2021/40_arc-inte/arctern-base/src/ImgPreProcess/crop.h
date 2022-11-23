//
// Created by Admin on 2021/11/1.
//

#ifndef CUDA_CROP_H_
#define CUDA_CROP_H_
#include "buffers.h"
#include "cuda.h"
#include "include/arctern.h"
using namespace ImgPreprocessEx;
namespace arctern {
namespace cuda {

extern void crop(const DeviceBuffer &_src,
                 DeviceBuffer &_dst,
                  ArcternRect rROI,cudaStream_t *m_pStream);
}
}

#endif //CUDA_CROP_H_

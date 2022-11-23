//
// Created by Admin on 2021/7/7.
//

#ifndef CUDA_CVTCOLOR_H_
#define CUDA_CVTCOLOR_H_
#include "buffers.h"
using namespace ImgPreprocessEx;
namespace arctern {namespace cuda{
extern void cvt3CTo4C(const DeviceBuffer& _src,
                            DeviceBuffer& _dst);

extern void cvt4CTo3C(const DeviceBuffer& _src,
                            DeviceBuffer& _dst);

extern void ExtractChannel(const DeviceBuffer& _src,
                          DeviceBuffer& _dst,cudaStream_t *m_pStream);

extern void cvtRGB2BGR(const DeviceBuffer& _src,
                       DeviceBuffer& _dst,cudaStream_t *m_pStream);

extern void cvtBGR2RGB(const DeviceBuffer& _src,
                       DeviceBuffer& _dst,cudaStream_t *m_pStream);

extern void cvtBGR2GRAY(const DeviceBuffer& _src,
                        DeviceBuffer& _dst,cudaStream_t *m_pStream);

extern void cvtRGB2GRAY(const DeviceBuffer& _src,
                        DeviceBuffer& _dst,cudaStream_t *m_pStream);
}}
#endif //CUDA_CVTCOLOR_H_

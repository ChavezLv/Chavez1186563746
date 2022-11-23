//
// Created by Admin on 2021/11/1.
//

#include "affine.h"
#include "cuda/affine.cuh"
void arctern::cuda::Affine(const DeviceBuffer &_src, DeviceBuffer &_dst,
                             double transform[2][3],cudaStream_t *m_pStream){
  assert(_src.Type() == nvinfer1::DataType::kINT8);
  assert(_dst.Type() == nvinfer1::DataType::kFLOAT);

  int srcStep = _src.Step();
  PtrStepSzb src(static_cast<int>(_src.Height()), static_cast<int>(_src.Width()),
                 static_cast<unsigned char*>(const_cast<void*>(_src.data())),
                 srcStep);

  int dstStep = _dst.Step();
  PtrStepSzb dst(_dst.Height(),_dst.Width(),static_cast<unsigned char*>(_dst.data()),dstStep);

  arctern::cuda::device::affine<uchar3,float3>(src, dst, transform,m_pStream);
}
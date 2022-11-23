//
// Created by Admin on 2021/7/5.
//

#include "normalized.h"
#include "cuda/normalized.cuh"
namespace arctern{
namespace cuda{
void normalized(const DeviceBuffer& _src,
               DeviceBuffer& _dst,
               const float4 alpha,
               const float4 beta,cudaStream_t *m_pStream){
  //typedef void (*func_t)();
  using func_t = void (*)(const PtrStepSzb &src, PtrStepSzb &dst, const float4 alpha, const float4 beta,cudaStream_t *m_pStream);
  static const func_t funcs[4][3]={{device::normalized<uchar,float>,0,device::normalized<uchar3,float3>},
                                 {0,0,0},
                                 {0,0,0},
                                 {device::normalized<float,float>,0,device::normalized<float3,float3>}};
  auto func = funcs[_src.getElementSize() -1][_src.Channel() - 1];
  size_t srcStep = _src.Step();
  PtrStepSzb src(_src.Height(),_src.Width(),static_cast<unsigned char*>(const_cast<void *>(_src.data())),srcStep);
  size_t dstStep = _dst.Step();
  PtrStepSzb dst(_dst.Height(),_dst.Width(),static_cast<unsigned char *>(_dst.data()),dstStep);
  func(src,dst,alpha,beta,m_pStream);
}
}
}
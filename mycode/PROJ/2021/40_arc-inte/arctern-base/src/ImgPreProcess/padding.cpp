//
// Created by Admin on 2021/7/9.
//

#include "padding.h"
#include "cuda/padding.cuh"
namespace arctern{
namespace cuda{
void padding(const DeviceBuffer& _src,
             DeviceBuffer& _dst,
             int top,int left, float3 _borderValue,cudaStream_t *m_pStream){
  assert(_src.Width() > 0 && _src.Height() > 0);
  assert(_dst.Width() > 0 && _dst.Height() > 0);

  size_t srcStep = _src.Step();
  PtrStepSzb src(_src.Height(),_src.Width(),static_cast<unsigned char*>(const_cast<void *>(_src.data())),srcStep);
  size_t dstStep = _dst.Step();
  PtrStepSzb dst(_dst.Height(),_dst.Width(),static_cast<unsigned char *>(_dst.data()),dstStep);

  auto e = _src.getElementSize() - 1;
  auto c = _src.Channel() - 1;
  if(e == 0 && c == 0){
    device::padding<uchar>(src,dst,top,left,(uchar) _borderValue.x,m_pStream);
  }
  else if(e == 3 && c == 0){
    device::padding<float>(src,dst,top,left, _borderValue.x,m_pStream);
  }
  else if(e == 0 && c == 2){
    uchar3 tmp {(uchar) _borderValue.x,(uchar) _borderValue.y,(uchar) _borderValue.z};
    device::padding<uchar3>(src,dst,top,left, tmp,m_pStream);
  }
  else if(e == 3 && c == 2){
    device::padding<float3>(src,dst,top,left,_borderValue,m_pStream);
  }
}
}
}
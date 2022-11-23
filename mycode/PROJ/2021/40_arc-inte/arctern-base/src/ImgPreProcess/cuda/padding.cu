//
// Created by Admin on 2021/7/9.
//

#include "padding.cuh"
#include "vec_math.hpp"

namespace arctern{ namespace cuda{ namespace device{


template <typename T>
__global__ void padding_k(const PtrStepSz<T> src, PtrStepSz<T> dst,int top,int left,T borderValue)
{
  const int dst_x = blockDim.x * blockIdx.x + threadIdx.x;
  const int dst_y = blockDim.y * blockIdx.y + threadIdx.y;

  if (dst_x < dst.cols && dst_y < dst.rows)
  {
    /*if(dst_x < left || dst_y < top){
      dst(dst_y, dst_x) = borderValue;
    }
    else if(dst_x >= left + src.cols || dst_y >= top + src.rows){
      dst(dst_y, dst_x) = borderValue;
    } else{
      int src_x = dst_x - left;
      int src_y = dst_y - top;
      dst(dst_y,dst_x) = src(src_y,src_x);
    }*/

    if(dst_y >= top && dst_x >= left && dst_y < src.rows + top && dst_x < src.cols + left){
      int src_x = dst_x - left;
      int src_y = dst_y - top;
      dst(dst_y,dst_x) = src(src_y,src_x);
    } else{
      dst(dst_y,dst_x) = borderValue;
    }
  }
}

template <typename T>
void padding(const PtrStepSz<T>& src,const PtrStepSz<T>& dst,int top,int left,T borderValue,cudaStream_t *m_pStream)
{
  const dim3 block(32, 8);
  const dim3 grid(divUp(dst.cols, block.x), divUp(dst.rows, block.y));

  padding_k<<<grid, block, 0, *m_pStream>>>(src, dst, top, left, borderValue);

  cudaSafeCall( cudaGetLastError() );

  //cudaSafeCall(cudaStreamSynchronize(m_pStream));
}

template <typename T>
void padding(const PtrStepSzb& src, PtrStepSzb& dst,int top,int left, T borderValue,cudaStream_t *m_pStream)
{
  padding<T>(static_cast<PtrStepSz<T> >(src), static_cast<PtrStepSz<T> > (dst), top, left, borderValue, m_pStream);
}

template void padding<uchar>(const PtrStepSzb& src, PtrStepSzb& dst,int top,int left, uchar borderValue,cudaStream_t *m_pStream);
template void padding<float>(const PtrStepSzb& src, PtrStepSzb& dst,int top,int left, float borderValue,cudaStream_t *m_pStream);

template void padding<uchar3>(const PtrStepSzb& src, PtrStepSzb& dst,int top,int left, uchar3 borderValue,cudaStream_t *m_pStream);
template void padding<float3>(const PtrStepSzb& src, PtrStepSzb& dst,int top,int left, float3 borderValue,cudaStream_t *m_pStream);

}}}
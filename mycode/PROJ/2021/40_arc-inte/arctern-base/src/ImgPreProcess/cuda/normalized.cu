//
// Created by Admin on 2021/7/5.
//

#include "normalized.cuh"
#include "vec_math.hpp"

namespace arctern{ namespace cuda{ namespace device{
//binary operators (vecA *+ vecB)
#define ARCTERN_CUDA_OP(op,type) \
    __device__ __forceinline__ float operator op(const type &a,const float4 &b) { \
        return a op b.x; \
    }\
    __device__ __forceinline__ float ## 3 operator op(const type ## 3 &a,const float4 &b) { \
        return VecTraits<float ## 3>::make(a.x op b.x, a.y op b.y, a.z op b.z); \
    }\
    __device__ __forceinline__ float ## 4 operator op(const type ## 4 &a,const float4 &b) { \
        return VecTraits<float ## 4>::make(a.x op b.x, a.y op b.y, a.z op b.z, a.w op b.w); \
    }

ARCTERN_CUDA_OP(*,uchar)
ARCTERN_CUDA_OP(*,float)
ARCTERN_CUDA_OP(+,uchar)
ARCTERN_CUDA_OP(+,float)


template <typename T,typename U>
__global__ void normalized_k(const PtrStepSz<T> src, PtrStepSz<U> dst, const float4 alpha, const float4 beta)
{
  const int dst_x = blockDim.x * blockIdx.x + threadIdx.x;
  const int dst_y = blockDim.y * blockIdx.y + threadIdx.y;

  if (dst_x < dst.cols && dst_y < dst.rows)
  {
    const float src_x = dst_x ;
    const float src_y = dst_y ;

    dst(dst_y, dst_x) = src(src_y, src_x) * alpha + beta;
  }
}

template <typename T,typename U>
void normalized(const PtrStepSz<T>& src,const PtrStepSz<U>& dst, const float4 alpha, const float4 beta,cudaStream_t *m_pStream)
{
  const dim3 block(64, 16);
  const dim3 grid(divUp(dst.cols, block.x), divUp(dst.rows, block.y));

  normalized_k<<<grid, block,0, *m_pStream>>>(src, dst, alpha, beta);

  cudaSafeCall( cudaGetLastError() );

  //cudaSafeCall(cudaStreamSynchronize(m_pStream));
}

template <typename T,typename U>
void normalized(const PtrStepSzb& src, PtrStepSzb& dst, const float4 alpha, const float4 beta,cudaStream_t *m_pStream)
{
  normalized<T,U>(static_cast<PtrStepSz<T> >(src), static_cast<PtrStepSz<U> > (dst),  alpha,  beta ,m_pStream);
}

template void normalized<uchar,float>(const PtrStepSzb& src, PtrStepSzb& dst, const float4 alpha, const float4 beta,cudaStream_t *m_pStream);
template void normalized<float,float>(const PtrStepSzb& src, PtrStepSzb& dst, const float4 alpha, const float4 beta,cudaStream_t *m_pStream);

template void normalized<uchar3,float3>(const PtrStepSzb& src, PtrStepSzb& dst, const float4 alpha, const float4 beta,cudaStream_t *m_pStream);
template void normalized<float3,float3>(const PtrStepSzb& src, PtrStepSzb& dst, const float4 alpha, const float4 beta,cudaStream_t *m_pStream);
}}}
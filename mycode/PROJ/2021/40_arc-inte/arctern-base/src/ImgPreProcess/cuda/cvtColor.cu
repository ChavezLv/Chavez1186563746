//
// Created by Admin on 2021/7/7.
//

#include "cvtColor.cuh"
#include "common.hpp"
#include "util.hpp"
#include "vec_math.hpp"
namespace arctern {
namespace cuda {
namespace device {

template<typename T,typename U>
__global__ void cvtRGB2GRAY_k(const PtrStepSz<T> src, PtrStepSz<U> dst)
{
  const int dst_x = blockDim.x * blockIdx.x + threadIdx.x;
  const int dst_y = blockDim.y * blockIdx.y + threadIdx.y;

  if (dst_x < dst.cols && dst_y < dst.rows) {
    int src_x = dst_x;
    int src_y = dst_y;
    dst(dst_y,dst_x) = 0.299*src(src_y,src_x).x + 0.587*src(src_y,src_x).y + 0.114*src(src_y,src_x).z;
  }
}

template<typename T,typename U>
void cvtRGB2GRAY(const PtrStepSz<T> &src, PtrStepSz<U> dst,cudaStream_t *m_pStream) {
  const dim3 block(64, 16);
  const dim3 grid(divUp(dst.cols, block.x), divUp(dst.rows, block.y));

  cvtRGB2GRAY_k<T,U><<<grid, block,0,*m_pStream>>>(src, dst);

  cudaSafeCall(cudaGetLastError());

  //cudaSafeCall(cudaStreamSynchronize(m_pStream));
}

template<typename T,typename U>
void cvtRGB2GRAY(const PtrStepSzb &src, PtrStepSzb &dst,cudaStream_t *m_pStream) {
  cvtRGB2GRAY<T,U>(static_cast<PtrStepSz<T> >(src), static_cast<PtrStepSz<U> > (dst),m_pStream);
}

template void cvtRGB2GRAY<uchar3,uchar>(const PtrStepSzb &src, PtrStepSzb &dst,cudaStream_t *m_pStream);
template void cvtRGB2GRAY<float3,float>(const PtrStepSzb &src, PtrStepSzb &dst,cudaStream_t *m_pStream);


template<typename T,typename U>
__global__ void cvtBGR2GRAY_k(const PtrStepSz<T> src, PtrStepSz<U> dst)
{
  const int dst_x = blockDim.x * blockIdx.x + threadIdx.x;
  const int dst_y = blockDim.y * blockIdx.y + threadIdx.y;

  if (dst_x < dst.cols && dst_y < dst.rows) {
    int src_x = dst_x;
    int src_y = dst_y;
    dst(dst_y,dst_x) = 0.114*src(src_y,src_x).x + 0.587*src(src_y,src_x).y + 0.299*src(src_y,src_x).z;
  }
}

template<typename T,typename U>
void cvtBGR2GRAY(const PtrStepSz<T> &src, PtrStepSz<U> dst,cudaStream_t *m_pStream) {
  const dim3 block(64, 16);
  const dim3 grid(divUp(dst.cols, block.x), divUp(dst.rows, block.y));

  cvtBGR2GRAY_k<T,U><<<grid, block,0,*m_pStream>>>(src, dst);

  cudaSafeCall(cudaGetLastError());

  //cudaSafeCall(cudaStreamSynchronize(m_pStream));
}

template<typename T,typename U>
void cvtBGR2GRAY(const PtrStepSzb &src, PtrStepSzb &dst,cudaStream_t *m_pStream) {
  cvtBGR2GRAY<T,U>(static_cast<PtrStepSz<T> >(src), static_cast<PtrStepSz<U> > (dst),m_pStream);
}

template void cvtBGR2GRAY<uchar3,uchar>(const PtrStepSzb &src, PtrStepSzb &dst,cudaStream_t *m_pStream);
template void cvtBGR2GRAY<float3,float>(const PtrStepSzb &src, PtrStepSzb &dst,cudaStream_t *m_pStream);


template<typename T>
__global__ void cvtBGR2RGB_k(const PtrStepSz<T> src, PtrStepSz<T> dst)
{
  const int dst_x = blockDim.x * blockIdx.x + threadIdx.x;
  const int dst_y = blockDim.y * blockIdx.y + threadIdx.y;

  if (dst_x < dst.cols && dst_y < dst.rows) {
    int src_x = dst_x;
    int src_y = dst_y;
    dst(dst_y,dst_x).x = src(src_y,src_x).z;
    dst(dst_y,dst_x).y = src(src_y,src_x).y;
    dst(dst_y,dst_x).z = src(src_y,src_x).x;
  }
}

template<typename T>
void cvtBGR2RGB(const PtrStepSz<T> &src, PtrStepSz<T> dst,cudaStream_t *m_pStream) {
  const dim3 block(64, 16);
  const dim3 grid(divUp(dst.cols, block.x), divUp(dst.rows, block.y));

  cvtBGR2RGB_k<T> <<< grid, block,0 ,*m_pStream >>>(src, dst);

  cudaSafeCall(cudaGetLastError());

  //cudaSafeCall(cudaStreamSynchronize(m_pStream));
}

template<typename T>
void cvtBGR2RGB(const PtrStepSzb &src, PtrStepSzb &dst,cudaStream_t *m_pStream) {
  cvtBGR2RGB<T>(static_cast<PtrStepSz<T> >(src), static_cast<PtrStepSz<T> > (dst),m_pStream);
}

template void cvtBGR2RGB<uchar3>(const PtrStepSzb &src, PtrStepSzb &dst,cudaStream_t *m_pStream);
template void cvtBGR2RGB<float3>(const PtrStepSzb &src, PtrStepSzb &dst,cudaStream_t *m_pStream);


template<typename T,typename U>
__global__ void ExtractChannel_k(const PtrStepSz<T> src, PtrStepSz<T> dst)
{
  const int dst_x = blockDim.x * blockIdx.x + threadIdx.x;
  const int dst_y = blockDim.y * blockIdx.y + threadIdx.y;

  if (dst_x < dst.cols && dst_y < dst.rows) {
    U *px = (U*)(((uchar*)dst.ptr()) + (0 * dst.cols * dst.rows  + dst_y * dst.cols + dst_x)*sizeof(U) );
    U *py = (U*)(((uchar*)dst.ptr()) + (1 * dst.cols * dst.rows + dst_y * dst.cols + dst_x)*sizeof(U) );
    U *pz = (U*)(((uchar*)dst.ptr()) + (2 * dst.cols * dst.rows + dst_y * dst.cols + dst_x)*sizeof(U) );

    *px = src(dst_y,dst_x).x;
    *py = src(dst_y,dst_x).y;
    *pz = src(dst_y,dst_x).z;
  }
}

template<typename T,typename U>
void ExtractChannel(const PtrStepSz<T> &src, PtrStepSz<T> dst,cudaStream_t *m_pStream) {
  const dim3 block(64, 16);
  const dim3 grid(divUp(dst.cols, block.x), divUp(dst.rows, block.y));

  ExtractChannel_k<T,U><<<grid, block, 0 ,*m_pStream>>>(src, dst);

  cudaSafeCall(cudaGetLastError());

  //cudaSafeCall(cudaStreamSynchronize(m_pStream));
}

template<typename T,typename U>
void ExtractChannel(const PtrStepSzb &src, PtrStepSzb &dst,cudaStream_t *m_pStream) {
  ExtractChannel<T,U>(static_cast<PtrStepSz<T> >(src), static_cast<PtrStepSz<T> > (dst),m_pStream);
}

template void ExtractChannel<uchar3,uchar>(const PtrStepSzb &src, PtrStepSzb &dst,cudaStream_t *m_pStream);
template void ExtractChannel<float3,float>(const PtrStepSzb &src, PtrStepSzb &dst,cudaStream_t *m_pStream);

template<typename T, typename U>
__global__ void cvt3CTo4C_k(const PtrStepSz<T> src, PtrStepSz<U> dst) {
  const int dst_x = blockDim.x * blockIdx.x + threadIdx.x;
  const int dst_y = blockDim.y * blockIdx.y + threadIdx.y;

  if (dst_x < dst.cols && dst_y < dst.rows) {
    const float src_x = dst_x;
    const float src_y = dst_y;

    dst(dst_y, dst_x).x = src(src_y, src_x).x;
    dst(dst_y, dst_x).y = src(src_y, src_x).y;
    dst(dst_y, dst_x).z = src(src_y, src_x).z;
    dst(dst_y, dst_x).w = 255;
  }
}


template<typename T, typename U>
void cvt3CTo4C(const PtrStepSz<T> &src, const PtrStepSz<U> &dst) {
  const dim3 block(32, 8);
  const dim3 grid(divUp(dst.cols, block.x), divUp(dst.rows, block.y));

  cvt3CTo4C_k<<<grid, block>>>(src, dst);

  cudaSafeCall(cudaGetLastError());

  cudaSafeCall(cudaDeviceSynchronize());
}

template<typename T, typename U>
void cvt3CTo4C(const PtrStepSzb &src, PtrStepSzb &dst) {
  cvt3CTo4C<T, U>(static_cast<PtrStepSz<T> >(src), static_cast<PtrStepSz<U> > (dst));
}

template void cvt3CTo4C<uchar3, uchar4>(const PtrStepSzb &src, PtrStepSzb &dst);
template void cvt3CTo4C<float3, float4>(const PtrStepSzb &src, PtrStepSzb &dst);


template<typename T, typename U>
__global__ void cvt4CTo3C_k(const PtrStepSz<T> src, PtrStepSz<U> dst) {
  const int dst_x = blockDim.x * blockIdx.x + threadIdx.x;
  const int dst_y = blockDim.y * blockIdx.y + threadIdx.y;

  if (dst_x < dst.cols && dst_y < dst.rows) {
    const float src_x = dst_x;
    const float src_y = dst_y;

    dst(dst_y, dst_x).x = src(src_y, src_x).x;
    dst(dst_y, dst_x).y = src(src_y, src_x).y;
    dst(dst_y, dst_x).z = src(src_y, src_x).z;
  }
}

template<typename T, typename U>
void cvt4CTo3C(const PtrStepSz<T> &src, const PtrStepSz<U> &dst) {
  const dim3 block(32, 8);
  const dim3 grid(divUp(dst.cols, block.x), divUp(dst.rows, block.y));

  cvt4CTo3C_k<<<grid, block>>>(src, dst);

  cudaSafeCall(cudaGetLastError());

  cudaSafeCall(cudaDeviceSynchronize());
}

template<typename T, typename U>
void cvt4CTo3C(const PtrStepSzb &src, PtrStepSzb &dst) {
  cvt4CTo3C<T, U>(static_cast<PtrStepSz<T> >(src), static_cast<PtrStepSz<U> > (dst));
}

template void cvt4CTo3C<uchar4, uchar3>(const PtrStepSzb &src, PtrStepSzb &dst);
template void cvt4CTo3C<float4, float3>(const PtrStepSzb &src, PtrStepSzb &dst);

}
}
}
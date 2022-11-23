//
// Created by Admin on 2021/11/1.
//

#include "crop.cuh"
namespace arctern{ namespace cuda{ namespace device{


template <typename T>
__global__ void crop_k(const PtrStepSz<T> src, PtrStepSz<T> dst, ArcternRect rROI)
{
  const int dst_x = blockDim.x * blockIdx.x + threadIdx.x;
  const int dst_y = blockDim.y * blockIdx.y + threadIdx.y;

  if (dst_x < dst.cols && dst_y < dst.rows)
  {
      int src_x = dst_x + (int)rROI.x;
      int src_y = dst_y + (int)rROI.y;
      dst(dst_y,dst_x) = src(src_y,src_x);
  }
}

template <typename T>
void crop(const PtrStepSz<T>& src,const PtrStepSz<T>& dst, ArcternRect rROI,cudaStream_t *m_pStream)
{
  const dim3 block(32, 8);
  const dim3 grid(divUp(dst.cols, block.x), divUp(dst.rows, block.y));

  crop_k<<<grid, block,0,*m_pStream>>>(src, dst, rROI);

  cudaSafeCall( cudaGetLastError() );

  //cudaSafeCall(cudaStreamSynchronize(m_pStream));
}

template <typename T>
void crop(const PtrStepSzb& src, PtrStepSzb& dst, ArcternRect rROI,cudaStream_t *m_pStream)
{

  crop<T>(static_cast<PtrStepSz<T> >(src), static_cast<PtrStepSz<T> > (dst), rROI,m_pStream);
}

template void crop<uchar>(const PtrStepSzb& src, PtrStepSzb& dst, ArcternRect rROI,cudaStream_t *m_pStream);
template void crop<float>(const PtrStepSzb& src, PtrStepSzb& dst, ArcternRect rROI,cudaStream_t *m_pStream);

template void crop<uchar3>(const PtrStepSzb& src, PtrStepSzb& dst, ArcternRect rROI,cudaStream_t *m_pStream);
template void crop<float3>(const PtrStepSzb& src, PtrStepSzb& dst, ArcternRect rROI,cudaStream_t *m_pStream);

}}}
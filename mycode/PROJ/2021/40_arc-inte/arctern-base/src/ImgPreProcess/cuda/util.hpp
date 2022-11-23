//
// Created by Admin on 2021/6/29.
//

#ifndef CUDA_RESIZE_CUDA_UTIL_HPP_
#define CUDA_RESIZE_CUDA_UTIL_HPP_
#include "common.hpp"
#include "texture_fetch_functions.h"

namespace arctern { namespace cuda {
inline void checkCudaError(cudaError_t err, const char* file, const int line)
{
  if (cudaSuccess != err)
    std::cout << "file:" << file <<"  line:"<< line<<"  message:"<< cudaGetErrorString(err)<<std::endl;
}
}}

namespace arctern { namespace cuda
{

#define cudaSafeCall(expr)  arctern::cuda::checkCudaError(expr, __FILE__, __LINE__)

namespace device
{
__host__ __device__ __forceinline__ int divUp(int total, int grain)
{
  return (total + grain - 1) / grain;
}

template<class T> inline void bindTexture(const textureReference* tex, const PtrStepSz<T>& img)
{
  cudaChannelFormatDesc desc = cudaCreateChannelDesc<T>();
  cudaSafeCall( cudaBindTexture2D(0, tex, img.ptr(), &desc, img.cols, img.rows, img.step) );
}

template<class T> inline void createTextureObjectPitch2D(cudaTextureObject_t* tex, PtrStepSz<T>& img, const cudaTextureDesc& texDesc)
{
  cudaResourceDesc resDesc;
  memset(&resDesc, 0, sizeof(resDesc));
  resDesc.resType = cudaResourceTypePitch2D;
  resDesc.res.pitch2D.devPtr = static_cast<void*>(img.ptr());
  resDesc.res.pitch2D.height = img.rows;
  resDesc.res.pitch2D.width = img.cols;
  resDesc.res.pitch2D.pitchInBytes = img.step;
  resDesc.res.pitch2D.desc = cudaCreateChannelDesc<T>();

  cudaSafeCall( cudaCreateTextureObject(tex, &resDesc, &texDesc, NULL) );
}
}
}}









#endif //CUDA_RESIZE_CUDA_UTIL_HPP_

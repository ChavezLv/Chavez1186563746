//
// Created by gpu on 19-12-2.
//
#include "ModSaclarKernal.h"

template <typename DType>
__global__ void ModSaclarKernel(const int threadNum, DType* out, const DType* input, int scalar){
  const int index = blockIdx.x * blockDim.x + threadIdx.x;
  if (index < threadNum)
  {
    out[index] = (int)input[index] % scalar;
  }
}

template <typename DType>
__global__ void DivSaclarKernel(const int threadNum, DType* out, const DType* input, int scalar){
  const int index = blockIdx.x * blockDim.x + threadIdx.x;
  if (index < threadNum)
  {
    out[index] = input[index] / scalar;
  }
}

pluginStatus_t ModSaclarInference(cudaStream_t stream, const int& threadNum, void* outputs, const void* inputs,
                                  int& scalar, int& mode){
  int kMaxGridNum = 65536;
  dim3 block(256);
  dim3 grid(std::min(kMaxGridNum, (int)((threadNum + block.x -1) / block.x)));
  switch (mode){
    case 0:/// mod op pass to element
        ModSaclarKernel<float><<<grid, block, 0, stream>>>(threadNum, (float*)outputs, (float*)inputs, scalar);
        break;
    case 1: /// div op pass to element
        DivSaclarKernel<float><<<grid, block, 0, stream>>>(threadNum, (float*)outputs, (float*)inputs, scalar);
        break;
    default: break;
  }

  return STATUS_SUCCESS;
}

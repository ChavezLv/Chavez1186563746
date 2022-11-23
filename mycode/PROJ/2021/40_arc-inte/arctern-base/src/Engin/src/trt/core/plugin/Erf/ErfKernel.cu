//
// Created by gpu on 19-12-2.
//
#include "ErfKernel.h"
#include "cuda_runtime.h"

template <typename DType>
__global__ void ErfKernel(const int threadNum, DType* out, const DType* input){
  const int index = blockIdx.x * blockDim.x + threadIdx.x;
  if (index < threadNum)
  {
    out[index] = erf(input[index]);
  }
}

pluginStatus_t ErfInference(cudaStream_t stream, const int threadNum, void* outputs, const void* inputs){
  const int kMaxGridNum = 65536;
  dim3 block(256);
  dim3 grid(std::min(kMaxGridNum, (int)((threadNum + block.x -1) / block.x)));
  ErfKernel<float><<<grid, block, 0, stream>>>(threadNum, (float*)outputs, (float*)inputs);
  return STATUS_SUCCESS;
}
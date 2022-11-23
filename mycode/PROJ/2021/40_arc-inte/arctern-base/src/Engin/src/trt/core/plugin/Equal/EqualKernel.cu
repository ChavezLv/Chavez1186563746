//
// Created by gpu on 19-12-2.
//
#include "EqualKernel.h"

template <typename DType>
__global__ void EqualKernel(const int threadNum, DType* out, const DType* inputs_0, const DType* inputs_1){
  const int index = blockIdx.x * blockDim.x + threadIdx.x;
  if (index < threadNum)
  {
    out[index] = (inputs_0[index] == inputs_1[index]) ? 1 : 0;
  }
}

pluginStatus_t EqualInference(cudaStream_t stream, const int threadNum, void* outputs, const void* inputs_0,
                              const void* inputs_1){
  const int kMaxGridNum = 65536;
  dim3 block(256);
  dim3 grid(std::min(kMaxGridNum, (int)((threadNum + block.x -1) / block.x)));
  EqualKernel<float><<<grid, block, 0, stream>>>(threadNum, (float*)outputs, (float*)inputs_0, (float*)inputs_1);
  return STATUS_SUCCESS;
}
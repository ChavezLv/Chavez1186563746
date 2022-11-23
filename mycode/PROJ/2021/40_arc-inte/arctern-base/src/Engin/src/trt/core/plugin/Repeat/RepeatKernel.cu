//
// Created by gpu on 19-12-2.
//
#include "RepeatKernel.h"

template <typename DType>
__global__ void RepeatKernel_axis2(DType* output, const DType* input,
                                   const int input_shape_c, const int input_shape_h, const int input_shape_w,
                                   const int output_shape_c, const int output_shape_h, const int output_shape_w,
                                   int repeat){
  int index = blockIdx.x * blockDim.x + threadIdx.x;
  int plane = blockIdx.y;
  int batch = blockIdx.z;
  if(index >= output_shape_h * output_shape_w){
    return;
  }

  int dst_x = index % output_shape_w;
  int dst_y = index / output_shape_w;

  int src_x = dst_x;
  int src_y = dst_y / repeat;

  DType src_value = input[batch * input_shape_c * input_shape_h * input_shape_w +
                          plane * input_shape_h * input_shape_w + src_y * input_shape_w + src_x];
  output[batch * output_shape_c * output_shape_h * output_shape_w + plane * output_shape_h * output_shape_w +
         dst_y * output_shape_w + dst_x] = src_value;
}

template <typename DType>
__global__ void RepeatKernel_axis3(DType* output, const DType* input,
                                   const int input_shape_c, const int input_shape_h, const int input_shape_w,
                                   const int output_shape_c, const int output_shape_h, const int output_shape_w,
                                   int repeat){
  int index = blockIdx.x * blockDim.x + threadIdx.x;
  int plane = blockIdx.y;
  int batch = blockIdx.z;
  if(index >= output_shape_h * output_shape_w){
    return;
  }

  int dst_x = index % output_shape_w;
  int dst_y = index / output_shape_w;

  int src_x = dst_x / repeat;
  int src_y = dst_y;

  DType src_value = input[batch * input_shape_c * input_shape_h * input_shape_w +
                          plane * input_shape_h * input_shape_w + src_y * input_shape_w + src_x];
  output[batch * output_shape_c * output_shape_h * output_shape_w + plane * output_shape_h * output_shape_w +
         dst_y * output_shape_w + dst_x] = src_value;
}

pluginStatus_t RepeatInference(cudaStream_t stream, void* outputs_0, const void* inputs_0,
                               int batchsize, const int* input_shape, const int* output_shape,
                               int repeats, int axis){
  const int kBaseThreadBits = 8;
  const int kBaseThreadNum = 1 << kBaseThreadBits;
  int xGridSize = (output_shape[2] * output_shape[3] + kBaseThreadNum - 1) / kBaseThreadNum;
  dim3 dimGrid(xGridSize, output_shape[1], output_shape[0] * batchsize);

  if(axis == 2){
    RepeatKernel_axis2<float><<<dimGrid, kBaseThreadNum, 0, stream>>>((float*)outputs_0, (float*)inputs_0, input_shape[1],
            input_shape[2], input_shape[3], output_shape[1], output_shape[2], output_shape[3], repeats);
  }else if(axis == 3){
    RepeatKernel_axis3<float><<<dimGrid, kBaseThreadNum, 0, stream>>>((float*)outputs_0, (float*)inputs_0, input_shape[1],
            input_shape[2], input_shape[3], output_shape[1], output_shape[2], output_shape[3], repeats);
  }

  return STATUS_SUCCESS;
}
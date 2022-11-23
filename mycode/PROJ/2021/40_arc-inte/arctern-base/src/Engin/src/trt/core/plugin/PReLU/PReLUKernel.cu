#include "PReLUKernel.h"

template <typename DType>
__global__ void PReluKernel(const int num_threads, DType* out, const DType* in_data, const DType* in_alpha,
    const int channel_stride, const int alpha_stride)
{
    int x0 = blockIdx.x * blockDim.x + threadIdx.x;
    for (int index = x0; index < num_threads; index += gridDim.x * blockDim.x)
    {
        int c = (index / channel_stride) % alpha_stride;
        out[index] = in_data[index] > 0 ? in_data[index] : in_data[index] * in_alpha[c];
    }
}

pluginStatus_t PReLUInference(cudaStream_t stream, const int num_threads, void* outputs, const void* inputs_data,
    const void* inputs_alpha, const int ndim, const int* input_data_shape)
{
    const int kMaxGridNum = 65535;
    const int kBaseThreadBits = 8;
    const int kBaseThreadNum = 1 << kBaseThreadBits;

    int ngrid = std::min(kMaxGridNum, (num_threads + kBaseThreadNum - 1) / kBaseThreadNum);

    if (ndim == 2)
    {
        PReluKernel<float><<<ngrid, kBaseThreadNum, 0, stream>>>(
            num_threads, (float*) outputs, (float*) inputs_data, (float*) inputs_alpha, 1, input_data_shape[1]);
    }
    else if (ndim == 3)
    {
        PReluKernel<float><<<ngrid, kBaseThreadNum, 0, stream>>>(num_threads, (float*) outputs, (float*) inputs_data,
            (float*) inputs_alpha, input_data_shape[2], input_data_shape[1]);
    }
    else if (ndim == 4)
    {
        PReluKernel<float><<<ngrid, kBaseThreadNum, 0, stream>>>(num_threads, (float*) outputs, (float*) inputs_data,
            (float*) inputs_alpha, input_data_shape[2] * input_data_shape[3], input_data_shape[1]);
    }

    return STATUS_SUCCESS;
}

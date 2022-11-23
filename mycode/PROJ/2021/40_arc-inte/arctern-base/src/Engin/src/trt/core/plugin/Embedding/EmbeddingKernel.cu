#include "EmbeddingKernel.h"

template <typename DType, typename IType, bool clip = true>
__global__ void EmbeddingKernel(
    const int num_threads, DType* out_data, const DType* in_data, const IType* idx, const int M, const int K)
{
    int x0 = blockIdx.x * blockDim.x + threadIdx.x;

    for (int index = x0; index < num_threads; index += gridDim.x * blockDim.x)
    {
        int j = idx[index / M];
        if (clip)
        {
            if (j <= 0)
                j = 0;
            else if (j >= K)
                j = K - 1;
        }
        else
        {
            j = j % K;
            j += (j < 0) ? K : 0;
        }
        out_data[index] = in_data[j * M + index % M];
    }
}

pluginStatus_t EmbeddingInference(cudaStream_t stream, const int num_threads, void* outputs, const void* inputs_data,
    const void* idx, const int output_dim, const int input_dim)
{
    const int kMaxGridNum = 65535;
    const int kBaseThreadBits = 8;
    const int kBaseThreadNum = 1 << kBaseThreadBits;

    int ngrid = std::min(kMaxGridNum, (num_threads + kBaseThreadNum - 1) / kBaseThreadNum);

    EmbeddingKernel<float><<<ngrid, kBaseThreadNum, 0, stream>>>(
        num_threads, (float*) outputs, (float*) inputs_data, (float*) idx, output_dim, input_dim);

    return STATUS_SUCCESS;
}

#include "../cub/block/block_reduce.cuh"

#include "L2NormalizationKernel.h"

template <typename DType, int BLOCK_THREADS>
__global__ void L2NormalizationKernel(
    DType* out, const DType* in, const int batch_size, const int batch_step, const float eps)
{
    typedef cub::BlockReduce<float, BLOCK_THREADS> BlockReduce;
    __shared__ typename BlockReduce::TempStorage temp_storage;

    int x0 = threadIdx.x;
    int batch0 = blockIdx.x;

    for (int batch = batch0; batch < batch_size; batch += gridDim.x)
    {
        int offset = batch * batch_step;

        float sum = 0.0f;
        __shared__ float norm;

        for (int index = x0; index < batch_step; index += blockDim.x)
        {
            const float data = in[offset + index];
            sum += data * data;
        }
        float reduce_result = BlockReduce(temp_storage).Sum(sum);

        if (threadIdx.x == 0)
        {
            norm = sqrtf(reduce_result);
            norm = fmaxf(norm, eps);
        }
        __syncthreads();

        for (int index = x0; index < batch_step; index += blockDim.x)
        {
            out[offset + index] = in[offset + index] / norm;
        }
    }
}

pluginStatus_t L2NormalizationInference(cudaStream_t stream, void* outputs, const void* inputs_data,
    const int batch_size, const int batch_step, const float eps)
{
    const int kMaxGridNum = 65535;
    const int kBaseThreadBits = 8;
    const int kBaseThreadNum = 1 << kBaseThreadBits;

    int ngrid = std::min(kMaxGridNum, batch_size);

    L2NormalizationKernel<float, kBaseThreadNum>
        <<<ngrid, kBaseThreadNum, 0, stream>>>((float*) outputs, (float*) inputs_data, batch_size, batch_step, eps);

    return STATUS_SUCCESS;
}

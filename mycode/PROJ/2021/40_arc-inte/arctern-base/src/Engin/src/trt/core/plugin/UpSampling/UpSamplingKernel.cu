#include "UpSamplingKernel.h"

template <typename DType>
__global__ void UpsamplingKernel(DType* out, const DType* in, const int batchSize, const int input_h, const int input_w,
    const int output_h, const int output_w, const int scale)
{
    int x0 = threadIdx.x + blockIdx.x * blockDim.x;
    int y0 = threadIdx.y + blockIdx.y * blockDim.y;
    int z0 = blockIdx.z;

    int istride = input_w;
    int ostride = output_w;
    int ibatchstride = input_h * istride;
    int obatchstride = output_h * ostride;

    for (int batch = z0; batch < batchSize; batch += gridDim.z)
    {
        for (int oy = y0; oy < output_h; oy += blockDim.y * gridDim.y)
        {
            for (int ox = x0; ox < output_w; ox += blockDim.x * gridDim.x)
            {
                int ix = int(ox / scale);
                int iy = int(oy / scale);
                out[batch * obatchstride + oy * ostride + ox] = in[batch * ibatchstride + iy * istride + ix];
            }
        }
    }
}

pluginStatus_t UpSamplingInference(cudaStream_t stream, void* outputs, const void* inputs, int batchSize,
    const int ndim, const int* input_shape, const int* output_shape, const int scale)
{
    const int kMaxGridNum = 65535;
    dim3 block(32, 16);
    dim3 grid((output_shape[3] - 1) / block.x + 1, (output_shape[2] - 1) / block.y + 1,
        std::min(output_shape[0] * output_shape[1] * batchSize, kMaxGridNum));

    UpsamplingKernel<float><<<grid, block, 0, stream>>>((float*) outputs, (float*) inputs,
        output_shape[0] * output_shape[1] * batchSize, input_shape[2], input_shape[3], output_shape[2], output_shape[3],
        scale);
    return STATUS_SUCCESS;
}

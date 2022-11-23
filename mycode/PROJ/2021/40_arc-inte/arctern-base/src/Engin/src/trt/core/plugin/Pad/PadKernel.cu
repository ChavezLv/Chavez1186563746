#include "PadKernel.h"

template <typename DType>
__global__ void image_2d_pad_edge_kernel(DType* dst, const DType* src, const int input_shape_c, const int input_shape_h,
    const int input_shape_w, const int output_shape_c, const int output_shape_h, const int output_shape_w,
    const int padT, const int padL)
{
    int outputPointId = threadIdx.x + blockIdx.x * blockDim.x;
    int plane = blockIdx.y;
    int batch = blockIdx.z;
    if (outputPointId >= output_shape_h * output_shape_w)
    {
        return;
    }
    int outputPointX = outputPointId % output_shape_w;
    int outputPointY = outputPointId / output_shape_w;

    int iStartX = max(0, -padL);
    int iStartY = max(0, -padT);
    int oStartX = max(0, padL);
    int oStartY = max(0, padT);

    int inputPointX = min(max(padL, outputPointX), static_cast<int>(input_shape_w) + padL - 1) - oStartX + iStartX;
    int inputPointY = min(max(padT, outputPointY), static_cast<int>(input_shape_h) + padT - 1) - oStartY + iStartY;

    DType valueToCopy = src[batch * input_shape_c * input_shape_h * input_shape_w
        + plane * input_shape_h * input_shape_w + inputPointY * input_shape_w + inputPointX];
    dst[batch * output_shape_c * output_shape_h * output_shape_w + plane * output_shape_h * output_shape_w
        + outputPointY * output_shape_w + outputPointX]
        = valueToCopy;

    //  DType valueToCopy = src[batch][plane][inputPointY][inputPointX];
    //  dst[batch][plane][outputPointY][outputPointX] = valueToCopy;
}

template <typename DType>
inline void image_pad_edge(cudaStream_t stream, DType* dst, const DType* src, const int batchSize, const int ndim,
    const int* input_shape, const int* output_shape, const std::vector<int> pad)
{
    const int kBaseThreadBits = 8;
    const int kBaseThreadNum = 1 << kBaseThreadBits;

    if (ndim == 4)
    {
        const int padT = pad[4];
        const int padL = pad[6];
        int xGridSize = (output_shape[2] * output_shape[3] + kBaseThreadNum - 1) / kBaseThreadNum;
        dim3 dimGrid(xGridSize, output_shape[1], output_shape[0] * batchSize);
        image_2d_pad_edge_kernel<DType><<<dimGrid, kBaseThreadNum, 0, stream>>>(dst, src, input_shape[1],
            input_shape[2], input_shape[3], output_shape[1], output_shape[2], output_shape[3], padT, padL);
    }
}

template <typename DType>
__global__ void image_2d_pad_constant_kernel(DType* dst, const DType* src, const int input_shape_c,
    const int input_shape_h, const int input_shape_w, const int output_shape_c, const int output_shape_h,
    const int output_shape_w, const int padT, const int padL, const DType constant)
{
    int outputPointId = threadIdx.x + blockIdx.x * blockDim.x;
    int plane = blockIdx.y;
    int batch = blockIdx.z;
    if (outputPointId >= output_shape_h * output_shape_w)
    {
        return;
    }
    // cast sizes to int to use in min/max
    int Ny = input_shape_h;
    int Nx = input_shape_w;

    int outputPointX = outputPointId % output_shape_w;
    int outputPointY = outputPointId / output_shape_w;

    int checkT = max(0, outputPointY - padT + 1);
    int checkB = max(0, padT + Ny - outputPointY);
    int checkL = max(0, outputPointX - padL + 1);
    int checkR = max(0, padL + Nx - outputPointX);

    int inputPointX = min(max(outputPointX - padL, 0), Nx - 1);
    int inputPointY = min(max(outputPointY - padT, 0), Ny - 1);

    // 1 if need padding, 0 if not
    int need_pad = !(checkT * checkB * checkL * checkR);

    DType valueToCopy = src[batch * input_shape_c * input_shape_h * input_shape_w
        + plane * input_shape_h * input_shape_w + inputPointY * input_shape_w + inputPointX];
    dst[batch * output_shape_c * output_shape_h * output_shape_w + plane * output_shape_h * output_shape_w
        + outputPointY * output_shape_w + outputPointX]
        = valueToCopy * (!need_pad) + need_pad * constant;
}

template <typename DType>
inline void image_pad_constant(cudaStream_t stream, DType* dst, const DType* src, const int batchSize, const int ndim,
    const int* input_shape, const int* output_shape, const std::vector<int> pad, const float constant)
{
    const int kBaseThreadBits = 8;
    const int kBaseThreadNum = 1 << kBaseThreadBits;

    if (ndim == 4)
    {
        const int padT = pad[4];
        const int padL = pad[6];
        int xGridSize = (output_shape[2] * output_shape[3] + kBaseThreadNum - 1) / kBaseThreadNum;
        dim3 dimGrid(xGridSize, output_shape[1], output_shape[0] * batchSize);
        image_2d_pad_constant_kernel<DType><<<dimGrid, kBaseThreadNum, 0, stream>>>(dst, src, input_shape[1],
            input_shape[2], input_shape[3], output_shape[1], output_shape[2], output_shape[3], padT, padL, constant);
    }
}

pluginStatus_t PadInference(cudaStream_t stream, void* outputs, const void* inputs, const int batchSize, const int ndim,
    const int* input_shape, const int* output_shape, const std::vector<int> pad_width, const int mode,
    const float constant_value)
{
    switch (mode)
    {
    case pad_enum::kEdge:
        image_pad_edge<float>(
            stream, (float*) outputs, (float*) inputs, batchSize, ndim, input_shape, output_shape, pad_width);
        break;
    case pad_enum::kConstant:
        image_pad_constant<float>(stream, (float*) outputs, (float*) inputs, batchSize, ndim, input_shape, output_shape,
            pad_width, constant_value);
        break;
        //  case pad_enum::kReflect:
        //    image_pad_reflect<float>(stream, (float *)outputs, (float *)inputs,
        //                             pad_width);
        //    break;
    default: break;
    }
    return STATUS_SUCCESS;
}
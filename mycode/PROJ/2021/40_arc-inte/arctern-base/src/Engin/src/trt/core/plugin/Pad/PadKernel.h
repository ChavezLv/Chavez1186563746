#pragma once

#include "NvInfer.h"
#include "../common/plugin.h"

namespace pad_enum
{
enum PadOpInputs
{
    kData
};
enum PadOpType
{
    kConstant,
    kEdge,
    kReflect
};
enum PadOpOutputs
{
    kOut
};
} // namespace pad_enum

pluginStatus_t PadInference(cudaStream_t stream, void* outputs, const void* inputs, const int batchSize, const int ndim,
    const int* input_shape, const int* output_shape, const std::vector<int> pad_width, const int mode,
    const float constant_value);

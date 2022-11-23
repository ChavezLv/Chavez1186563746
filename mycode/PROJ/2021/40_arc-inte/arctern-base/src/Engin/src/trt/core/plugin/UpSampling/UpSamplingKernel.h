#pragma once

#include "NvInfer.h"
#include "../common/plugin.h"

pluginStatus_t UpSamplingInference(cudaStream_t stream, void* outputs, const void* inputs, const int batchSize,
    const int ndim, const int* input_shape, const int* output_shape, const int scale);
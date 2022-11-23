#pragma once

#include "NvInfer.h"
#include "../common/plugin.h"

pluginStatus_t PReLUInference(cudaStream_t stream, const int num_threads, void* outputs, const void* inputs_data,
    const void* inputs_alpha, const int ndim, const int* input_data_shape);

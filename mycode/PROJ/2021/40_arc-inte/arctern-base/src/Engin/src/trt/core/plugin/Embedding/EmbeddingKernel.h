#pragma once

#include "NvInfer.h"
#include "../common/plugin.h"

pluginStatus_t EmbeddingInference(cudaStream_t stream, const int num_threads, void* outputs, const void* inputs_data,
    const void* idx, const int output_dim, const int input_dim);

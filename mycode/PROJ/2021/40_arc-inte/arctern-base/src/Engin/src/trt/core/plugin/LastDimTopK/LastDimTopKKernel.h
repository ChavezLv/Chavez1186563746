#pragma once

#include "NvInfer.h"
#include "../common/plugin.h"

pluginStatus_t LastDimTopKInference(cudaStream_t stream, const int num_threads, void* out_value, void* out_indices,
    const void* in_data, const int K, const int N, const bool is_ascend);

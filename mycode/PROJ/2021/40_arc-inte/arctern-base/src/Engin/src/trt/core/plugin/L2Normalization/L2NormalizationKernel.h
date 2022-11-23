#pragma once

#include "NvInfer.h"
#include "../common/plugin.h"

pluginStatus_t L2NormalizationInference(cudaStream_t stream, void* outputs, const void* inputs_data,
    const int batch_size, const int batch_step, const float eps);

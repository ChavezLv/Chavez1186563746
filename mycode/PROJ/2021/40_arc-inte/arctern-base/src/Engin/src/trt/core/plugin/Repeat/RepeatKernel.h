//
// Created by gpu on 19-12-2.
//

#ifndef ARCTERN_REPEATKERNEL_H
#define ARCTERN_REPEATKERNEL_H

#include "NvInfer.h"
#include "../common/plugin.h"

pluginStatus_t RepeatInference(cudaStream_t stream, void* outputs_0, const void* inputs_0,
                               int batchsize, const int* input_shape, const int* output_shape,
                               int repeats, int axis);

#endif //ARCTERN_EQUALKERNEL_H

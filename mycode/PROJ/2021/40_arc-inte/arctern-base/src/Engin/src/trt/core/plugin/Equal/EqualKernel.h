//
// Created by gpu on 19-12-2.
//

#ifndef ARCTERN_EQUALKERNEL_H
#define ARCTERN_EQUALKERNEL_H

#include "NvInfer.h"
#include "../common/plugin.h"

pluginStatus_t EqualInference(cudaStream_t stream, const int threadNum, void* outputs, const void* inputs_0,
                              const void* inputs_1);

#endif //ARCTERN_EQUALKERNEL_H

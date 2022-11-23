//
// Created by gpu on 19-12-2.
//

#ifndef ARCTERN_ERFKERNEL_H
#define ARCTERN_ERFKERNEL_H

#include "NvInfer.h"
#include "../common/plugin.h"

pluginStatus_t ErfInference(cudaStream_t stream, const int threadNum, void* outputs, const void* inputs);

#endif //ARCTERN_EQUALKERNEL_H

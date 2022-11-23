//
// Created by gpu on 19-12-2.
//

#ifndef ARCTERN_MODSACLARKERNAL_H
#define ARCTERN_MODSACLARKERNAL_H
#include "NvInfer.h"
#include "../common/plugin.h"

pluginStatus_t ModSaclarInference(cudaStream_t stream, const int& threadNum, void* outputs, const void* inputs,
                                  int& scalar, int& mode);
#endif //ARCTERN_MODSACLARKERNAL_H

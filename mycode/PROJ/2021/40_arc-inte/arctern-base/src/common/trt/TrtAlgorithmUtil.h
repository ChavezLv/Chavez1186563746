//
// Created by Admin on 2021/6/18.
//

#ifndef ARCTERN_BASE_SRC_COMMON_TRT_TRTALGORITHMUTIL_H_
#define ARCTERN_BASE_SRC_COMMON_TRT_TRTALGORITHMUTIL_H_
#include "include/arctern.h"
#include <iostream>
#include <array>
#include <common/algUtil.h>
#include "src/common/common.h"
using namespace std;
using namespace arctern;
extern ArcternRect CheckShrinkBox(const ArcternRect& box, int image_width,int image_height);
extern ArcternRect GetFace(int width, int height,
                           const ArcternRect&bbox,
                           std::array<float, 4> extScale,
                           Float2Int type = Float2Int::ROUNDING,
                           bool square_rect = true);
#endif //ARCTERN_BASE_SRC_COMMON_TRT_TRTALGORITHMUTIL_H_

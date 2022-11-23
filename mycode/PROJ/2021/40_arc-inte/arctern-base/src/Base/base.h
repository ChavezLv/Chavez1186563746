//
// Created by Admin on 2021/8/18.
//

#ifndef ARCTERN_BASE_SRC_BASE_BASE_H_
#define ARCTERN_BASE_SRC_BASE_BASE_H_
/// \brief image type
typedef enum {
  U8C1 = 0,
  U8C3,
  F32C1,
  F32C3
} ImageType;

/// \brief image color cvt
typedef enum {
  RGB2BGR,
  BGR2RGB,
  BGR2GRAY,
  RGB2GRAY,
  GRAY2BGR,
  GRAY2RGB,
  YUV2BGR_NV21,
  YUV2RGB_NV21,
  YUV2BGR_I420,
  YUV2RGB_I420,
  YUV2GRAY_NV21,
  YUV2GRAY_I420,
  RGB2YUV_I420,
  BGR2YUV_I420
} ColorCvtType;

/// \brief interpolation
typedef enum {
  NEAREST = 0,
  LINEAR = 1,
  CUBIC = 2,
  AREA = 3
} ArcternInterpolation;
#endif //ARCTERN_BASE_SRC_BASE_BASE_H_

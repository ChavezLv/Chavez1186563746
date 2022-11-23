//
// Created by gh on 2021/10/31.
//

#ifndef ARCTERN_BASE_SRC_PROCESSORS_COMMON_H_
#define ARCTERN_BASE_SRC_PROCESSORS_COMMON_H_

namespace arctern {

enum class AlignFace {
  NoAlignFace = 0,
  AlignFaceWith2point = 1,
  NoAlignFaceExt = 2, ///< special impl, now just for NoiseMotionblur
  CropAlignFace = 3   // now just for faceJoy
};

enum class AlignMent {
  AlignCenter = 0,
  AlignLeft = 1
};

enum class DetType {
  Face = 0,
  MultiCat = 1
};


enum class OperatorType {
  cvtColor,
  crop,
  affine,
  padding,
  normalized,
  splitNormalized,
  extractChannel,
  resized,
  twiceResized,
  detFace,
  detFace2,
  multiCat,
  merge,
  ///> merge is diff with mergeChannel,
  ///> mergeChannel is th inverse process of extractChannel,
  ///> but merge is not
  mergeChannel,
  faceJoy,
  faceExp,
  faceFeat,   //for gluon7.2 or later
  faceWipe,  //  口罩擦除
};

typedef struct  {
  ColorCvtType code;
  ArcternRect roi;
  bool isExtractChannel;
  std::vector<ArcternPoint> m_lmk;

}OperatorParam;

}

#endif //ARCTERN_BASE_SRC_PROCESSORS_COMMON_H_

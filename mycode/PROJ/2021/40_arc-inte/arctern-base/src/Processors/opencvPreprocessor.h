//
// Created by gh on 2021/10/31.
//

#ifndef ARCTERN_BASE_SRC_PROCESSORS_OPENCVPREPROCESSOR_H_
#define ARCTERN_BASE_SRC_PROCESSORS_OPENCVPREPROCESSOR_H_

#include <memory>
#include "opencv2/opencv.hpp"
#include "../Base/base.h"
#include "include/arctern.h"
#include "common.h"


namespace arctern {

class OpencvPreprocessorInitPt {
 public:
  int net_h_;
  int net_w_;
  float mean_;
  float std_;
  int border_value;
  std::vector<float> meanV_;
  std::vector<float> stdV_;
 public:
  AlignFace alignFace_;
  std::vector<ArcternPoint>
      lmk_; ///< standard point , now use 2 points. left eye and right eye

 public:
  AlignMent alignMent_;
  DetType detType_;
};

class OpencvPreprocessorRunRt {
 public:
  OpencvPreprocessorRunRt();
  ~OpencvPreprocessorRunRt();

 public:
  cv::Mat image_;
  std::vector<arctern::ArcternPoint> lmk_;
  int cvt_type_;
  arctern::ArcternRect roi_;
  std::vector<float> padding_param_;
  std::vector<OperatorType> sequence_;
};

class OpencvPreprocessorRet {
 public:
  cv::Mat mat_;
};

class OpencvPreprocessor final {
 public:
  OpencvPreprocessor();

  ~OpencvPreprocessor() = default;

  ErrorType Init(OpencvPreprocessorInitPt *p);

  std::unique_ptr<OpencvPreprocessorRet> Process(OpencvPreprocessorRunRt *p);

 public:
  inline int GetLastError() const {
    return m_lastErrorCode;
  }

 private:
 public:
  int calcMatrix( OpencvPreprocessorRunRt *pRunParam, double dArray[2][3] );

  int cvtColor(OpencvPreprocessorRunRt *pRunParam);
  int crop(OpencvPreprocessorRunRt *pRunParam);
  int resize(OpencvPreprocessorRunRt * pRunParam);
  int affine(OpencvPreprocessorRunRt *pRunParam);
  int normalize(OpencvPreprocessorRunRt *pRunParam);
  int padding(OpencvPreprocessorRunRt *pRunParam);
  int extractChannel(OpencvPreprocessorRunRt *pRunParam);

  int detFace(OpencvPreprocessorRunRt *pRunParam);
  int merge(OpencvPreprocessorRunRt *pRunParam);
  int splitNormalized(OpencvPreprocessorRunRt *pRunParam);
  int twiceResize(OpencvPreprocessorRunRt *pRunParam);
  int multiCat(OpencvPreprocessorRunRt *pRunParam);
  int faceJoy(OpencvPreprocessorRunRt *pRunParam);
  int faceExp(OpencvPreprocessorRunRt *pRunParam);
  int detFace2(OpencvPreprocessorRunRt *pRunParam);
  int faceFeat(OpencvPreprocessorRunRt *pRunParam);
  int faceWipe(OpencvPreprocessorRunRt *pRunParam);

 private:
  int m_lastErrorCode;
  OpencvPreprocessorInitPt init_pt_;
};

}

#endif //ARCTERN_BASE_SRC_PROCESSORS_OPENCVPREPROCESSOR_H_

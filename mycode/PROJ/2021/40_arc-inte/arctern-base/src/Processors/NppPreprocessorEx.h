//
// Created by Admin on 2020/6/7.
//

#ifndef ARCTERN_BASE_NPPPREPROCESSOREX_H
#define ARCTERN_BASE_NPPPREPROCESSOREX_H
#include "src/ImgPreProcess/ImgPreprocessorOnNppEx.h"
#include "alignFace.h"
#include "cuda.h"
#include "cuda_runtime.h"
#include <memory>
#include "src/common/testUtil.h"
#include "src/ImgPreProcess/buffers.h"
#include<iomanip>
#include "common.h"
using namespace std;
using namespace ImgPreprocessEx;

namespace arctern {

class NppPreprocessorInitPt  {
 public:
  NppPreprocessorInitPt() = default;
//  NppPreprocessorInitPt &operator=(const NppPreprocessorInitPt &initParam);
  ~NppPreprocessorInitPt() = default;

 public:
  int m_gpu_id = 0;
  AlignFace alignFace_;
  std::vector<ArcternPoint>      lmk_; ///< standard point , now use 2 points. left eye and right eye
  AlignMent alignMent_;
  DetType detType_;
  int borderValue_;
  int netH_{};
  int netW_{};
  float mean_{};
  float std_{};
  std::vector<float> meanV_;
  std::vector<float> stdV_;

};





class NppPreprocessorRunPt{
 public:

  NppPreprocessorRunPt();
  ~NppPreprocessorRunPt();

  unique_ptr<DeviceBuffer> arcternImage;
  vector<ArcternPoint> m_lmk;
  int m_cvtColorCode = -1;
  ArcternRect m_roi;
  vector<float> m_paddingParam;

  std::vector<OperatorType> sequence_;

};

class NppPreprocessorRet/*: public PreProResult*/{
 public:
  NppPreprocessorRet();
  ~NppPreprocessorRet();

  std::unique_ptr<DeviceBuffer> arcternImage ;
};
#define OUTPUT
template <typename T>
inline void printDeviceBuffer (ImgPreprocessEx::DeviceBuffer & buffer,string outToFile="") {
#ifdef OUTPUT
  // T == float
  int type = CV_8U;
  if(sizeof(T) == 4 ) {
    type = CV_32F;
  }
  cv::Mat mat(buffer.Height(), buffer.Width(), CV_MAKETYPE(type, buffer.Channel()));
  cudaMemcpy(mat.data, buffer.data(),
             buffer.nbBytes(),
             cudaMemcpyDeviceToHost);
  if(outToFile.size() > 0){
    printfImg<T>(mat,0,0,outToFile);
  }
  else{
    printfImg<T>(mat);
  }

#endif //OUTPUT
}

template<typename T>
class Mat2Array {
 public:
  Mat2Array();

  virtual ~ Mat2Array();

  void init(const cv::Mat &mat);

  T **Transform(const cv::Mat &mat);

  template<int rowSize, int colSize>
  void Transform(const cv::Mat &mat, T array[rowSize][colSize]);

 private:
  int cols;
  int rows;
  int channels;
  int step;
  T **t;
};

class NppPreprocessor  {
 public:
  NppPreprocessor(cudaStream_t *pStream);

  ~NppPreprocessor() = default;

  virtual ErrorType Init(NppPreprocessorInitPt *p);

  virtual std::unique_ptr<NppPreprocessorRet> Process( NppPreprocessorRunPt *p);

 public:
  inline int GetLastError() const {
    return m_lastErrorCode;
  }

 public:
  int calcMatrix( NppPreprocessorRunPt *pRunParam, double dArray[2][3] );

  int cvtColor(NppPreprocessorRunPt *pRunParam);
  int crop(NppPreprocessorRunPt *pRunParam);
  int resize(NppPreprocessorRunPt * pRunParam);
  int affine(NppPreprocessorRunPt *pRunParam);
  int normalize(NppPreprocessorRunPt *pRunParam);
  int padding(NppPreprocessorRunPt *pRunParam);
  int extractChannel(NppPreprocessorRunPt *pRunParam);

  int detFace(NppPreprocessorRunPt *pRunParam);
  int merge(NppPreprocessorRunPt *pRunParam);
  int splitNormalized(NppPreprocessorRunPt *pRunParam);
  int twiceResize(NppPreprocessorRunPt *pRunParam);
  int multiCat(NppPreprocessorRunPt *pRunParam);
  int faceJoy(NppPreprocessorRunPt *pRunParam);
  int faceExp(NppPreprocessorRunPt *pRunParam);
  int detFace2(NppPreprocessorRunPt *pRunParam);
  int faceFeat(NppPreprocessorRunPt *pRunParam);
 private:
  unique_ptr<NppPreprocessorInitPt> m_initPt;
  ImgPreprocessorOnNppEx m_nppProcEx;
  int m_lastErrorCode;

};

}  // namespace arctern
#endif //ARCTERN_BASE_NPPPREPROCESSOREX_H

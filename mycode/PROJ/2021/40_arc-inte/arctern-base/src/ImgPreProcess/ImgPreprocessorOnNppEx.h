//
// Created by Admin on 2020/6/7.
//

#ifndef ARCTERN_BASE_IMGPREPROCESSORONNPPEX_H
#define ARCTERN_BASE_IMGPREPROCESSORONNPPEX_H
#include "include/arctern.h"
#include "Base/base.h"
#include "buffers.h"
#include "npp.h"
using namespace arctern;
namespace ImgPreprocessEx {
extern int GetImageType(const int& channel,const int& depth);
class ImgPreprocessorOnNppEx {
 public:
  explicit ImgPreprocessorOnNppEx(cudaStream_t *pStream = nullptr);

  virtual ~ImgPreprocessorOnNppEx();

  virtual int Resize(const DeviceBuffer &srcImg,
                     DeviceBuffer &dstImg,
                     const ArcternInterpolation &interpolation,
                     double fy = 0,double fx = 0);

  virtual int Crop(const DeviceBuffer &srcImg, DeviceBuffer &dstImg, ArcternRect rROI);

  virtual int
  Normalized(const DeviceBuffer &srcImg, DeviceBuffer &dstImg, const float &alpha, const float &beta);

  virtual int
  Normalized(const DeviceBuffer &srcImg, DeviceBuffer &dstImg, const float4 &alpha, const float4 &beta);

  virtual int Padding(const DeviceBuffer &srcImg, DeviceBuffer &dstImg,
                      const int &topBorderHeight, const int &leftBorderWidth,
                      const int borderColors[4]);

  virtual int Affine(const DeviceBuffer &srcImg, DeviceBuffer &dstImg,  double transform[2][3],
                     const ArcternInterpolation &interpolation);

  virtual int
  CvtColor(const DeviceBuffer &srcImg, DeviceBuffer &dstImg, const ColorCvtType &code);

  virtual int ExtractChannel(const DeviceBuffer &srcImg, DeviceBuffer &dstImg);

  virtual int Merge(const DeviceBuffer &srcImg, DeviceBuffer &dstImg,int num=3);

  virtual int MergeChannel(const vector<DeviceBuffer> &vSrcImg,DeviceBuffer &dstImg);

  virtual int GetInterpolation(const ArcternInterpolation &interpolation);
 protected:
  int GetLastError();

 private:
  cudaStream_t *m_pStream = nullptr;
  int m_lastError = 0;
};
}
#endif //ARCTERN_BASE_IMGPREPROCESSORONNPPEX_H

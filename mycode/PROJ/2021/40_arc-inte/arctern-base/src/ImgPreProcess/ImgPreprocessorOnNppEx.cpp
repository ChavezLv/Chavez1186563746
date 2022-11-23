//
// Created by Admin on 2020/6/7.
//

#include <opencv2/core/cuda.hpp>
#include <common/testUtil.h>

#include "ImgPreprocessorOnNppEx.h"
#include "spdlog/spdlog.h"
#include "cuda.h"
#include "cuda_runtime.h"
#include "npp.h"
#include "nppi.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
#include "resize.h"
#include "normalized.h"
#include "cvtColor.h"
#include "crop.h"
#include "affine.h"
#include "padding.h"


namespace ImgPreprocessEx {

#define CHECK(status)                             \
    do                                            \
    {                                             \
        int ret = status;                         \
        if (ret != 0)                             \
        {                                         \
            SPDLOG_ERROR("cuda error code: {}",ret);    \
        }                                         \
    } while (0)

#define INJECTION(st) #st

#define START(op)                                                                   \
    if(srcImg.data() == nullptr) {                                                   \
      m_lastError = -1;                                                             \
    }                                                                               \
    if(m_lastError != 0) return m_lastError;                                        \
    NppiSize srcSize{static_cast<int>(srcImg.Width()), static_cast<int>(srcImg.Height())};                              \
    NppiRect srcRect{0, 0, srcSize.width, srcSize.height};                          \
    NppiSize dstSize{static_cast<int>(dstImg.Width()) ,static_cast<int>(dstImg.Height())};                             \
    NppiRect dstRect{0, 0, dstSize.width, dstSize.height};                          \
    int srcStep = srcSize.width * srcImg.Channel() * srcImg.getElementSize();       \
    int dstStep = dstSize.width * dstImg.Channel() * dstImg.getElementSize();

#define END(op)         \
    return m_lastError;


ImgPreprocessorOnNppEx::ImgPreprocessorOnNppEx(cudaStream_t *pStream):m_pStream(pStream) {
}
ImgPreprocessorOnNppEx::~ImgPreprocessorOnNppEx() {
}

int ImgPreprocessorOnNppEx::GetLastError() {
  return m_lastError;
}
int ImgPreprocessorOnNppEx::GetInterpolation(const ArcternInterpolation &interpolation) {
  switch (interpolation) {
    case LINEAR:return NppiInterpolationMode::NPPI_INTER_LINEAR;
    case CUBIC:return NppiInterpolationMode::NPPI_INTER_CUBIC;
  }
  return NppiInterpolationMode::NPPI_INTER_LINEAR;
}

int ImgPreprocessorOnNppEx::Resize(const DeviceBuffer &srcImg,
                                   DeviceBuffer &dstImg,
                                   const ArcternInterpolation &interpolation,
                                   double fy,double fx) {
  arctern::cuda::resize(srcImg,dstImg,fy,fx,(int) interpolation,m_pStream);
  return m_lastError;
}

int ImgPreprocessorOnNppEx::Crop(const DeviceBuffer &srcImg, DeviceBuffer &dstImg, ArcternRect rROI) {

  arctern::cuda::crop(srcImg,dstImg,rROI,m_pStream);
  return m_lastError;
}

int ImgPreprocessorOnNppEx::Normalized(const DeviceBuffer &srcImg,
                                       DeviceBuffer &dstImg,
                                       const float &alpha,
                                       const float &beta) {

  float4 constant{alpha, alpha, alpha,alpha};
  float4 addConstants{beta, beta, beta};
  arctern::cuda::normalized(srcImg,dstImg,constant,addConstants,m_pStream);
  return m_lastError;
}

int ImgPreprocessorOnNppEx::Normalized(const DeviceBuffer &srcImg,
                                       DeviceBuffer &dstImg,
                                       const float4 &alpha,
                                       const float4 &beta) {
  arctern::cuda::normalized(srcImg,dstImg,alpha,beta,m_pStream);
  return m_lastError;
}

int ImgPreprocessorOnNppEx::Padding(const DeviceBuffer &srcImg, DeviceBuffer &dstImg,
                                    const int &topBorderHeight, const int &leftBorderWidth,
                                    const int borderColors[4]) {
  float3 borderValue{(float)borderColors[0],(float)borderColors[1],(float)borderColors[2]};
  arctern::cuda::padding(srcImg,dstImg,topBorderHeight,leftBorderWidth,borderValue,m_pStream);

  return m_lastError;
}

int ImgPreprocessorOnNppEx::Affine(const DeviceBuffer &srcImg, DeviceBuffer &dstImg,
                                    double transform[2][3],
                                   const ArcternInterpolation &interpolation) {
  arctern::cuda::Affine(srcImg,dstImg,transform,m_pStream);
  return m_lastError;
}

int ImgPreprocessorOnNppEx::CvtColor(const DeviceBuffer &srcImg, DeviceBuffer &dstImg, const ColorCvtType &code) {
  switch (code) {
    case RGB2GRAY:
      arctern::cuda::cvtRGB2GRAY(srcImg,dstImg,m_pStream);
      break;
    case BGR2GRAY:
      arctern::cuda::cvtBGR2GRAY(srcImg,dstImg,m_pStream);
      break;
    case BGR2RGB:
      arctern::cuda::cvtBGR2RGB(srcImg,dstImg,m_pStream);
      break;
    case RGB2BGR:
      arctern::cuda::cvtRGB2BGR(srcImg,dstImg,m_pStream);
      break;
    default: break;
  }
  return m_lastError;
}

int ImgPreprocessorOnNppEx::ExtractChannel(const DeviceBuffer &srcImg, DeviceBuffer &dstImg){
  arctern::cuda::ExtractChannel(srcImg,dstImg,m_pStream);
  return m_lastError;
}

int ImgPreprocessorOnNppEx::Merge(const DeviceBuffer &srcImg,DeviceBuffer &dstImg,int num){
  unsigned long offset = 0;
  for(int i=0;i<num;i++){
    if(srcImg.data() == nullptr) {
      m_lastError = -1;
      return m_lastError;
    }
    offset = i * srcImg.nbBytes();
    Npp8u * dstData = static_cast<Npp8u*>(dstImg.data()) + offset;
    CHECK( cudaMemcpyAsync(static_cast<void*>(dstData),
                      srcImg.data(),
                      srcImg.nbBytes(),
                      cudaMemcpyDeviceToDevice,*m_pStream));
  }
  return m_lastError;
}

int ImgPreprocessorOnNppEx::MergeChannel(const vector<DeviceBuffer> &vSrcImg,DeviceBuffer &dstImg){

  return m_lastError;
}

}
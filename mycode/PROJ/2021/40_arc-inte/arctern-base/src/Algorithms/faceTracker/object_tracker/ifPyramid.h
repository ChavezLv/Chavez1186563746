/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_IFPYRAMID_H_
#define SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_IFPYRAMID_H_
//! smooths and downsamples the image
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/types_c.h"

#if defined __SSE2__ || defined _M_X64  || (defined _M_IX86_FP && _M_IX86_FP >= 2)
#  include "emmintrin.h"
#  define IFCV_SSE 1
#  define IFCV_SSE2 1
#endif

/*********************************** CPU capabilities ***********************************/
#define IFCV_CPU_NONE    0
#define IFCV_CPU_MMX     1
#define IFCV_CPU_SSE     2
#define IFCV_CPU_SSE2    3
#define IFCV_CPU_SSE3    4
#define IFCV_CPU_SSSE3   5
#define IFCV_CPU_SSE4_1  6
#define IFCV_CPU_SSE4_2  7
#define IFCV_CPU_POPCNT  8
#define IFCV_CPU_AVX    10
#define IFCV_HARDWARE_MAX_FEATURE 255

#define IFCV_CN_MAX     512
#define IFCV_CN_SHIFT   3
#define IFCV_DEPTH_MAX  (1 << IFCV_CN_SHIFT)

#define IFCV_MAT_CN_MASK          ((IFCV_CN_MAX - 1) << IFCV_CN_SHIFT)
#define IFCV_MAT_CN(flags)        ((((flags) & IFCV_MAT_CN_MASK) >> IFCV_CN_SHIFT) + 1)

#define IFCV_8U   0
#define IFCV_8S   1
#define IFCV_16U  2
#define IFCV_16S  3
#define IFCV_32S  4
#define IFCV_32F  5
#define IFCV_64F  6
#define IFCV_USRTYPE1 7

#define IFCV_MAT_DEPTH_MASK       (IFCV_DEPTH_MAX - 1)
#define IFCV_MAT_DEPTH(flags)     ((flags) & IFCV_MAT_DEPTH_MASK)

#define IFCV_MAKETYPE(depth, cn) (IFCV_MAT_DEPTH(depth) + (((cn)-1) << IFCV_CN_SHIFT))
#define IFCV_MAKE_TYPE IFCV_MAKETYPE

#define IFCV_8UC1 CV_MAKETYPE(IFCV_8U, 1)
#define IFCV_8UC2 CV_MAKETYPE(IFCV_8U, 2)
#define IFCV_8UC3 CV_MAKETYPE(IFCV_8U, 3)
#define IFCV_8UC4 CV_MAKETYPE(IFCV_8U, 4)
#define IFCV_8UC(n) CV_MAKETYPE(IFCV_8U, (n))

#define  IFCV_DESCALE(x, n)     (((x) + (1 << ((n)-1))) >> (n))
#ifdef __GNUC__
#  define IFCV_DECL_ALIGNED(x) __attribute__ ((aligned (x)))
#elif defined _MSC_VER
#  define IFCV_DECL_ALIGNED(x) __declspec(align(x))
#else
#  define IFCV_DECL_ALIGNED(x)
#endif

#define IFIPL_BORDER_CONSTANT   0
#define IFIPL_BORDER_REPLICATE  1
#define IFIPL_BORDER_REFLECT    2
#define IFIPL_BORDER_WRAP       3
#define IFIPL_BORDER_REFLECT_101    4
#define IFIPL_BORDER_TRANSPARENT    5

bool ifcheckHardwareSupport(int feature);

namespace ifcv {
//! various border interpolation methods
enum {
  IFBORDER_REPLICATE = IFIPL_BORDER_REPLICATE, IFBORDER_CONSTANT = IFIPL_BORDER_CONSTANT,
  IFBORDER_REFLECT = IFIPL_BORDER_REFLECT, IFBORDER_WRAP = IFIPL_BORDER_WRAP,
  IFBORDER_REFLECT_101 = IFIPL_BORDER_REFLECT_101, IFBORDER_REFLECT101 = IFBORDER_REFLECT_101,
  IFBORDER_TRANSPARENT = IFIPL_BORDER_TRANSPARENT,
  IFBORDER_DEFAULT = IFBORDER_REFLECT_101, IFBORDER_ISOLATED = 16
};

int ifcvFloor(double value);

int ifcvRound(double value);

typedef void(*ifPyrFunc)(const cv::Mat &, cv::Mat &, int);

void ifpyrDown(cv::InputArray src,
               cv::OutputArray dst,
               const cv::Size &dstsize = cv::Size(),
               int borderType = IFBORDER_DEFAULT);

void ifscalarToRawData(const cv::Scalar &s, void *_buf, int type, int unroll_to);

//! 1D interpolation function: returns coordinate of the "donor" pixel for the specified location p.
int ifborderInterpolate(int p, int len, int borderType);

void ifcopyMakeBorder(cv::InputArray src,
                      cv::OutputArray dst,
                      int top,
                      int bottom,
                      int left,
                      int right,
                      int borderType,
                      const cv::Scalar &value = cv::Scalar());

}  // namespace ifcv
#endif  // SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_IFPYRAMID_H_

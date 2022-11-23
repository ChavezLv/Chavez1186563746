/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include "src/Algorithms/faceTracker/object_tracker/lkpyramidPC.h"

// #include "opencv2/core/internal.hpp"
#include <float.h>
#include <vector>
#include <algorithm>
#include "trackingPC.h"  // @zzf_added
#include "ifPyramid.h"
#include "timeEvaluate.h"

// @zzf no name namespace can only be used in the file where it is defined.
namespace {
static void calcSharrDeriv(const cv::Mat &src, cv::Mat &dst) {
  using ifcv::detail::deriv_type;
  int rows = src.rows, cols = src.cols, cn = src.channels(), colsn = cols * cn, depth = src.depth();
  CV_Assert(depth == CV_8U);
  dst.create(rows, cols, CV_MAKETYPE(cv::DataType<deriv_type>::depth, cn * 2));

#ifdef HAVE_TEGRA_OPTIMIZATION
  if (tegra::calcSharrDeriv(src, dst))
      return;
#endif

  int x, y, delta = cv::alignSize((cols + 2) * cn, 16);
  cv::AutoBuffer<deriv_type> _tempBuf(delta * 2 + 64);
  deriv_type *trow0 = cv::alignPtr(_tempBuf + cn, 16), *trow1 = cv::alignPtr(trow0 + delta, 16);

#if IFCV_SSE2
  __m128i z = _mm_setzero_si128(), c3 = _mm_set1_epi16(3), c10 = _mm_set1_epi16(10);
#endif

  for (y = 0; y < rows; y++) {
    const uchar *srow0 = src.ptr<uchar>(y > 0 ? y - 1 : rows > 1 ? 1 : 0);
    const uchar *srow1 = src.ptr<uchar>(y);
    const uchar *srow2 = src.ptr<uchar>(y < rows - 1 ? y + 1 : rows > 1 ? rows - 2 : 0);
    deriv_type *drow = dst.ptr<deriv_type>(y);

    // do vertical convolution
    x = 0;
#if IFCV_SSE2
    for (; x <= colsn - 8; x += 8) {
      __m128i s0 = _mm_unpacklo_epi8(_mm_loadl_epi64((const __m128i *) (srow0 + x)), z);
      __m128i s1 = _mm_unpacklo_epi8(_mm_loadl_epi64((const __m128i *) (srow1 + x)), z);
      __m128i s2 = _mm_unpacklo_epi8(_mm_loadl_epi64((const __m128i *) (srow2 + x)), z);
      __m128i t0 = _mm_add_epi16(_mm_mullo_epi16(_mm_add_epi16(s0, s2), c3), _mm_mullo_epi16(s1, c10));
      __m128i t1 = _mm_sub_epi16(s2, s0);
      _mm_store_si128((__m128i *) (trow0 + x), t0);  // NOLINT
      _mm_store_si128((__m128i *) (trow1 + x), t1);  // NOLINT
    }
#endif
    for (; x < colsn; x++) {
      int t0 = (srow0[x] + srow2[x]) * 3 + srow1[x] * 10;
      int t1 = srow2[x] - srow0[x];
      trow0[x] = (deriv_type) t0;
      trow1[x] = (deriv_type) t1;
    }

    // make border
    int x0 = (cols > 1 ? 1 : 0) * cn, x1 = (cols > 1 ? cols - 2 : 0) * cn;
    for (int k = 0; k < cn; k++) {
      trow0[-cn + k] = trow0[x0 + k];
      trow0[colsn + k] = trow0[x1 + k];
      trow1[-cn + k] = trow1[x0 + k];
      trow1[colsn + k] = trow1[x1 + k];
    }

    // do horizontal convolution, interleave the results and store them to dst
    x = 0;
#if IFCV_SSE2
    for (; x <= colsn - 8; x += 8) {
      __m128i s0 = _mm_loadu_si128((const __m128i *) (trow0 + x - cn));
      __m128i s1 = _mm_loadu_si128((const __m128i *) (trow0 + x + cn));
      __m128i s2 = _mm_loadu_si128((const __m128i *) (trow1 + x - cn));
      __m128i s3 = _mm_load_si128((const __m128i *) (trow1 + x));
      __m128i s4 = _mm_loadu_si128((const __m128i *) (trow1 + x + cn));

      __m128i t0 = _mm_sub_epi16(s1, s0);
      __m128i t1 = _mm_add_epi16(_mm_mullo_epi16(_mm_add_epi16(s2, s4), c3), _mm_mullo_epi16(s3, c10));
      __m128i t2 = _mm_unpacklo_epi16(t0, t1);
      t0 = _mm_unpackhi_epi16(t0, t1);
      // this can probably be replaced with aligned stores if we aligned dst properly.
      _mm_storeu_si128((__m128i *) (drow + x * 2), t2);  // NOLINT
      _mm_storeu_si128((__m128i *) (drow + x * 2 + 8), t0);  // NOLINT
    }
#endif
    for (; x < colsn; x++) {
      deriv_type t0 = (deriv_type) (trow0[x + cn] - trow0[x - cn]);
      deriv_type t1 = (deriv_type) ((trow1[x + cn] + trow1[x - cn]) * 3 + trow1[x] * 10);
      drow[x * 2] = t0;
      drow[x * 2 + 1] = t1;
    }
  }
}

}  // namespace

ifcv::detail::LKTrackerInvoker::LKTrackerInvoker(
    const cv::Mat &_prevImg, const cv::Mat &_prevDeriv, const cv::Mat &_nextImg,
    const cv::Point2f *_prevPts, cv::Point2f *_nextPts,
    uchar *_status, float *_err,
    cv::Size _winSize, cv::TermCriteria _criteria,
    int _level, int _maxLevel, int _flags, float _minEigThreshold) {
  prevImg = &_prevImg;
  prevDeriv = &_prevDeriv;
  nextImg = &_nextImg;
  prevPts = _prevPts;
  nextPts = _nextPts;
  status = _status;
  err = _err;
  winSize = _winSize;
  criteria = _criteria;
  level = _level;
  maxLevel = _maxLevel;
  flags = _flags;
  minEigThreshold = _minEigThreshold;
}

void ifcv::detail::LKTrackerInvoker::runLK(int nPoints) {   // @zzf added
  cv::Point2f halfWin((winSize.width - 1) * 0.5f, (winSize.height - 1) * 0.5f);
  const cv::Mat &I = *prevImg;
  const cv::Mat &J = *nextImg;
  const cv::Mat &derivI = *prevDeriv;

  int j, cn = I.channels(), cn2 = cn * 2;
  cv::AutoBuffer<ifcv::detail::deriv_type> _buf(winSize.area() * (cn + cn2));
  int derivDepth = cv::DataType<ifcv::detail::deriv_type>::depth;

  // @zzf use to store I and dI in prevImg for iteration
  cv::Mat IWinBuf(winSize, IFCV_MAKETYPE(derivDepth, cn), (ifcv::detail::deriv_type *) _buf);
  cv::Mat
      derivIWinBuf(winSize, IFCV_MAKETYPE(derivDepth, cn2), (ifcv::detail::deriv_type *) _buf + winSize.area() * cn);

  for (int ptidx = 0; ptidx < nPoints; ptidx++) {
    cv::Point2f prevPt = prevPts[ptidx] * static_cast<float>(1. / (1 << level));
    cv::Point2f nextPt;
    if (level == maxLevel) {
      nextPt = nextPts[ptidx] * static_cast<float>(1. / (1 << level));
      // if (flags & OPTFLOW_USE_INITIAL_FLOW)
      //  nextPt = nextPts[ptidx] * (float)(1. / (1 << level));
      // else
      //             nextPt = prevPt;
    } else {
      nextPt = nextPts[ptidx] * 2.f;
    }
    nextPts[ptidx] = nextPt;

    cv::Point2i iprevPt, inextPt;
    prevPt -= halfWin;
    iprevPt.x = ifcvFloor(prevPt.x);
    iprevPt.y = ifcvFloor(prevPt.y);

    if (iprevPt.x < -winSize.width || iprevPt.x >= derivI.cols ||
        iprevPt.y < -winSize.height || iprevPt.y >= derivI.rows) {
      if (level == 0) {
        if (status)
          status[ptidx] = false;
        if (err)
          err[ptidx] = 0;
      }
      continue;
    }

    float a = prevPt.x - iprevPt.x;
    float b = prevPt.y - iprevPt.y;
    const int W_BITS = 14, W_BITS1 = 14;
    const float FLT_SCALE = 1.f / (1 << 20);
    int iw00 = ifcvRound((1.f - a) * (1.f - b) * (1 << W_BITS));
    int iw01 = ifcvRound(a * (1.f - b) * (1 << W_BITS));
    int iw10 = ifcvRound((1.f - a) * b * (1 << W_BITS));
    int iw11 = (1 << W_BITS) - iw00 - iw01 - iw10;

    // @zzf for common images: step:bytes that one row of image contains;
    // elemSize1:bytes that one channel of a pixel contains
    int dstep = static_cast<int>(derivI.step
        / derivI.elemSize1());
    int stepI = static_cast<int>(I.step / I.elemSize1());
    int stepJ = static_cast<int>(J.step / J.elemSize1());
    float A11 = 0, A12 = 0, A22 = 0;

#if IFCV_SSE2
    __m128i qw0 = _mm_set1_epi32(iw00 + (iw01 << 16));
    __m128i qw1 = _mm_set1_epi32(iw10 + (iw11 << 16));
    __m128i z = _mm_setzero_si128();
    __m128i qdelta_d = _mm_set1_epi32(1 << (W_BITS1 - 1));
    __m128i qdelta = _mm_set1_epi32(1 << (W_BITS1 - 5 - 1));
    __m128 qA11 = _mm_setzero_ps(), qA12 = _mm_setzero_ps(), qA22 = _mm_setzero_ps();
#endif

    // extract the patch from the first image, compute covariation matrix of derivatives
    int x, y;
    for (y = 0; y < winSize.height; y++) {
      const uchar *src = (const uchar *) I.data + (y + iprevPt.y) * stepI + iprevPt.x * cn;
      const deriv_type *dsrc = (const deriv_type *) derivI.data + (y + iprevPt.y) * dstep + iprevPt.x * cn2;

      deriv_type *Iptr = reinterpret_cast<deriv_type *>(IWinBuf.data + y * IWinBuf.step);
      deriv_type *dIptr = reinterpret_cast<deriv_type *>(derivIWinBuf.data + y * derivIWinBuf.step);

      x = 0;

#if IFCV_SSE2
      for (; x <= winSize.width * cn - 4; x += 4, dsrc += 4 * 2, dIptr += 4 * 2) {
        __m128i v00, v01, v10, v11, t0, t1;

        v00 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(const int *) (src + x)), z);
        v01 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(const int *) (src + x + cn)), z);
        v10 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(const int *) (src + x + stepI)), z);
        v11 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(const int *) (src + x + stepI + cn)), z);

        t0 = _mm_add_epi32(_mm_madd_epi16(_mm_unpacklo_epi16(v00, v01), qw0),
                           _mm_madd_epi16(_mm_unpacklo_epi16(v10, v11), qw1));
        t0 = _mm_srai_epi32(_mm_add_epi32(t0, qdelta), W_BITS1 - 5);
        _mm_storel_epi64((__m128i *) (Iptr + x), _mm_packs_epi32(t0, t0));   // NOLINT

        v00 = _mm_loadu_si128((const __m128i *) (dsrc));
        v01 = _mm_loadu_si128((const __m128i *) (dsrc + cn2));
        v10 = _mm_loadu_si128((const __m128i *) (dsrc + dstep));
        v11 = _mm_loadu_si128((const __m128i *) (dsrc + dstep + cn2));

        t0 = _mm_add_epi32(_mm_madd_epi16(_mm_unpacklo_epi16(v00, v01), qw0),
                           _mm_madd_epi16(_mm_unpacklo_epi16(v10, v11), qw1));
        t1 = _mm_add_epi32(_mm_madd_epi16(_mm_unpackhi_epi16(v00, v01), qw0),
                           _mm_madd_epi16(_mm_unpackhi_epi16(v10, v11), qw1));
        t0 = _mm_srai_epi32(_mm_add_epi32(t0, qdelta_d), W_BITS1);
        t1 = _mm_srai_epi32(_mm_add_epi32(t1, qdelta_d), W_BITS1);
        v00 = _mm_packs_epi32(t0, t1);  // Ix0 Iy0 Ix1 Iy1 ...   NOLINT

        _mm_storeu_si128((__m128i *) dIptr, v00);   // NOLINT
        t0 = _mm_srai_epi32(v00, 16);  // Iy0 Iy1 Iy2 Iy3
        t1 = _mm_srai_epi32(_mm_slli_epi32(v00, 16), 16);  // Ix0 Ix1 Ix2 Ix3

        __m128 fy = _mm_cvtepi32_ps(t0);
        __m128 fx = _mm_cvtepi32_ps(t1);

        qA22 = _mm_add_ps(qA22, _mm_mul_ps(fy, fy));
        qA12 = _mm_add_ps(qA12, _mm_mul_ps(fx, fy));
        qA11 = _mm_add_ps(qA11, _mm_mul_ps(fx, fx));
      }
#endif

      for (; x < winSize.width * cn; x++, dsrc += 2, dIptr += 2) {
        // @zzf iw00,iw01,iw10,iw11 is used for bi-linear interpolation
        int ival = IFCV_DESCALE(src[x] * iw00 + src[x + cn] * iw01 +
            src[x + stepI] * iw10 + src[x + stepI + cn] * iw11, W_BITS1 - 5);
        int ixval = IFCV_DESCALE(dsrc[0] * iw00 + dsrc[cn2] * iw01 +
            dsrc[dstep] * iw10 + dsrc[dstep + cn2] * iw11, W_BITS1);
        int iyval = IFCV_DESCALE(dsrc[1] * iw00 + dsrc[cn2 + 1] * iw01 + dsrc[dstep + 1] * iw10 +
            dsrc[dstep + cn2 + 1] * iw11, W_BITS1);

        Iptr[x] = static_cast<short > (ival);   // NOLINT
        dIptr[0] = static_cast<short > (ixval);  // NOLINT
        dIptr[1] = static_cast<short > (iyval);  // NOLINT

        A11 += static_cast<float> (ixval * ixval);
        A12 += static_cast<float> (ixval * iyval);
        A22 += static_cast<float> (iyval * iyval);
      }
    }

#if IFCV_SSE2
    float IFCV_DECL_ALIGNED(16) A11buf[4], A12buf[4], A22buf[4];
    _mm_store_ps(A11buf, qA11);
    _mm_store_ps(A12buf, qA12);
    _mm_store_ps(A22buf, qA22);
    A11 += A11buf[0] + A11buf[1] + A11buf[2] + A11buf[3];
    A12 += A12buf[0] + A12buf[1] + A12buf[2] + A12buf[3];
    A22 += A22buf[0] + A22buf[1] + A22buf[2] + A22buf[3];
#endif

    A11 *= FLT_SCALE;
    A12 *= FLT_SCALE;
    A22 *= FLT_SCALE;

    // RInfo_txt << "point index:" << ptidx << std::endl;
    // RInfo_txt << "G:(G11,G12,G21,G22)=" << A11 << "," << A12 << "," << A12 << "," << A22 << std::endl;
    // @zzf D is determinant of matrix A;
    // @zzf minEig is min of two eigen values of matrix A devide the area of window, easy to deduce;
    float D = A11 * A22 - A12 * A12;
    float minEig = (A22 + A11 - std::sqrt((A11 - A22) * (A11 - A22) +
        4.f * A12 * A12)) / (2 * winSize.width * winSize.height);

    if (err && (flags & IFCV_LKFLOW_GET_MIN_EIGENVALS) != 0)
      err[ptidx] =  minEig;

    if (minEig < minEigThreshold || D < FLT_EPSILON) {
      if (level == 0 && status)
        status[ptidx] = false;
      continue;
    }

    D = 1.f / D;

    nextPt -= halfWin;  // @zzf the init point is on the middle of window, need to move to left top of the window
    cv::Point2f prevDelta;

    for (j = 0; j < criteria.maxCount; j++) {
      inextPt.x = ifcvFloor(nextPt.x);
      inextPt.y = ifcvFloor(nextPt.y);

      if (inextPt.x < -winSize.width || inextPt.x >= J.cols ||
          inextPt.y < -winSize.height || inextPt.y >= J.rows) {
        if (level == 0 && status)
          status[ptidx] = false;
        break;
      }

      a = nextPt.x - inextPt.x;
      b = nextPt.y - inextPt.y;
      iw00 = ifcvRound((1.f - a) * (1.f - b) * (1 << W_BITS));
      iw01 = ifcvRound(a * (1.f - b) * (1 << W_BITS));
      iw10 = ifcvRound((1.f - a) * b * (1 << W_BITS));
      iw11 = (1 << W_BITS) - iw00 - iw01 - iw10;
      float b1 = 0, b2 = 0;
#if IFCV_SSE2
      qw0 = _mm_set1_epi32(iw00 + (iw01 << 16));
      qw1 = _mm_set1_epi32(iw10 + (iw11 << 16));
      __m128 qb0 = _mm_setzero_ps(), qb1 = _mm_setzero_ps();
#endif

      for (y = 0; y < winSize.height; y++) {
        const uchar *Jptr = (const uchar *) J.data + (y + inextPt.y) * stepJ + inextPt.x * cn;
        const deriv_type *Iptr = (const deriv_type *) (IWinBuf.data + y * IWinBuf.step);
        const deriv_type *dIptr = (const deriv_type *) (derivIWinBuf.data + y * derivIWinBuf.step);

        x = 0;

#if IFCV_SSE2
        for (; x <= winSize.width * cn - 8; x += 8, dIptr += 8 * 2) {
          __m128i diff0 = _mm_loadu_si128((const __m128i *) (Iptr + x)), diff1;
          __m128i v00 = _mm_unpacklo_epi8(_mm_loadl_epi64((const __m128i *) (Jptr + x)), z);
          __m128i v01 = _mm_unpacklo_epi8(_mm_loadl_epi64((const __m128i *) (Jptr + x + cn)), z);
          __m128i v10 = _mm_unpacklo_epi8(_mm_loadl_epi64((const __m128i *) (Jptr + x + stepJ)), z);
          __m128i v11 = _mm_unpacklo_epi8(_mm_loadl_epi64((const __m128i *) (Jptr + x + stepJ + cn)), z);

          __m128i t0 = _mm_add_epi32(_mm_madd_epi16(_mm_unpacklo_epi16(v00, v01), qw0),
                                     _mm_madd_epi16(_mm_unpacklo_epi16(v10, v11), qw1));
          __m128i t1 = _mm_add_epi32(_mm_madd_epi16(_mm_unpackhi_epi16(v00, v01), qw0),
                                     _mm_madd_epi16(_mm_unpackhi_epi16(v10, v11), qw1));
          t0 = _mm_srai_epi32(_mm_add_epi32(t0, qdelta), W_BITS1 - 5);
          t1 = _mm_srai_epi32(_mm_add_epi32(t1, qdelta), W_BITS1 - 5);
          diff0 = _mm_subs_epi16(_mm_packs_epi32(t0, t1), diff0);
          diff1 = _mm_unpackhi_epi16(diff0, diff0);
          diff0 = _mm_unpacklo_epi16(diff0, diff0);  // It0 It0 It1 It1 ...
          v00 = _mm_loadu_si128((const __m128i *) (dIptr));  // Ix0 Iy0 Ix1 Iy1 ...
          v01 = _mm_loadu_si128((const __m128i *) (dIptr + 8));
          v10 = _mm_mullo_epi16(v00, diff0);
          v11 = _mm_mulhi_epi16(v00, diff0);
          v00 = _mm_unpacklo_epi16(v10, v11);
          v10 = _mm_unpackhi_epi16(v10, v11);
          qb0 = _mm_add_ps(qb0, _mm_cvtepi32_ps(v00));
          qb1 = _mm_add_ps(qb1, _mm_cvtepi32_ps(v10));
          v10 = _mm_mullo_epi16(v01, diff1);
          v11 = _mm_mulhi_epi16(v01, diff1);
          v00 = _mm_unpacklo_epi16(v10, v11);
          v10 = _mm_unpackhi_epi16(v10, v11);
          qb0 = _mm_add_ps(qb0, _mm_cvtepi32_ps(v00));
          qb1 = _mm_add_ps(qb1, _mm_cvtepi32_ps(v10));
        }
#endif

        for (; x < winSize.width * cn; x++, dIptr += 2) {
          int diff = IFCV_DESCALE(Jptr[x] * iw00 + Jptr[x + cn] * iw01 +
              Jptr[x + stepJ] * iw10 + Jptr[x + stepJ + cn] * iw11,
                                  W_BITS1 - 5) - Iptr[x];
          b1 += static_cast<float> (diff * dIptr[0]);
          b2 += static_cast<float> (diff * dIptr[1]);
        }
      }

#if IFCV_SSE2
      float IFCV_DECL_ALIGNED(16) bbuf[4];
      _mm_store_ps(bbuf, _mm_add_ps(qb0, qb1));
      b1 += bbuf[0] + bbuf[2];
      b2 += bbuf[1] + bbuf[3];
#endif

      b1 *= FLT_SCALE;
      b2 *= FLT_SCALE;

      // @zzf the delta's positive and negative is different to paper
      cv::Point2f delta(static_cast<float> ((A12 * b2 - A22 * b1) * D),
                        static_cast<float> ((A12 * b1 - A11 * b2) * D));

      nextPt += delta;
      nextPts[ptidx] = nextPt + halfWin;

      if (delta.ddot(delta) <= criteria.epsilon)
        break;

      if (j > 0 && std::abs(delta.x + prevDelta.x) < 0.01 &&
          std::abs(delta.y + prevDelta.y) < 0.01) {
        nextPts[ptidx] -= delta * 0.5f;
        break;
      }
      prevDelta = delta;
    }

    // @zzf calculate every point's pixel intensity error between predicated position and previous position
    // if (status[ptidx] && level == 0)
    if (status[ptidx] && err && level == 0 && (flags & IFCV_LKFLOW_GET_MIN_EIGENVALS) == 0) {
      cv::Point2f nextPoint = nextPts[ptidx] - halfWin;
      cv::Point inextPoint;

      inextPoint.x = ifcvFloor(nextPoint.x);
      inextPoint.y = ifcvFloor(nextPoint.y);

      if (inextPoint.x < -winSize.width || inextPoint.x >= J.cols ||
          inextPoint.y < -winSize.height || inextPoint.y >= J.rows) {
        if (status)
          status[ptidx] = false;
        continue;
      }

      float aa = nextPoint.x - inextPoint.x;
      float bb = nextPoint.y - inextPoint.y;
      iw00 = ifcvRound((1.f - aa) * (1.f - bb) * (1 << W_BITS));
      iw01 = ifcvRound(aa * (1.f - bb) * (1 << W_BITS));
      iw10 = ifcvRound((1.f - aa) * bb * (1 << W_BITS));
      iw11 = (1 << W_BITS) - iw00 - iw01 - iw10;
      float errval = 0.f;

      for (y = 0; y < winSize.height; y++) {
        const uchar *Jptr = (const uchar *) J.data + (y + inextPoint.y) * stepJ + inextPoint.x * cn;
        const deriv_type *Iptr = (const deriv_type *) (IWinBuf.data + y * IWinBuf.step);

        for (x = 0; x < winSize.width * cn; x++) {
          int diff = IFCV_DESCALE(Jptr[x] * iw00 + Jptr[x + cn] * iw01 +
              Jptr[x + stepJ] * iw10 + Jptr[x + stepJ + cn] * iw11,
                                  W_BITS1 - 5) - Iptr[x];
          errval += std::abs(static_cast<float> (diff));
        }
      }
      err[ptidx] = errval * 1.f / (32 * winSize.width * cn * winSize.height);
    }
  }
}

int ifcv::calMaxLevel(cv::Size rectSize, cv::Size winSize, int maxLevel) {
  cv::Size sz = rectSize;
  for (int level = 0; level <= maxLevel; ++level) {
    sz = cv::Size((sz.width + 1) / 2,
                  (sz.height + 1) / 2);  // @zzf down sample to last image's 1/2, if sz < winSize, will break
    if (sz.width <= winSize.width || sz.height <= winSize.height) {
      return level;
    }
  }

  return maxLevel;
}

int ifcv::buildOpticalFlowPyramidPC(cv::InputArray _img,
                                    cv::OutputArrayOfArrays pyramid,
                                    cv::Size winSize,
                                    int maxLevel,
                                    int pyrBorder) {
  cv::Mat img = _img.getMat();
  CV_Assert(img.depth() == IFCV_8U && winSize.width > 2 && winSize.height > 2);
  int pyrstep = 1;
  pyramid.create(1, (maxLevel + 1) * pyrstep, 0 /*type*/, -1, true, 0);

  // level 0
  cv::Mat &temp = pyramid.getMatRef(0);

  if (!temp.empty())
    temp.adjustROI(winSize.height, winSize.height, winSize.width, winSize.width);
  if (temp.type() != img.type() || temp.cols != winSize.width * 2 + img.cols
      || temp.rows != winSize.height * 2 + img.rows)
    temp.create(img.rows + winSize.height * 2, img.cols + winSize.width * 2, img.type());

  if (pyrBorder == ifcv::IFBORDER_TRANSPARENT)
    img.copyTo(temp(cv::Rect(winSize.width, winSize.height, img.cols, img.rows)));
  else
    ifcopyMakeBorder(img,
                     temp,
                     winSize.height,
                     winSize.height,
                     winSize.width,
                     winSize.width,
                     pyrBorder);  // @zzf borderType == 0;
  temp.adjustROI(-winSize.height, -winSize.height, -winSize.width, -winSize.width);

  cv::Size sz = img.size();
  cv::Mat prevLevel = pyramid.getMatRef(0);
  cv::Mat thisLevel = prevLevel;

  for (int level = 0; level <= maxLevel; ++level) {
    if (level != 0) {
      cv::Mat &temp = pyramid.getMatRef(level * pyrstep);

      if (!temp.empty())
        temp.adjustROI(winSize.height, winSize.height, winSize.width, winSize.width);
      if (temp.type() != img.type() || temp.cols != winSize.width * 2 + sz.width
          || temp.rows != winSize.height * 2 + sz.height)
        temp.create(sz.height + winSize.height * 2, sz.width + winSize.width * 2, img.type());

      thisLevel = temp(cv::Rect(winSize.width, winSize.height, sz.width, sz.height));
      ifpyrDown(prevLevel, thisLevel, sz);

      if (pyrBorder != ifcv::IFBORDER_TRANSPARENT)
        ifcopyMakeBorder(thisLevel,
                         temp,
                         winSize.height,
                         winSize.height,
                         winSize.width,
                         winSize.width,
                         pyrBorder | ifcv::IFBORDER_ISOLATED);  // @zzf boderType == 16;
      temp.adjustROI(-winSize.height, -winSize.height, -winSize.width, -winSize.width);
    }

    sz = cv::Size((sz.width + 1) / 2,
                  (sz.height + 1) / 2);  // @zzf down sample to last image's 1/2, if sz < winSize, will break
    if (sz.width <= winSize.width || sz.height <= winSize.height) {
      pyramid.create(1, (level + 1) * pyrstep, 0 /*type*/, -1, true, 0);  // check this
      return level;
    }

    prevLevel = thisLevel;
  }

  return maxLevel;
}

void ifcv::calcOpticalFlowPyrLKPC(cv::InputArray _prevImg, cv::InputArray _nextImg,
                                  cv::InputArray _prevPts, cv::InputOutputArray _nextPts,
                                  cv::OutputArray _status, cv::OutputArray _err, cv::Rect targetRect,
                                  cv::Size winSize, int maxLevel,
                                  cv::TermCriteria criteria,
                                  int flags, double minEigThreshold) {
  mfxTime start_m;
  mfxGetTime(&start_m);
  cv::Mat prevPtsMat = _prevPts.getMat();
  const int derivDepth = cv::DataType<ifcv::detail::deriv_type>::depth;

  CV_Assert(maxLevel >= 0 && winSize.width > 2 && winSize.height > 2);

  int level = 0, i, npoints;
  CV_Assert((npoints = prevPtsMat.checkVector(2, IFCV_32F, true)) >= 0);

  if (npoints == 0) {
    _nextPts.release();
    _status.release();
    _err.release();
    return;
  }

  // if( !(flags & OPTFLOW_USE_INITIAL_FLOW) )
  //    _nextPts.create(prevPtsMat.size(), prevPtsMat.type(), -1, true);

  cv::Mat nextPtsMat = _nextPts.getMat();
  CV_Assert(nextPtsMat.checkVector(2, IFCV_32F, true) == npoints);

  // const cv::Point2f* prevPts = (const cv::Point2f*)prevPtsMat.data;
  cv::Point2f *prevPts = (cv::Point2f *) prevPtsMat.data;
  cv::Point2f *nextPts = (cv::Point2f *) nextPtsMat.data;

  // if (_status.needed())
  // {
  //   _status.create((int)npoints, 1, IFCV_8U, -1, true);
  //  }
  cv::Mat statusMat = _status.getMat(), errMat;
  CV_Assert(statusMat.isContinuous());
  uchar *status = statusMat.data;
  float *err = 0;

  for (i = 0; i < npoints; i++)
    status[i] = true;

  //   if( _err.needed() )
  //   {
  //       _err.create((int)npoints, 1, IFCV_32F, -1, true);
  // }

  errMat = _err.getMat();
  CV_Assert(errMat.isContinuous());
  err = reinterpret_cast<float *>(errMat.data);


  // cv::vector<cv::Mat> prevPyr, nextPyr;
  std::vector<cv::Mat> prevPyr, nextPyr;
  int lvlStep1 = 1;
  int lvlStep2 = 1;
  mfxTime start_m0;
  mfxGetTime(&start_m0);

  // @zzf use little area to search points
  cv::Mat _candidatePrevImg = _prevImg.getMat();
  cv::Mat _candidateNextImg = _nextImg.getMat();
  cv::Mat _seekPrevImg;
  cv::Mat _seekNextImg;
  maxLevel = calMaxLevel(targetRect.size(), winSize, maxLevel);
  int edgePixelPad = std::pow(2, maxLevel);
/// int littleSeekFlag = 1;
  cv::Rect prevIRect = cvRect(targetRect.x - edgePixelPad,
                              targetRect.y - edgePixelPad,
                              targetRect.width + 2 * edgePixelPad,
                              targetRect.height + 2 * edgePixelPad);
  // cv::Rect prevIRect = cvRect(targetRect.x - targetRect.width - edgePixelPad,
  // targetRect.y - targetRect.height - edgePixelPad,
  // 3 * targetRect.width + 2 * edgePixelPad, 3 * targetRect.height + 2 * edgePixelPad);
  // cv::Rect prevIRect = cvRect(0, 0, _candidatePrevImg.cols, _candidatePrevImg.rows);
  // littleSeekFlag = 0;
  validate_rect(&prevIRect, _candidatePrevImg.cols, _candidatePrevImg.rows);
  cv::Rect nextIRect = cvRect(targetRect.x - targetRect.width - edgePixelPad,
                              targetRect.y - targetRect.height - edgePixelPad,
                              3 * targetRect.width + 2 * edgePixelPad,
                              3 * targetRect.height + 2 * edgePixelPad);
  // cv::Rect nextIRect = cvRect(targetRect.x - edgePixelPad, targetRect.y - edgePixelPad,
  // targetRect.width + 2 * edgePixelPad, targetRect.height + 2 * edgePixelPad);
  // cv::Rect nextIRect = cvRect(0, 0, _candidateNextImg.cols, _candidateNextImg.rows);
  validate_rect(&nextIRect, _candidateNextImg.cols, _candidateNextImg.rows);
  _seekPrevImg = _candidatePrevImg(prevIRect);
  _seekNextImg = _candidateNextImg(nextIRect);
  maxLevel = buildOpticalFlowPyramidPC(_seekNextImg, nextPyr, winSize, maxLevel, false);
  maxLevel = buildOpticalFlowPyramidPC(_seekPrevImg, prevPyr, winSize, maxLevel, false);
  // printf("maxLevel = %d\n", maxLevel);
  // @zzf transform orig coordinate of prevPts and nextPts to new rect
  for (i = 0; i < npoints; i++) {
    nextPts[i].x = prevPts[i].x - nextIRect.x;
    nextPts[i].y = prevPts[i].y - nextIRect.y;
    // circle(_seekNextImg, nextPts[i], 2, cvScalar(0, 0, 255));
    prevPts[i].x = prevPts[i].x - prevIRect.x;
    prevPts[i].y = prevPts[i].y - prevIRect.y;
    // circle(_seekPrevImg, prevPts[i], 2, cvScalar(255, 0, 0));
  }

  // imshow("prev", _seekPrevImg);
  // cv::waitKey(0);
  // imshow("next", _seekNextImg);
  // cv::waitKey(0);

  // maxLevel = buildOpticalFlowPyramidPC(_prevImg, prevPyr, winSize, maxLevel, false);
  // @zzf make image pyramid without derivation
  // maxLevel = buildOpticalFlowPyramidPC(_nextImg, nextPyr, winSize, maxLevel, false);
  mfxTime stop_m0;
  mfxGetTime(&stop_m0);
/// double mtime0 = TimeDiffMsec(stop_m0, start_m0);
  // printf("buildOpticalFlowPyramidPC cost time %f\n", mtime0);

  if ((criteria.type & cv::TermCriteria::COUNT) == 0)
    criteria.maxCount = 30;
  else
    criteria.maxCount = std::min(std::max(criteria.maxCount, 0), 100);
  if ((criteria.type & cv::TermCriteria::EPS) == 0)
    criteria.epsilon = 0.01;
  else
    criteria.epsilon = std::min(std::max(criteria.epsilon, 0.), 10.);
  criteria.epsilon *= criteria.epsilon;

  // dI/dx ~ Ix, dI/dy ~ Iy
  cv::Mat derivIBuf;
  derivIBuf.create(prevPyr[0].rows + winSize.height * 2,
                   prevPyr[0].cols + winSize.width * 2,
                   IFCV_MAKETYPE(derivDepth, prevPyr[0].channels() * 2));

  for (level = maxLevel; level >= 0; level--) {
    // RInfo_txt << "level:" << level << std::endl;
    cv::Mat derivI;
    cv::Size imgSize = prevPyr[level * lvlStep1].size();
    cv::Mat _derivI(imgSize.height + winSize.height * 2,
                    imgSize.width + winSize.width * 2, derivIBuf.type(), derivIBuf.data);
    derivI = _derivI(cv::Rect(winSize.width, winSize.height, imgSize.width, imgSize.height));
    mfxTime start_m1;
    mfxGetTime(&start_m1);
    calcSharrDeriv(prevPyr[level * lvlStep1], derivI);
    ifcopyMakeBorder(derivI,
                     _derivI,
                     winSize.height,
                     winSize.height,
                     winSize.width,
                     winSize.width,
                     ifcv::IFBORDER_CONSTANT | ifcv::IFBORDER_ISOLATED);  // @zzf borderType == 16; NOLINT
    mfxTime stop_m1;
    mfxGetTime(&stop_m1);
/// double mtime1 = TimeDiffMsec(stop_m1, start_m1);
    // printf("%d level calcSharrDeriv cost mtime %f\n", level, mtime1);


    // CV_Assert(prevPyr[level * lvlStep1].size() == nextPyr[level * lvlStep2].size());
    // CV_Assert(prevPyr[level * lvlStep1].type() == nextPyr[level * lvlStep2].type());

    //// output the target IOrig dIOrig INext into txt.
    // if (level == 1)
    // {
    // cv::Mat IOrig = prevPyr[level * lvlStep1];
    // cv::Mat INext = nextPyr[level * lvlStep1];
    // cv::Mat dIOrig = derivI;
    // cv::Rect IOrigRect, INextRect;
    // double lscale = (1 << level);
    // if (littleSeekFlag == 1)
    // {
    //  IOrigRect = cvRect(cvFloor(edgePixelPad / lscale), cvFloor(edgePixelPad / lscale),
    //  cvCeil(targetRect.width / lscale), cvCeil(targetRect.height / lscale));
    //  INextRect = cvRect(cvFloor((edgePixelPad + targetRect.width) / lscale),
    //  cvFloor((edgePixelPad + targetRect.height) / lscale), cvCeil(targetRect.width / lscale),
    //  cvCeil(targetRect.height / lscale));
    // }
    // else
    // {
    //   IOrigRect = cvRect(cvFloor(targetRect.x / lscale), cvFloor(targetRect.y / lscale),
    //   cvCeil(targetRect.width / lscale), cvCeil(targetRect.height / lscale));
    //   INextRect = cvRect(cvFloor(targetRect.x / lscale), cvFloor(targetRect.y / lscale),
    //   cvCeil(targetRect.width / lscale), cvCeil(targetRect.height / lscale));
    // }
    //
    //   IOrig = IOrig(IOrigRect);
    //   INext = INext(INextRect);
    //  dIOrig = dIOrig(IOrigRect);
    //  const uchar *IOrigPtr = (const uchar *)IOrig.data;
    //  int IOrigStep = IOrig.step / IOrig.elemSize1();
    //  const uchar *INextPtr = (const uchar *)INext.data;
    //  int INextStep = INext.step / INext.elemSize1();
    //  const short *dIOrigPtr = (const short *)dIOrig.data;
    //  int dIOrigStep = dIOrig.step / dIOrig.elemSize1();
    //
    //  RInfo_txt << "IOrig" << std::endl;
    //  for (int y = 0; y < IOrig.rows; y++)
    //  {
    //    for (int x = 0; x < IOrig.cols; x++)
    //   {
    //      int val = (int)(IOrigPtr[y*IOrigStep + x]);
    //      RInfo_txt << val<<",";
    //   }
    //  RInfo_txt << std::endl;
    // }

    //  RInfo_txt << "INext" << std::endl;
    //  for (int y = 0; y < INext.rows; y++)
    //  {
    //    for (int x = 0; x < INext.cols; x++)
    //    {
    //       int val = (int)(INextPtr[y*INextStep + x]);
    //       RInfo_txt << val << ",";
    //     }
    //    RInfo_txt << std::endl;
    //  }

    // RInfo_txt << "dIOrig" << std::endl;
    // for (int y = 0; y < dIOrig.rows; y++)
    // {
    //   for (int x = 0; x < dIOrig.cols; x++)
    //    {
    //      short val = (short)(dIOrigPtr[y*dIOrigStep + x]);
    //      RInfo_txt << val << ",";
    //    }
    //   RInfo_txt << std::endl;
    // }
    // }


    // @zzf calculate every point's optical flow
    mfxTime start_m2;
    mfxGetTime(&start_m2);
    typedef ifcv::detail::LKTrackerInvoker LKTrackerInvoker;
    LKTrackerInvoker LKTrackerInvokerObj(prevPyr[level * lvlStep1], derivI,
                                         nextPyr[level * lvlStep2], prevPts, nextPts,
                                         status, err,
                                         winSize, criteria, level, maxLevel,
                                         flags, static_cast<float>(minEigThreshold));
    LKTrackerInvokerObj.runLK(npoints);
    mfxTime stop_m2;
    mfxGetTime(&stop_m2);
/// double mtime2 = TimeDiffMsec(stop_m2, start_m2);
    // printf("%d level LKTrackerInvoker cost mtime %f\n", level, mtime2);
  }

  // @zzf recover orig coordinate of prevPts and nextPts
  for (i = 0; i < npoints; i++) {
    prevPts[i].x = prevPts[i].x + prevIRect.x;
    prevPts[i].y = prevPts[i].y + prevIRect.y;
    nextPts[i].x = nextPts[i].x + nextIRect.x;
    nextPts[i].y = nextPts[i].y + nextIRect.y;
  }

  mfxTime stop_m;
  mfxGetTime(&stop_m);
/// double mtime = TimeDiffMsec(stop_m, start_m);
  // printf("calcOpticalFlowPyrLKPC cost time %f\n", mtime);
}

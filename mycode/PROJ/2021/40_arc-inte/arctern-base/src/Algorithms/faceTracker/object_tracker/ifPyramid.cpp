/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "src/Algorithms/faceTracker/object_tracker/ifPyramid.h"
#include <algorithm>

namespace ifcv {
static void ifcopyMakeBorder_8u(const uchar *src, size_t srcstep, cv::Size srcroi,
                                uchar *dst, size_t dststep, cv::Size dstroi,
                                int top, int left, int cn, int borderType);

static void ifcopyMakeConstBorder_8u(const uchar *src, size_t srcstep, cv::Size srcroi,
                                     uchar *dst, size_t dststep, cv::Size dstroi,
                                     int top, int left, int cn, const uchar *value);
}

struct ifHWFeatures {
  enum { MAX_FEATURE = IFCV_HARDWARE_MAX_FEATURE };

  ifHWFeatures(void) {
    memset(have, 0, sizeof(have));
    x86_family = 0;
  }

  static ifHWFeatures initialize(void) {
    ifHWFeatures f;
    int cpuid_data[4] = {0, 0, 0, 0};

#if defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
    __cpuid(cpuid_data, 1);
#elif defined __GNUC__ && (defined __i386__ || defined __x86_64__)
#ifdef __x86_64__
    asm __volatile__
    (
    "movl $1, %%eax\n\t"
    "cpuid\n\t"
    :[eax]"=a"(cpuid_data[0]), [ebx]"=b"(cpuid_data[1]), [ecx]"=c"(cpuid_data[2]), [edx]"=d"(cpuid_data[3])
    :
    : "cc"
    );   // NOLINT
#else
    asm volatile
        (
        "pushl %%ebx\n\t"
        "movl $1,%%eax\n\t"
        "cpuid\n\t"
        "popl %%ebx\n\t"
        : "=a"(cpuid_data[0]), "=c"(cpuid_data[2]), "=d"(cpuid_data[3])
        :
        : "cc"
        );   // NOLINT
#endif
#endif

    f.x86_family = (cpuid_data[0] >> 8) & 15;
    if (f.x86_family >= 6) {
      f.have[IFCV_CPU_MMX] = (cpuid_data[3] & (1 << 23)) != 0;
      f.have[IFCV_CPU_SSE] = (cpuid_data[3] & (1 << 25)) != 0;
      f.have[IFCV_CPU_SSE2] = (cpuid_data[3] & (1 << 26)) != 0;
      f.have[IFCV_CPU_SSE3] = (cpuid_data[2] & (1 << 0)) != 0;
      f.have[IFCV_CPU_SSSE3] = (cpuid_data[2] & (1 << 9)) != 0;
      f.have[IFCV_CPU_SSE4_1] = (cpuid_data[2] & (1 << 19)) != 0;
      f.have[IFCV_CPU_SSE4_2] = (cpuid_data[2] & (1 << 20)) != 0;
      f.have[IFCV_CPU_POPCNT] = (cpuid_data[2] & (1 << 23)) != 0;
      f.have[IFCV_CPU_AVX] = (((cpuid_data[2] & (1 << 28)) != 0)
          && ((cpuid_data[2] & (1 << 27)) != 0));  // OS uses XSAVE_XRSTORE and CPU support AVX
    }

    return f;
  }

  int x86_family;
  bool have[MAX_FEATURE + 1];
};

static ifHWFeatures featuresEnabled = ifHWFeatures::initialize();
static ifHWFeatures *currentFeatures = &featuresEnabled;

bool ifcheckHardwareSupport(int feature) {
  if (0 <= feature && feature <= IFCV_HARDWARE_MAX_FEATURE) {
    return currentFeatures->have[feature];
  } else {
    printf("error! function(ifcheckHardwareSupport): feature out of range!\n");
    return false;
  }
}

template<typename _Tp>
static inline _Tp ifsaturate_cast(double v) { return _Tp(v); }

int ifcv::ifcvFloor(double value) {
#if defined _MSC_VER && defined _M_X64 || (defined __GNUC__ && defined __SSE2__ && !defined __APPLE__)
  __m128d t = _mm_set_sd(value);
  int i = _mm_cvtsd_si32(t);
  return i - _mm_movemask_pd(_mm_cmplt_sd(t, _mm_cvtsi32_sd(t, i)));
#elif defined __GNUC__
  int i = static_cast<int>(value);
  return i - (i > value);
#else
  int i = cvRound(value);
  float diff = static_cast<float>(value - i);
  return i - (diff < 0);
#endif
}

int ifcv::ifcvRound(double value) {
#if (defined _MSC_VER && defined _M_X64) || (defined __GNUC__ && defined __x86_64__ && defined __SSE2__ && !defined __APPLE__)   // NOLINT
  __m128d t = _mm_set_sd(value);
  return _mm_cvtsd_si32(t);
#elif defined _MSC_VER && defined _M_IX86
  int t;
  __asm
  {                      // NOLINT
      fld value;
      fistp t;
  }
  return t;
#elif defined _MSC_VER && defined _M_ARM && defined HAVE_TEGRA_OPTIMIZATION
  TEGRA_ROUND(value);
#elif defined HAVE_LRINT || defined CV_ICC || defined __GNUC__
#  ifdef HAVE_TEGRA_OPTIMIZATION
  TEGRA_ROUND(value);
#  else
  return static_cast<int>(lrint(value));
#  endif
#else
  double intpart, fractpart;
  fractpart = modf(value, &intpart);
  if ((fabs(fractpart) != 0.5) || ((((int)intpart) % 2) != 0))     // NOLINT
      return static_cast<int>(value + (value >= 0 ? 0.5 : -0.5));
  else
      return static_cast<int>intpart;
#endif
}

template<>
inline uchar ifsaturate_cast<uchar>(double v) {
  int iv = ifcv::ifcvRound(v);
  return (uchar) ((unsigned) iv <= UCHAR_MAX ? iv : iv > 0 ? UCHAR_MAX : 0);
}

template<>
inline short ifsaturate_cast<short>(double v) {  // NOLINT
  int iv = ifcv::ifcvRound(v);
  return (short) ((unsigned) (iv - SHRT_MIN) <= (unsigned) USHRT_MAX ?  // NOLINT
                  iv : iv > 0 ? SHRT_MAX : SHRT_MIN);
}

void ifcv::ifscalarToRawData(const cv::Scalar &s, void *_buf, int type, int unroll_to) {
  int i, depth = IFCV_MAT_DEPTH(type), cn = IFCV_MAT_CN(type);
  CV_Assert(cn <= 4);
  switch (depth) {
    case IFCV_8U: {
      uchar *buf = (uchar *) _buf;  // NOLINT
      for (i = 0; i < cn; i++)
        buf[i] = ifsaturate_cast<uchar>(s.val[i]);
      for (; i < unroll_to; i++)
        buf[i] = buf[i - cn];
    }
      break;
    case CV_16S: {
      short *buf = (short *) _buf;    // NOLINT
      for (i = 0; i < cn; i++)
        buf[i] = ifsaturate_cast<short>(s.val[i]);  // NOLINT
      for (; i < unroll_to; i++)
        buf[i] = buf[i - cn];
    }
      break;
    default: CV_Error(CV_StsUnsupportedFormat, "");
  }
}

int ifcv::ifborderInterpolate(int p, int len, int borderType) {
  if ((unsigned) p < (unsigned) len) {}
  else if (borderType == IFBORDER_REPLICATE) {   // NOLINT
    p = p < 0 ? 0 : len - 1;
  } else if (borderType == IFBORDER_REFLECT || borderType == IFBORDER_REFLECT_101) {
    int delta = borderType == IFBORDER_REFLECT_101;
    if (len == 1) {
      return 0;
    }
    do {
      if (p < 0) {
        p = -p - 1 + delta;
      } else {
        p = len - 1 - (p - len) - delta;
      }
    } while ((unsigned) p >= (unsigned) len);
  } else if (borderType == IFBORDER_WRAP) {
    if (p < 0) {
      p -= ((p - len + 1) / len) * len;
    }
    if (p >= len) {
      p %= len;
    }
  } else if (borderType == IFBORDER_CONSTANT) {
    p = -1;
  } else {
    CV_Error(CV_StsBadArg, "Unknown/unsupported border type");
  }

  return p;
}

static void ifcv::ifcopyMakeBorder_8u(const uchar *src, size_t srcstep, cv::Size srcroi,
                                      uchar *dst, size_t dststep, cv::Size dstroi,
                                      int top, int left, int cn, int borderType) {
  const int isz = sizeof(int);
  int i, j, k, elemSize = 1;
  bool intMode = false;

  if ((cn | srcstep | dststep | (size_t) src | (size_t) dst) % isz == 0) {
    cn /= isz;
    elemSize = isz;
    intMode = true;
  }

  cv::AutoBuffer<int> _tab((dstroi.width - srcroi.width) * cn);
  int *tab = _tab;
  int right = dstroi.width - srcroi.width - left;
  int bottom = dstroi.height - srcroi.height - top;

  for (i = 0; i < left; i++) {
    j = ifborderInterpolate(i - left, srcroi.width, borderType) * cn;
    for (k = 0; k < cn; k++)
      tab[i * cn + k] = j + k;
  }

  for (i = 0; i < right; i++) {
    j = ifborderInterpolate(srcroi.width + i, srcroi.width, borderType) * cn;
    for (k = 0; k < cn; k++)
      tab[(i + left) * cn + k] = j + k;
  }

  srcroi.width *= cn;
  dstroi.width *= cn;
  left *= cn;
  right *= cn;

  uchar *dstInner = dst + dststep * top + left * elemSize;

  for (i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep) {
    if (dstInner != src)
      memcpy(dstInner, src, srcroi.width * elemSize);

    if (intMode) {
      const int *isrc = (int *) src;       // NOLINT
      int *idstInner = (int *) dstInner;  // NOLINT
      for (j = 0; j < left; j++)
        idstInner[j - left] = isrc[tab[j]];
      for (j = 0; j < right; j++)
        idstInner[j + srcroi.width] = isrc[tab[j + left]];
    } else {
      for (j = 0; j < left; j++)
        dstInner[j - left] = src[tab[j]];
      for (j = 0; j < right; j++)
        dstInner[j + srcroi.width] = src[tab[j + left]];
    }
  }

  dstroi.width *= elemSize;
  dst += dststep * top;

  for (i = 0; i < top; i++) {
    j = ifborderInterpolate(i - top, srcroi.height, borderType);
    memcpy(dst + (i - top) * dststep, dst + j * dststep, dstroi.width);
  }

  for (i = 0; i < bottom; i++) {
    j = ifborderInterpolate(i + srcroi.height, srcroi.height, borderType);
    memcpy(dst + (i + srcroi.height) * dststep, dst + j * dststep, dstroi.width);
  }
}

static void ifcv::ifcopyMakeConstBorder_8u(const uchar *src, size_t srcstep, cv::Size srcroi,
                                           uchar *dst, size_t dststep, cv::Size dstroi,
                                           int top, int left, int cn, const uchar *value) {
  int i, j;
  cv::AutoBuffer<uchar> _constBuf(dstroi.width * cn);
  uchar *constBuf = _constBuf;
  int right = dstroi.width - srcroi.width - left;
  int bottom = dstroi.height - srcroi.height - top;

  for (i = 0; i < dstroi.width; i++) {
    for (j = 0; j < cn; j++)
      constBuf[i * cn + j] = value[j];
  }

  srcroi.width *= cn;
  dstroi.width *= cn;
  left *= cn;
  right *= cn;

  uchar *dstInner = dst + dststep * top + left;

  for (i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep) {
    if (dstInner != src)
      memcpy(dstInner, src, srcroi.width);
    memcpy(dstInner - left, constBuf, left);
    memcpy(dstInner + srcroi.width, constBuf, right);
  }

  dst += dststep * top;

  for (i = 0; i < top; i++)
    memcpy(dst + (i - top) * dststep, constBuf, dstroi.width);

  for (i = 0; i < bottom; i++)
    memcpy(dst + (i + srcroi.height) * dststep, constBuf, dstroi.width);
}

void ifcv::ifcopyMakeBorder(cv::InputArray _src, cv::OutputArray _dst, int top, int bottom,
                            int left, int right, int borderType, const cv::Scalar &value) {
  cv::Mat src = _src.getMat();
  CV_Assert(top >= 0 && bottom >= 0 && left >= 0 && right >= 0);

  if (src.isSubmatrix() && (borderType & IFBORDER_ISOLATED) == 0) {
    cv::Size wholeSize;
    cv::Point ofs;
    src.locateROI(wholeSize, ofs);
    int dtop = std::min(ofs.y, top);
    int dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
    int dleft = std::min(ofs.x, left);
    int dright = std::min(wholeSize.width - src.cols - ofs.x, right);
    src.adjustROI(dtop, dbottom, dleft, dright);
    top -= dtop;
    left -= dleft;
    bottom -= dbottom;
    right -= dright;
  }

  _dst.create(src.rows + top + bottom, src.cols + left + right, src.type());
  cv::Mat dst = _dst.getMat();

  if (top == 0 && left == 0 && bottom == 0 && right == 0) {
    if (src.data != dst.data || src.step != dst.step)
      src.copyTo(dst);
    return;
  }

  borderType &= ~IFBORDER_ISOLATED;

  if (borderType != IFBORDER_CONSTANT) {
    ifcopyMakeBorder_8u(src.data, src.step, src.size(),
                        dst.data, dst.step, dst.size(),
                        top, left, (int) src.elemSize(), borderType);  // NOLINT
  } else {
    int cn = src.channels(), cn1 = cn;
    cv::AutoBuffer<double> buf(cn);
    if (cn > 4) {
      CV_Assert(value[0] == value[1] && value[0] == value[2] && value[0] == value[3]);
      cn1 = 1;
    }
    ifscalarToRawData(value, buf, IFCV_MAKETYPE(src.depth(), cn1), cn);
    ifcopyMakeConstBorder_8u(src.data, src.step, src.size(),
                             dst.data, dst.step, dst.size(),
                             top, left, (int) src.elemSize(), (uchar *) (double *) buf);  // NOLINT
  }
}

// @zzf fixed point cast
template<typename T, int shift>
struct ifFixPtCast {
  typedef int type1;
  typedef T rtype;
  rtype operator()(type1 arg) const { return (T) ((arg + (1 << (shift - 1))) >> shift); }
};

template<typename T1, typename T2>
struct ifNoVec {
  int operator()(T1 **, T2 *, int, int) const { return 0; }
};

/*!
Aligns pointer by the certain number of bytes
This small inline function aligns the pointer by the certain number of bytes by shifting
it forward by 0 or a positive offset.
*/
template<typename _Tp>
static inline _Tp *ifalignPtr(_Tp *ptr, int n = sizeof(_Tp)) {
  return (_Tp *) (((size_t) ptr + n - 1) & -n);  // NOLINT
}

/*!
Aligns buffer size by the certain number of bytes
This small inline function aligns a buffer size by the certain number of bytes by enlarging it.
*/
// @zzf for 2^n bytes align it is right
static inline size_t ifalignSize(size_t sz, int n) {
  return (sz + n - 1) & -n;
}

#if IFCV_SSE2

struct ifPyrDownVec_32s8u {
  int operator()(int **src, uchar *dst, int, int width) const {
    if (!ifcheckHardwareSupport(IFCV_CPU_SSE2))
      return 0;

    int x = 0;
    const int *row0 = src[0], *row1 = src[1], *row2 = src[2], *row3 = src[3], *row4 = src[4];
    __m128i delta = _mm_set1_epi16(128);

    for (; x <= width - 16; x += 16) {
      __m128i r0, r1, r2, r3, r4, t0, t1;
      r0 = _mm_packs_epi32(_mm_load_si128((const __m128i *) (row0 + x)),
                           _mm_load_si128((const __m128i *) (row0 + x + 4)));
      r1 = _mm_packs_epi32(_mm_load_si128((const __m128i *) (row1 + x)),
                           _mm_load_si128((const __m128i *) (row1 + x + 4)));
      r2 = _mm_packs_epi32(_mm_load_si128((const __m128i *) (row2 + x)),
                           _mm_load_si128((const __m128i *) (row2 + x + 4)));
      r3 = _mm_packs_epi32(_mm_load_si128((const __m128i *) (row3 + x)),
                           _mm_load_si128((const __m128i *) (row3 + x + 4)));
      r4 = _mm_packs_epi32(_mm_load_si128((const __m128i *) (row4 + x)),
                           _mm_load_si128((const __m128i *) (row4 + x + 4)));
      r0 = _mm_add_epi16(r0, r4);
      r1 = _mm_add_epi16(_mm_add_epi16(r1, r3), r2);
      r0 = _mm_add_epi16(r0, _mm_add_epi16(r2, r2));
      t0 = _mm_add_epi16(r0, _mm_slli_epi16(r1, 2));
      r0 = _mm_packs_epi32(_mm_load_si128((const __m128i *) (row0 + x + 8)),
                           _mm_load_si128((const __m128i *) (row0 + x + 12)));
      r1 = _mm_packs_epi32(_mm_load_si128((const __m128i *) (row1 + x + 8)),
                           _mm_load_si128((const __m128i *) (row1 + x + 12)));
      r2 = _mm_packs_epi32(_mm_load_si128((const __m128i *) (row2 + x + 8)),
                           _mm_load_si128((const __m128i *) (row2 + x + 12)));
      r3 = _mm_packs_epi32(_mm_load_si128((const __m128i *) (row3 + x + 8)),
                           _mm_load_si128((const __m128i *) (row3 + x + 12)));
      r4 = _mm_packs_epi32(_mm_load_si128((const __m128i *) (row4 + x + 8)),
                           _mm_load_si128((const __m128i *) (row4 + x + 12)));
      r0 = _mm_add_epi16(r0, r4);
      r1 = _mm_add_epi16(_mm_add_epi16(r1, r3), r2);
      r0 = _mm_add_epi16(r0, _mm_add_epi16(r2, r2));
      t1 = _mm_add_epi16(r0, _mm_slli_epi16(r1, 2));
      t0 = _mm_srli_epi16(_mm_add_epi16(t0, delta), 8);
      t1 = _mm_srli_epi16(_mm_add_epi16(t1, delta), 8);
      _mm_storeu_si128((__m128i *) (dst + x), _mm_packus_epi16(t0, t1));  // NOLINT
    }

    for (; x <= width - 4; x += 4) {
      __m128i r0, r1, r2, r3, r4, z = _mm_setzero_si128();
      r0 = _mm_packs_epi32(_mm_load_si128((const __m128i *) (row0 + x)), z);
      r1 = _mm_packs_epi32(_mm_load_si128((const __m128i *) (row1 + x)), z);
      r2 = _mm_packs_epi32(_mm_load_si128((const __m128i *) (row2 + x)), z);
      r3 = _mm_packs_epi32(_mm_load_si128((const __m128i *) (row3 + x)), z);
      r4 = _mm_packs_epi32(_mm_load_si128((const __m128i *) (row4 + x)), z);
      r0 = _mm_add_epi16(r0, r4);
      r1 = _mm_add_epi16(_mm_add_epi16(r1, r3), r2);
      r0 = _mm_add_epi16(r0, _mm_add_epi16(r2, r2));
      r0 = _mm_add_epi16(r0, _mm_slli_epi16(r1, 2));
      r0 = _mm_srli_epi16(_mm_add_epi16(r0, delta), 8);
      *(int *) (dst + x) = _mm_cvtsi128_si32(_mm_packus_epi16(r0, r0));  // NOLINT
    }

    return x;
  }
};

#else

typedef ifNoVec<int, uchar> ifPyrDownVec_32s8u;

#endif

template<class CastOp, class VecOp>
void
ifpyrDown_(const cv::Mat &_src, cv::Mat &_dst, int borderType) {
  const int PD_SZ = 5;
  typedef typename CastOp::type1 WT;
  typedef typename CastOp::rtype T;

  cv::Size ssize = _src.size(), dsize = _dst.size();  // @zzf  size() is w*h
  int cn = _src.channels();
  int bufstep = static_cast<int>(ifalignSize(dsize.width * cn, 16));
  cv::AutoBuffer<WT> _buf(bufstep * PD_SZ + 16);
  WT *buf = ifalignPtr((WT *) _buf, 16);  // NOLINT
  int tabL[IFCV_CN_MAX * (PD_SZ + 2)], tabR[IFCV_CN_MAX * (PD_SZ + 2)];
  cv::AutoBuffer<int> _tabM(dsize.width * cn);
  int *tabM = _tabM;
  WT *rows[PD_SZ];
  CastOp castOp;
  VecOp vecOp;

  if (!(std::abs(dsize.width * 2 - ssize.width) <= 2 && std::abs(dsize.height * 2 - ssize.height) <= 2)) {
    printf("error! function(ifpyrDown_): src and destination's size is not match!\n");
    return;
  }

  // @zzf for even width, the width0 is width/2 + 1; for uneven width ,the width0 is width/2
  int k, x, sy0 = -PD_SZ / 2, sy = sy0, width0 = std::min((ssize.width - PD_SZ / 2 - 1) / 2 + 1,
                                                          dsize.width);

  for (x = 0; x <= PD_SZ + 1; x++) {
    int sx0 = ifcv::ifborderInterpolate(x - PD_SZ / 2, ssize.width, borderType)
        * cn;  // @zzf borderType use default value 4, the borderInterpolate type is reflect.
    int sx1 = ifcv::ifborderInterpolate(x + width0 * 2 - PD_SZ / 2, ssize.width, borderType) * cn;
    for (k = 0; k < cn; k++) {
      tabL[x * cn + k] = sx0 + k;  // @zzf tabL and tabR cast x in outside range  to x in inside range
      tabR[x * cn + k] = sx1 + k;
    }
  }

  ssize.width *= cn;
  dsize.width *= cn;
  width0 *= cn;

  for (x = 0; x < dsize.width; x++)
    tabM[x] = (x / cn) * 2 * cn + x % cn;

  for (int y = 0; y < dsize.height; y++) {
    T *dst = reinterpret_cast<T *>(_dst.data + _dst.step * y);
    WT *row0, *row1, *row2, *row3, *row4;

    // fill the ring buffer (horizontal convolution and decimation)
    for (; sy <= y * 2 + 2; sy++) {  // @zzf sy denotes y in src image
      WT *row = buf + ((sy - sy0) % PD_SZ) * bufstep;
      int _sy = ifcv::ifborderInterpolate(sy, ssize.height, borderType);
      const T *src = (const T *) (_src.data + _src.step * _sy);
      int limit = cn;
      const int *tab = tabL;

      for (x = 0;;) {
        for (; x < limit; x++) {
          row[x] = src[tab[x + cn * 2]] * 6 + (src[tab[x + cn]] + src[tab[x + cn * 3]]) * 4 +
              src[tab[x]] + src[tab[x + cn * 4]];
        }

        if (x == dsize.width)
          break;

        if (cn == 1) {  // @zzf core code.
          for (; x < width0; x++)
            row[x] = src[x * 2] * 6 + (src[x * 2 - 1] + src[x * 2 + 1]) * 4 +
                src[x * 2 - 2] + src[x * 2 + 2];
        } else if (cn == 3) {
          for (; x < width0; x += 3) {
            const T *s = src + x * 2;
            WT t0 = s[0] * 6 + (s[-3] + s[3]) * 4 + s[-6] + s[6];
            WT t1 = s[1] * 6 + (s[-2] + s[4]) * 4 + s[-5] + s[7];
            WT t2 = s[2] * 6 + (s[-1] + s[5]) * 4 + s[-4] + s[8];
            row[x] = t0;
            row[x + 1] = t1;
            row[x + 2] = t2;
          }
        } else if (cn == 4) {
          for (; x < width0; x += 4) {
            const T *s = src + x * 2;
            WT t0 = s[0] * 6 + (s[-4] + s[4]) * 4 + s[-8] + s[8];
            WT t1 = s[1] * 6 + (s[-3] + s[5]) * 4 + s[-7] + s[9];
            row[x] = t0;
            row[x + 1] = t1;
            t0 = s[2] * 6 + (s[-2] + s[6]) * 4 + s[-6] + s[10];
            t1 = s[3] * 6 + (s[-1] + s[7]) * 4 + s[-5] + s[11];
            row[x + 2] = t0;
            row[x + 3] = t1;
          }
        } else {
          for (; x < width0; x++) {
            int sx = tabM[x];
            row[x] = src[sx] * 6 + (src[sx - cn] + src[sx + cn]) * 4 +
                src[sx - cn * 2] + src[sx + cn * 2];
          }
        }

        limit = dsize.width;
        tab = tabR - x;
      }
    }

    // do vertical convolution and decimation and write the result to the destination image
    for (k = 0; k < PD_SZ; k++)
      rows[k] = buf + ((y * 2 - PD_SZ / 2 + k - sy0) % PD_SZ) * bufstep;
    row0 = rows[0];
    row1 = rows[1];
    row2 = rows[2];
    row3 = rows[3];
    row4 = rows[4];

    x = vecOp(rows, dst, static_cast<int>(_dst.step), dsize.width);
    for (; x < dsize.width; x++)  // @zzf core code
      dst[x] = castOp(row2[x] * 6 + (row1[x] + row3[x]) * 4 + row0[x] + row4[x]);
  }
}

void ifcv::ifpyrDown(cv::InputArray _src, cv::OutputArray _dst, const cv::Size &_dsz, int borderType) {
  cv::Mat src = _src.getMat();
  cv::Size dsz = _dsz == cv::Size() ? cv::Size((src.cols + 1) / 2, (src.rows + 1) / 2) : _dsz;
  _dst.create(dsz, src.type());
  cv::Mat dst = _dst.getMat();

  int depth = src.depth();
  ifPyrFunc func = 0;
  if (depth == IFCV_8U) {
    func = ifpyrDown_<ifFixPtCast<uchar, 8>, ifPyrDownVec_32s8u>;
  } else {
    printf("error! function(ifpyrDown): src 's depth type is not IFCV_8U!\n");
    return;  // @zzf error
  }

  func(src, dst, borderType);
}

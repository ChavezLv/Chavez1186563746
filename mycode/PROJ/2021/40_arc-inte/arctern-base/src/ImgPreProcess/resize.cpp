
/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#include "resize.h"
#include "cuda/cuda_types.hpp"
#include "cuda/common.hpp"
#include "cvtColor.h"
#include "padding.h"
#include "Base/base.h"
namespace arctern {
namespace cuda {
namespace device {
template<typename T>
void resize(const PtrStepSzb &src,
            const PtrStepSzb &srcWhole,
            int yoff,
            int xoff,
            const PtrStepSzb &dst,
            float fy,
            float fx,
            int interpolation,
            cudaStream_t stream);
}
}
}

int double2int(double value){
  return (int)(value + (value >= 0 ? 0.5 : -0.5));
}

namespace arctern{
int getPaddingWidth(const DeviceBuffer &_src){
  int id;
  cudaGetDevice(&id);
  cudaDeviceProp prop;
  cudaGetDeviceProperties(&prop,id);
  int pitch = prop.texturePitchAlignment;
  int srcStep = _src.Width() * _src.Channel() * _src.getElementSize();
  if(srcStep % pitch != 0){
    int s = srcStep / pitch;
    int newSrcStep = (s+1) * pitch;
    int newSrcWidth = newSrcStep / _src.Channel() / _src.getElementSize();
    assert(newSrcWidth > _src.Width());
    return newSrcWidth;
  }
  return 0;
}
}


void arctern::cuda::resize(const DeviceBuffer &_src,
                           DeviceBuffer &_dst,
                           double fy,
                           double fx,
                           int interpolation,
                           cudaStream_t *pStream) {
  if (_src.Width() <= 0 || _src.Height() <= 0 || _src.data() == nullptr) {
    std::cout << "Resize: error params" << endl;
    return;
  }

  if(_dst.Height() <= 0 || _dst.Width() <= 0){
    assert(fy > 0);
    assert(fx > 0);
    assert(_dst.data() == nullptr);
    _dst.resize(double2int(fy*_src.Height()),
                double2int(fx*_src.Width()),
                _src.Channel(),
                _src.Type());
  }
  else{
    fy = static_cast<double>(1.0 * _dst.Height() / _src.Height());
    fx = static_cast<double>(1.0 * _dst.Width() / _src.Width());
  }

  /*if (_dst.Width() == _src.Width() && _dst.Height() == _src.Height()) {
  ///> have a question
  ///>  _dst = _src;
    assert(_src.nbBytes() == _dst.nbBytes());
    cudaMemcpy(_dst.data(),_src.data(),_src.nbBytes(),cudaMemcpyDeviceToDevice);
    return;
  }*/
  assert(_src.Channel() == _dst.Channel());
  assert(_src.Channel() <= 4);
  assert(interpolation == NEAREST || interpolation == LINEAR || interpolation == CUBIC
             || interpolation == AREA);

  const DeviceBuffer *new_src;
  DeviceBuffer *new_dst;
  new_src = &_src;
  new_dst = &_dst;

  typedef void (*func_t)(const PtrStepSzb &src, const PtrStepSzb &srcWhole, int yoff, int xoff,
                         const PtrStepSzb &dst, float fy, float fx, int interpolation, cudaStream_t stream);

  static const func_t funcs[4][4] =
  {
      {device::resize<uchar>, 0 , device::resize<uchar3>, device::resize<uchar4>},
      {0 , 0 , 0 , 0 },
      {0 , 0 , 0 , 0 },
      {device::resize<float>, 0 , device::resize<float3>, device::resize<float4>}
  };


  const func_t func = funcs[new_src->getElementSize() - 1][new_src->Channel() - 1];

  if (!func)
    std::cout << "Unsupported combination of source and destination types" << endl;

  int srcStep = new_src->Step();
  PtrStepSzb wholeSrc(static_cast<int>(new_src->Height()), static_cast<int>(new_src->Width()),
                      static_cast<unsigned char*>(const_cast<void*>(new_src->data())),
                      srcStep);

  int dstStep = new_dst->Step();
  PtrStepSzb dst(new_dst->Height(),new_dst->Width(),
                 static_cast<unsigned char*>(new_dst->data()),dstStep);

  if(pStream!= nullptr){
    func(wholeSrc,
         wholeSrc,
         0,
         0,
         dst,
         static_cast<float>(1 / fy),
         static_cast<float>(1 / fx),
         interpolation,*pStream);
  }
  else{
    func(wholeSrc,
         wholeSrc,
         0,
         0,
         dst,
         static_cast<float>(1 / fy),
         static_cast<float>(1 / fx),
         interpolation,0);
  }
}


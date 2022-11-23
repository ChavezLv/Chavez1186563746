//
// Created by Admin on 2021/7/7.
//

#include <ImgPreProcess/cuda/vec_traits.hpp>
#include "cvtColor.h"
#include "cuda/cvtColor.cuh"
namespace arctern{
namespace cuda{
void cvt3CTo4C(const DeviceBuffer& _src,DeviceBuffer& _dst){
  assert(_src.Channel() == 3);
  assert(_dst.Channel() == 4);
  assert(_src.Type() == _dst.Type());
  using func_t = void (*)(const PtrStepSzb &, PtrStepSzb &);
  static const func_t funcs[] = {
      device::cvt3CTo4C<uchar3,uchar4>,0,0,device::cvt3CTo4C<float3,float4>
  };
  auto func = funcs[_src.getElementSize()-1];
  if (!func)
    std::cout << "Unsupported combination of source and destination types" << endl;

  int srcStep = _src.Width() * _src.Channel() * _src.getElementSize();
  PtrStepSzb src(static_cast<int>(_src.Height()), static_cast<int>(_src.Width()),
                 static_cast<unsigned char*>(const_cast<void*>(_src.data())),
                 srcStep);

  int dstStep = _dst.Width() * _dst.Channel() * _dst.getElementSize();
  PtrStepSzb dst(_dst.Height(),_dst.Width(),static_cast<unsigned char*>(_dst.data()),dstStep);

  func(src,dst);
}

void cvt4CTo3C(const DeviceBuffer& _src,DeviceBuffer& _dst){
  assert(_src.Channel() == 4);
  assert(_dst.Channel() == 3);
  assert(_src.Type() == _dst.Type());
  using func_t = void (*)(const PtrStepSzb &, PtrStepSzb &);
  static const func_t funcs[] = {
      device::cvt4CTo3C<uchar4,uchar3>,0,0,device::cvt4CTo3C<float4,float3>
  };
  auto func = funcs[_src.getElementSize()-1];
  if (!func)
    std::cout << "Unsupported combination of source and destination types" << endl;

  int srcStep = _src.Width() * _src.Channel() * _src.getElementSize();
  PtrStepSzb src(static_cast<int>(_src.Height()), static_cast<int>(_src.Width()),
                 static_cast<unsigned char*>(const_cast<void*>(_src.data())),
                 srcStep);

  int dstStep = _dst.Width() * _dst.Channel() * _dst.getElementSize();
  PtrStepSzb dst(_dst.Height(),_dst.Width(),static_cast<unsigned char*>(_dst.data()),dstStep);

  func(src,dst);
}

void ExtractChannel(const DeviceBuffer& _src,DeviceBuffer& _dst,cudaStream_t *m_pStream){
  assert(_src.Channel() == 3);
  assert(_dst.Channel() == 3);
  assert(_src.Type() == _dst.Type());
  using func_t = void (*)(const PtrStepSzb &, PtrStepSzb &,cudaStream_t *);
  static const func_t funcs[] = {
      device::ExtractChannel<uchar3,uchar>,0,0,device::ExtractChannel<float3,float>
  };
  auto func = funcs[_src.getElementSize()-1];
  if (!func)
    std::cout << "Unsupported combination of source and destination types" << endl;

  int srcStep = _src.Step();
  PtrStepSzb src(static_cast<int>(_src.Height()), static_cast<int>(_src.Width()),
                 static_cast<unsigned char*>(const_cast<void*>(_src.data())),
                 srcStep);

  int dstStep = _dst.Step();
  PtrStepSzb dst(_dst.Height(),_dst.Width(),static_cast<unsigned char*>(_dst.data()),dstStep);

  func(src,dst,m_pStream);
}

void cvtRGB2BGR(const DeviceBuffer& _src,
                       DeviceBuffer& _dst,cudaStream_t *m_pStream){
  cvtBGR2RGB(_src,_dst,m_pStream);
}

void cvtBGR2RGB(const DeviceBuffer& _src,
                       DeviceBuffer& _dst,cudaStream_t *m_pStream){
  assert(_src.Channel() == 3);
  assert(_dst.Channel() == 3);
  assert(_src.Type() == _dst.Type());
  assert(_src.Width() == _dst.Width());
  assert(_src.Height() == _dst.Height());

  using func_t = void (*)(const PtrStepSzb &, PtrStepSzb &,cudaStream_t *);
  static const func_t funcs[] = {
      device::cvtBGR2RGB<uchar3>,0,0,device::cvtBGR2RGB<float3>
  };
  auto func = funcs[_src.getElementSize()-1];
  if (!func)
    std::cout << "Unsupported combination of source and destination types" << endl;

  int srcStep = _src.Step();
  PtrStepSzb src(static_cast<int>(_src.Height()), static_cast<int>(_src.Width()),
                 static_cast<unsigned char*>(const_cast<void*>(_src.data())),
                 srcStep);

  int dstStep = _dst.Step();
  PtrStepSzb dst(_dst.Height(),_dst.Width(),static_cast<unsigned char*>(_dst.data()),dstStep);
  func(src,dst,m_pStream);
}

void cvtBGR2GRAY(const DeviceBuffer& _src,
                        DeviceBuffer& _dst,cudaStream_t *m_pStream){
  assert(_src.Channel() == 3);
  assert(_dst.Channel() == 1);
  assert(_src.Type() == _dst.Type());
  assert(_src.Width() == _dst.Width());
  assert(_src.Height() == _dst.Height());

  using func_t = void (*)(const PtrStepSzb &, PtrStepSzb &,cudaStream_t *);
  static const func_t funcs[] = {
      device::cvtBGR2GRAY<uchar3,uchar>,0,0,device::cvtBGR2GRAY<float3,float>
  };
  auto func = funcs[_src.getElementSize()-1];
  if (!func)
    std::cout << "Unsupported combination of source and destination types" << endl;

  int srcStep = _src.Step();
  PtrStepSzb src(static_cast<int>(_src.Height()), static_cast<int>(_src.Width()),
                 static_cast<unsigned char*>(const_cast<void*>(_src.data())),
                 srcStep);

  int dstStep = _dst.Step();
  PtrStepSzb dst(_dst.Height(),_dst.Width(),static_cast<unsigned char*>(_dst.data()),dstStep);

  func(src,dst,m_pStream);
}

void cvtRGB2GRAY(const DeviceBuffer& _src,
                 DeviceBuffer& _dst,cudaStream_t *m_pStream){
  assert(_src.Channel() == 3);
  assert(_dst.Channel() == 1);
  assert(_src.Type() == _dst.Type());
  assert(_src.Width() == _dst.Width());
  assert(_src.Height() == _dst.Height());

  using func_t = void (*)(const PtrStepSzb &, PtrStepSzb &,cudaStream_t *);
  static const func_t funcs[] = {
      device::cvtRGB2GRAY<uchar3,uchar>,0,0,device::cvtRGB2GRAY<float3,float>
  };
  auto func = funcs[_src.getElementSize()-1];
  if (!func)
    std::cout << "Unsupported combination of source and destination types" << endl;

  int srcStep = _src.Step();
  PtrStepSzb src(static_cast<int>(_src.Height()), static_cast<int>(_src.Width()),
                 static_cast<unsigned char*>(const_cast<void*>(_src.data())),
                 srcStep);

  int dstStep = _dst.Step();
  PtrStepSzb dst(_dst.Height(),_dst.Width(),static_cast<unsigned char*>(_dst.data()),dstStep);

  func(src,dst,m_pStream);
}

}
}
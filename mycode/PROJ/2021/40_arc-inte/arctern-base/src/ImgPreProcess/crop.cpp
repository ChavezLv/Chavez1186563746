//
// Created by Admin on 2021/11/1.
//

#include "crop.h"
#include "cuda/crop.cuh"
void arctern::cuda::crop(const DeviceBuffer &_src,
                         DeviceBuffer &_dst,
                          ArcternRect rROI,cudaStream_t *m_pStream) {
  if (_src.Width() <= 0 || _src.Height() <= 0 || _src.data() == nullptr) {
    std::cout << "Crop: error params" << endl;
    return;
  }
  if(rROI.width <= 0 || rROI.height <= 0 || rROI.x < 0 || rROI.y < 0 ||
     rROI.x + rROI.width > _src.Width() || rROI.y + rROI.height > _src.Height()){
    std::cout << "Crop: error params roi" << endl;
    return;
  }
  using func_t = void (*)(const PtrStepSzb &src, PtrStepSzb &dst,  ArcternRect rROI,cudaStream_t *);
  static const func_t funcs[4][3]={{device::crop<uchar>,0,device::crop<uchar3>},
                                   {0,0,0},
                                   {0,0,0},
                                   {device::crop<float>,0,device::crop<float3>}};
  auto func = funcs[_src.getElementSize() -1][_src.Channel() - 1];
  size_t srcStep = _src.Step();
  PtrStepSzb src(_src.Height(),_src.Width(),static_cast<unsigned char*>(const_cast<void *>(_src.data())),srcStep);
  size_t dstStep = _dst.Step();
  PtrStepSzb dst(_dst.Height(),_dst.Width(),static_cast<unsigned char *>(_dst.data()),dstStep);
  func(src,dst,rROI,m_pStream);
}
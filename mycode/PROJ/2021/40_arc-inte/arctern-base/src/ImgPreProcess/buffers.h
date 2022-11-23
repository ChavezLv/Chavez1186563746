//
// Created by Admin on 2019/6/15.
//

#ifndef ARCTERN_BASE_SRC_IMGPREPROCESS_BUFFERS_H_
#define ARCTERN_BASE_SRC_IMGPREPROCESS_BUFFERS_H_

#include "NvInfer.h"
#include <cassert>
#include <cuda_runtime_api.h>
#include <iostream>
#include <iterator>
#include <memory>
#include <new>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

namespace ImgPreprocessEx {

//!
//! \brief  The GenericBuffer class is a templated class for buffers.
//!
//! \details This templated RAII (Resource Acquisition Is Initialization) class handles the allocation,
//!          deallocation, querying of buffers on both the device and the host.
//!          It can handle data of arbitrary types because it stores byte buffers.
//!          The template parameters AllocFunc and FreeFunc are used for the
//!          allocation and deallocation of the buffer.
//!          AllocFunc must be a functor that takes in (void** ptr, size_t size)
//!          and returns bool. ptr is a pointer to where the allocated buffer address should be stored.
//!          size is the amount of memory in bytes to allocate.
//!          The boolean indicates whether or not the memory allocation was successful.
//!          FreeFunc must be a functor that takes in (void* ptr) and returns void.
//!          ptr is the allocated buffer address. It must work with nullptr input.
//!
template<typename AllocFunc, typename FreeFunc>
class GenericBuffer {
 public:

  //!
  //! \brief Construct an empty buffer.
  //!
  GenericBuffer(nvinfer1::DataType type = nvinfer1::DataType::kFLOAT)
      : mSize(0), mCapacity(0), mType(type), mBuffer(nullptr) {
    for(auto & item:mDims.d){
      item=0;
    }
  }

  //!
  //! \brief Construct a buffer with the specified allocation size in bytes.
  //!
  GenericBuffer(size_t size, nvinfer1::DataType type)
      : mSize(size), mCapacity(size), mType(type) {
    if (!allocFn(&mBuffer, this->nbBytes())) {
      throw std::bad_alloc();
    }
    for(auto & item:mDims.d){
      item=0;
    }
  }

  GenericBuffer(const size_t &height,
                const size_t &width,
                const size_t &channel,
                const nvinfer1::DataType &type,
                const size_t &step = 0,
                const size_t &batch = 1)
      : mType(type),mBuffer(nullptr) {
    resize(height,width,channel,type,step,batch);
  }

  GenericBuffer(GenericBuffer &&buf)
      : mSize(buf.mSize), mCapacity(buf.mCapacity), mType(buf.mType), mBuffer(buf.mBuffer), mDims(buf.mDims) {
    buf.mSize = 0;
    buf.mCapacity = 0;
    buf.mType = nvinfer1::DataType::kFLOAT;
    buf.mBuffer = nullptr;
  }

  //!
  //! \brief Returns pointer to underlying array.
  //!
  void *data() {
    return mBuffer;
  }

  //!
  //! \brief Returns pointer to underlying array.
  //!
  const void *data() const {
    return mBuffer;
  }

  //!
  //! \brief Returns the size (in number of elements) of the buffer.
  //!
  size_t size() const {
    return mSize;
  }

  //!
  //! \brief Returns the size (in bytes) of the buffer.
  //!
  size_t nbBytes() const {
    return this->size();
    //return this->size() * getElementSize();
  }

  //!
  //! \brief Resizes the buffer. This is a no-op if the new size is smaller than or equal to the current capacity.
  //!
  void resize(size_t newSize) {
    mSize = newSize;
    if (mCapacity < newSize) {
      freeFn(mBuffer);
      mBuffer = nullptr;
      if (!allocFn(&mBuffer, this->nbBytes())) {
        throw std::bad_alloc{};
      }
      mCapacity = newSize;
    }
  }

  void resize(const size_t &height,
              const size_t &width,
              const size_t &channel,
              const nvinfer1::DataType &type,
              const size_t &step = 0,
              const size_t &batch = 1) {

    mType = type;
    mDims.nbDims = 4;
    mDims.d[0] = batch;
    mDims.d[1] = channel;
    mDims.d[2] = height;
    mDims.d[3] = width;
    mStep = step == 0? channel * width * getElementSize(): step;
    return this->resize(volume());
  }

  int64_t volume() const {
    return mDims.d[0] * mDims.d[2] * mStep;
    //return std::accumulate(mDims.d, mDims.d + mDims.nbDims, 1, std::multiplies<int64_t>());
  }
  //!
  //! \brief Overload of resize that accepts Dims
  //!
  void resize(const nvinfer1::Dims &dims) {
    mDims = dims;
    return this->resize(volume());
  }

  ~GenericBuffer() {
    freeFn(mBuffer);
    mBuffer = nullptr;
  }

  unsigned int getElementSize() const{
    switch (mType) {
      case nvinfer1::DataType::kINT32: return 4;
      case nvinfer1::DataType::kFLOAT: return 4;
      case nvinfer1::DataType::kHALF: return 2;
    //  case nvinfer1::DataType::kBOOL:
      case nvinfer1::DataType::kINT8: return 1;
    }
    throw std::runtime_error("Invalid DataType.");
    return 0;
  }

  size_t Batch() const {
    return mDims.d[0];
  }

  size_t Channel() const{
    return mDims.d[1];
  }

  size_t Height() const{
    return mDims.d[2];
  }

  size_t Width() const{
    return mDims.d[3];
  }
  size_t Step() const {
    return mStep;
  }
  nvinfer1::DataType Type() const{
    return mType;
  }
 private:
  nvinfer1::Dims mDims;
  size_t mStep{0};
  size_t mSize{0}, mCapacity{0};
  nvinfer1::DataType mType;
  void *mBuffer = nullptr;
  AllocFunc allocFn;
  FreeFunc freeFn;
};

class DeviceAllocator {
 public:
  bool operator()(void **ptr, size_t size) const {
    return cudaMalloc(ptr, size) == cudaSuccess;
  }
};

class DeviceFree {
 public:
  void operator()(void *ptr) const {
    if(ptr){
      auto err = cudaFree(ptr);
      assert(err == cudaSuccess);
    }
  }
};


class HostAllocator {
 public:
  bool operator()(void **ptr, size_t size) const {
    *ptr = malloc(size);
    return *ptr != nullptr;
  }
};

class HostFree {
 public:
  void operator()(void *ptr) const {
    free(ptr);
  }
};

using DeviceBuffer = GenericBuffer<DeviceAllocator, DeviceFree>;
using HostBuffer = GenericBuffer<HostAllocator, HostFree>;
}
#endif //ARCTERN_BASE_SRC_IMGPREPROCESS_BUFFERS_H_

/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.25
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_COMMON_TENSOR_H_
#define SRC_COMMON_TENSOR_H_
#include "tensorShape.h"
#include <memory>
#include <vector>
#include <iostream>
#include "opencv2/core/core.hpp"

namespace arctern {

template <typename T>
class Tensor {
 public:
  Tensor() {}

  explicit Tensor(const TensorShape& shape) : shape_(shape) {
    if (size() > 0) {
      data_.reset(new T[size()]());
    }
  }

  virtual ~Tensor() {}

  uint size() const { return shape_.size(); }

  const TensorShape& shape() { return shape_; }

  Tensor& reshape(const TensorShape& shape) {
    uint oldSize = shape_.size();
    uint newSize = shape.size();

    shape_ = shape;

    if (oldSize != newSize) {
      data_.reset(new T[size()]());
    }

    return *this;
  }

  const T* data() const { return &(data_.get()[0]); }

  T* data() { return &(data_.get()[0]); }

  const TensorShape& shape() const { return shape_; }

  void from_cvmat(const cv::Mat& mat, bool bswap_channels = true) {
    if (bswap_channels) {
      shape_ = TensorShape(1, mat.channels(), mat.rows, mat.cols);
      data_.reset(new T[size()]());
      data_format_ = DataFormat::NCHW;
      swap_channels(mat);
    } else {
      shape_ = TensorShape(1, mat.rows, mat.cols, mat.channels());
      data_.reset(new T[size()]());
      data_format_ = DataFormat::NHWC;
      memcpy((uchar*)data_.get(), (uchar*)mat.data, size() * sizeof(T));
    }
  }

  void from_cvmat(const std::vector<cv::Mat>& mats,
          int batchNum, bool bswap_channels = true)
  {
    assert((batchNum > 0) && (mats.size() <= batchNum));

    const int n = batchNum;
    const int c = mats[0].channels();
    const int h = mats[0].rows;
    const int w = mats[0].cols;
    const int nitems = c * h * w;

    if (bswap_channels) {
      shape_ = TensorShape(n, c, h, w);
      data_.reset(new T[size()]());
      data_format_ = DataFormat::NCHW;

      int offset = 0;
      for (size_t i = 0; i < mats.size(); ++i) {
        swap_channels(mats[i], offset);
        offset += nitems;
      }
    } else {
      shape_ = TensorShape(n, h, w, c);
      data_.reset(new T[size()]());
      data_format_ = DataFormat::NHWC;

      T* pdata = data_.get();
      for (size_t i = 0; i < mats.size(); ++i) {
        memcpy((uchar*)pdata, (uchar*)mats[i].data, nitems * sizeof(T));
        pdata += nitems;
      }
    }

    assert(batchNum * c * h * w == (int)this->size());
  }

  void from_vector(const std::vector<T>& vec) {
    data_.reset(new T[vec.size()]());
    memcpy((uchar*)data_.get(), (uchar*)vec.data(), vec.size() * sizeof(T));
  }

 protected:
  void swap_channels(const cv::Mat& image) {
    int width = image.cols;
    int height = image.rows;
    int depth = image.depth();

#if defined(WITH_NNIE)
    assert(depth == CV_8U);
#elif defined(RKNN_ALLOWED)
    assert((depth == CV_8U) || (depth == CV_16U));
#else
    assert((depth == CV_32F) || (depth == CV_16U));
#endif

    T* pdata = data_.get();
    std::vector<cv::Mat> input_channels;

    for (int i = 0; i < image.channels(); ++i) {
      cv::Mat imgchannel(height, width, depth, pdata);
      input_channels.push_back(imgchannel);
      pdata += (height * width);
    }
    cv::split(image, input_channels);
  }


  void swap_channels(const cv::Mat& image, int offset) {
    int width = image.cols;
    int height = image.rows;
    int depth = image.depth();

#if defined(WITH_NNIE)
    assert(depth == CV_8U);
#elif defined(RKNN_ALLOWED)
    assert((depth == CV_8U) || (depth == CV_16U));
#else
    assert((depth == CV_32F) || (depth == CV_16U));
#endif

    T* pdata = data_.get() + offset;
    std::vector<cv::Mat> input_channels;

    for (int i = 0; i < image.channels(); ++i) {
      cv::Mat imgchannel(height, width, depth, pdata);
      input_channels.push_back(imgchannel);
      pdata += (height * width);
    }

    cv::split(image, input_channels);
  }

  TensorShape shape_;
  std::shared_ptr<T> data_;
  DataFormat data_format_ = DataFormat::NCHW;
};

using OutputShape=Shape<2>;

template<typename T>
class OutputTensor{
 private:
  OutputShape m_shape;
  std::shared_ptr<T> m_data;
 public:
  explicit OutputTensor(const OutputShape &shape):m_shape(shape){
    int len = size();
    if(size() > 0){
      m_data.reset(new T[size()]);
    }
  }
  ~OutputTensor()=default;
  const T* data() const{
    return &m_data.get()[0];
  }
  T* data() {
    return &m_data.get()[0];
  }
  size_t size(){
    return m_shape.size();
  }
  const OutputShape& shape() const{
    return m_shape;
  }
  OutputTensor& reshape(const OutputShape& shape){
    if(shape.size() != m_shape.size()){
      m_shape = shape;
      m_data.reset(new T[size()]);
    }
    memset(m_data.get(),0,size());
    return *this;
  }
};

}  // namespace arctern
#endif  // SRC_COMMON_TENSOR_H_

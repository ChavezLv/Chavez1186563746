/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         Wenqi Ju
 *  Last modified:  2020.12.05
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/
#ifndef ARCTERN_BASE_COMMON_TENSOR5_H_
#define ARCTERN_BASE_COMMON_TENSOR5_H_
#include <fstream>
#include "./src/log/logger.h"
#include "tensor.h"

namespace arctern {
class TensorShape5 {
 public:
  TensorShape5() {
    kdims = 0;
    val = nullptr;
  }
  TensorShape5(int dimNum, int *value) : kdims(dimNum)
  {
    if (dimNum <= 0 || value == nullptr) {
      SPDLOG_ERROR("error !! dim <= 0 or input pointer is nullptr when init TensorShape");
      kdims = 0;
      return;
    }
    val = new int[dimNum];
    for (int i = 0; i < dimNum; i++) {
      val[i] = value[i];
    }
  }

  ~TensorShape5() {Release();}
  void Release() {
    if (val != nullptr) {
      delete[] val;
      val = nullptr;
    }
    kdims = 0;
  }

  explicit TensorShape5(const TensorShape5& ts5) {
    if (ts5.kdims > 0) {
      kdims = ts5.kdims;
      val = new int[kdims];
      for (int i = 0; i < kdims; i++) {
        val[i] = ts5.val[i];
      }
    }
  }
  TensorShape5 &operator = (const TensorShape5& ts5) {
    if(this == &ts5) return *this;
    Release();
    if (ts5.kdims > 0) {
      kdims = ts5.kdims;
      val = new int[kdims];
      for (int i = 0; i < kdims; i++) {
        val[i] = ts5.val[i];
      }
    }
    return *this;
  }
  int reshape(int dimNum, int *value) {
    Release();
    if (dimNum <= 0 || value == nullptr) {
      SPDLOG_ERROR("error !! dim <= 0 or input pointer is nullptr when init TensorShape");
      kdims = 0;
      return -1;
    }
    kdims = dimNum;
    val = new int[dimNum];
    for (int i = 0; i < dimNum; i++) {
      val[i] = value[i];
    }
    return 0;
  }
  void Print() {
    for (int i = 0; i < kdims; i++) {
      printf("val i %d = %d\n", i ,val[i]);
    }
  }
  int &operator[](int i) { return val[i]; }

  int operator[](int i) const { return val[i]; }

  int num_dims() const { return kdims; }

  bool operator == (const TensorShape5 &rhs) {
    if (num_dims() != rhs.num_dims()) return false;
    int i = 0;
    for (; i < num_dims() && (val[i] == rhs.val[i]); ++i) ;
    return (i == num_dims());
  }

  uint size() const {
      if (kdims <= 0) return 0;
      int totalSize = val[0];
      for (int i = 1; i < kdims; ++i) {
         totalSize *= val[i];
      }
      return totalSize;
   }

  friend inline std::ostream &operator<<(std::ostream &os, const TensorShape5 &shape);

 protected:
  int *val = nullptr;
  int kdims = 0;  // available dimensions
};


template <typename T>
class Tensor5 {
 public:
  std::shared_ptr<T> data_;
  TensorShape5 shape_;
  DataFormat data_format_ = DataFormat::NTHWC;
  int nused_dims_ = 0;

  uint size() const { return shape_.size(); }
  const T* data() const { return &(data_.get()[0]); }

  T* data() { return &(data_.get()[0]); }
 public:
  Tensor5() = default;
  ~Tensor5() = default;
  Tensor5(const Tensor5 &t5) {
     if(t5.size() > 0) {
       data_.reset(new T());
       shape_ = t5.shape_;
       data_format_ = t5.data_format_;
       nused_dims_ = t5.nused_dims_;
       memcpy((uchar*)data_.get(), (uchar*)t5.data(), size() * sizeof(T));
     } else {
       data_.reset();
       nused_dims_ = 0;
     }
  }

  Tensor5 &operator = (const Tensor5 &t5) {
    if(t5.size() >= 0) {
      data_.reset(new T());
      shape_ = t5.shape_;
      data_format_ = t5.data_format_;
      nused_dims_ = t5.nused_dims_;
      memcpy((uchar*)data_.get(), (uchar*)t5.data(), size() * sizeof(T));
    } else {
      data_.reset(nullptr);
      nused_dims_ = 0;
    }
  }

  const TensorShape5& shape() { return shape_; }

  void reshape(const TensorShape5& shape) {
    shape_ = shape;
    //printf("size = %d, dara_=%d", size(), data_);
    data_.reset(new T[size()]);
  }


  virtual void from_cvmat(const cv::Mat& mat, bool bswap_channels = true) {
  }

  //swap NTHWC => NCTHW
  virtual void from_cvmat(const std::vector<cv::Mat>& mats, const TensorShape5 &oriMatShape,
                   DataFormat swapDataFormat, bool bswap_channels = true) {
    if (mats.size() <= 0) return;
    int batchNum = oriMatShape[0];
    int sequenceNum = oriMatShape[1];
    // printf("shape_.num_dims()=%d, %d, %d\n ", shape_.num_dims(),(int)data_format_ ,(int) DataFormat::NTHWC);

    assert(shape_.num_dims() == 5 && DataFormat::NTHWC == data_format_);
    assert(batchNum * sequenceNum >= mats.size());
    const int c = mats[0].channels();
    const int h = mats[0].rows;
    const int w = mats[0].cols;
    int startoffset = 0;

    if (bswap_channels) {
      // std::vector<int> shapenumbers = {batchNum, c, sequenceNum, h, w};
      // data_format_ = DataFormat::NCTHW;
      // shape_ = TensorShape5(5, shapenumbers);
      std::vector<int> shapenumbers;
      shapenumbers.resize(oriMatShape.num_dims());
      for (size_t i = 0; i < shapenumbers.size(); ++i) {
        shapenumbers[i] = oriMatShape[i];
      }
      shapenumbers[1] = oriMatShape[shapenumbers.size() - 1];
      for (size_t i = 2; i < shapenumbers.size(); ++i) {
        shapenumbers[i] = oriMatShape[i-1];
      }
      shape_.reshape(shapenumbers.size(), &(shapenumbers[0]));
      // shape_.Print();
      data_format_ = swapDataFormat;
      data_.reset(new T[size()]);

      int strideForOnlyImage = h * w;
      int channelStepForSequence = h * w * sequenceNum;
      // int batchChannelStep = channelStep * ;
      int offsetStrideForSequenceChennel = h * w * sequenceNum * c;
      // int offsetStrideForABatch = h * w * sequenceNum * c * batchNum;
      // int startAddrForABatch = 0;
      int imageIdx = 0;
      int startoffsetStrideForSequenceChennel = 0;
      // int matsSize = mats.size();
      for (int k = 0; k < batchNum; ++k) {
        startoffset = startoffsetStrideForSequenceChennel;
        for (int i = 0; i < sequenceNum; ++i) {
          swap_channels(mats[imageIdx], startoffset, channelStepForSequence);
          startoffset += strideForOnlyImage;
          ++imageIdx;
        }
        startoffsetStrideForSequenceChennel += offsetStrideForSequenceChennel;
      }
      ///////////////////////////////////
//      for (int i = 0; i < 100; ++i) {
//        T* pointer = (T*)mats[0].data;
//        std::cout << "--000-------test  data i=" << i << "; imgdata="<<(T) pointer[i] << std::endl;
//      }
//      for (int i = 0; i < size(); ++i) {
//        T* pointer = (T*)data_.get();
//        std::cout << "--001-------test  data i=" << i << "; data="<<(T) pointer[i] << std::endl;
//      }
    } else {
      int nitems = c * h * w;
      // std::vector<int> shapenumbers = {batchNum, sequenceNum, h, w, c};
      // data_format_ = DataFormat::NTHWC;
      // shape_ = TensorShape5(5, shapenumbers);
      shape_ = oriMatShape;
      data_.reset(new T[size()]);
      data_format_ = DataFormat::NTHWC;
      T* pdata = data_.get();
      for (size_t i = 0; i < mats.size(); ++i) {
        memcpy((uchar*)pdata, (uchar*)mats[i].data, nitems * sizeof(T));
        pdata += nitems;
      }
    }
  }

  //swap NTHWC => NCTHW
  virtual void from_cvmat_slow(const std::vector<cv::Mat>& mats, const TensorShape5 &oriMatShape,
            int oriDimNum, int dstDimNum) {
//    if (mats.size() <= 0) return;
//    int batchNum = oriMatShape[0];
//    int sequenceNum = oriMatShape[1];
//    // printf("shape_.num_dims()=%d, %d, %d\n ", shape_.num_dims(),(int)data_format_ ,(int) DataFormat::NTHWC);
//
//    assert(shape_.num_dims() == 5 && DataFormat::NTHWC == data_format_);
//    const int c = mats[0].channels();
//    const int h = mats[0].rows;
//    const int w = mats[0].cols;
//    int startoffset = 0;
//    size_t imgSize = mats.size();
//    T* oridata = new T[imgSize * c * h * w];
//    T* startAdd = oridata;
//    int nitemsSize = c * h * w * sizeof(T);
//    int nitems = c * h * w;
//    data_.reset(new T[size()]);
//    for (size_t i = 0; i < imgSize; ++i) {
//      memcpy((uchar*)startAdd, (uchar*)mats[i].data, nitemsSize);
//      startAdd += nitems;
//    }
//
//    for (size_t i = 0; i < imgSize; ++i) {
//      memcpy((uchar*)startAdd, (uchar*)mats[i].data, nitemsSize);
//      startAdd += nitems;
//    }
//    delete[] oridata;
  }


//swap NTHWC => NCTHW
  virtual void from_cvmat(const std::vector<cv::Mat>& mats, const TensorShape5 &oriMatShape,
                          DataFormat swapDataFormat, int swapChannelToWhichDim) {

    if (mats.size() <= 0) return;
    // int batchNum = oriMatShape[0];
    // int sequenceNum = oriMatShape[1];
    // printf("shape_.num_dims()=%d, %d, %d\n ", shape_.num_dims(),(int)data_format_ ,(int) DataFormat::NTHWC);

    assert(shape_.num_dims() == 5 && DataFormat::NTHWC == data_format_);
    assert(oriMatShape[0] * oriMatShape[1] >= mats.size());
    const int c = mats[0].channels();
    const int h = mats[0].rows;
    const int w = mats[0].cols;
    int startoffset = 0;
    int upValidNum = swapChannelToWhichDim <= shape_.num_dims() - 2;
    if (swapChannelToWhichDim >= 0 && swapChannelToWhichDim <= upValidNum) {
      // std::vector<int> shapenumbers = {batchNum, c, sequenceNum, h, w};
      // data_format_ = DataFormat::NCTHW;
      // shape_ = TensorShape5(5, shapenumbers);
      std::vector<int> shapenumbers;
      shapenumbers.resize(oriMatShape.num_dims());
      for (size_t i = 0; i < shapenumbers.size(); ++i) {
        shapenumbers[i] = oriMatShape[i];
      }
      shapenumbers[1] = oriMatShape[shapenumbers.size() - 1];
      for (size_t i = 2; i < shapenumbers.size(); ++i) {
        shapenumbers[i] = oriMatShape[i-1];
      }
      data_format_ = swapDataFormat;
      data_.reset(new T[size()]);

     // printf("---------------0001\n");
      int strideForOnlyImage = h * w;
      // int channelStepForSequence = h * w * sequenceNum;
      int channelStepForSequence = 1;
      // int batchChannelStep = channelStep * ;
      // int offsetStrideForSequenceChennel = h * w * sequenceNum * c;
      int offsetStrideForSequenceChennel = 1;
      // int offsetStrideForABatch = h * w * sequenceNum * c * batchNum;
      // int startAddrForABatch = 0;
      int imageIdx = 0;
      int startoffsetStrideForSequenceChannel = 0;
      // int matsSize = mats.size();
      int totalBeforeCNum = 0;
      int totalAfterCNum = 0;
      for (int k = 0; k < swapChannelToWhichDim; ++k) {
        totalBeforeCNum += shape_[k];
      }
      for (int k = shape_.num_dims() - 4; k >= swapChannelToWhichDim; --k) {
        totalAfterCNum += shape_[k];
      }
      for (int k = shape_.num_dims() - 2; k >= swapChannelToWhichDim; --k) {
        channelStepForSequence *= shape_[k];
      }
      for (int k = shape_.num_dims() - 1; k >= swapChannelToWhichDim; --k) {
        offsetStrideForSequenceChennel *= shape_[k];
      }
      printf("-------%d, %d--------0002\n", channelStepForSequence, offsetStrideForSequenceChennel);
      for (int k = 0; k < totalBeforeCNum; ++k) {
        startoffset = startoffsetStrideForSequenceChannel;
        for (int i = 0; i < totalAfterCNum; ++i) {
          swap_channels(mats[imageIdx], startoffset, channelStepForSequence);
          startoffset += strideForOnlyImage;
          ++imageIdx;
        }
        startoffsetStrideForSequenceChannel += offsetStrideForSequenceChennel;
      }
      printf("---------------0003\n");
      shape_.reshape(5, &(shapenumbers[0]));
      ///////////////////////////////////
      /* for (int i = 0; i < size(); ++i) {
         std::cout << "---------test  data i=" << i << "; data="<<(int) data_.get()[i] << std::endl;
       }*/

    } else if (swapChannelToWhichDim < 0) {
      int nitems = c * h * w;
      // std::vector<int> shapenumbers = {batchNum, sequenceNum, h, w, c};
      // data_format_ = DataFormat::NTHWC;
      // shape_ = TensorShape5(5, shapenumbers);
      shape_ = oriMatShape;
      data_.reset(new T[size()]);
      data_format_ = DataFormat::NTHWC;
      T* pdata = data_.get();
      for (size_t i = 0; i < mats.size(); ++i) {
        memcpy((uchar*)pdata, (uchar*)mats[i].data, nitems * sizeof(T));
        pdata += nitems;
      }
    } else if (swapChannelToWhichDim > upValidNum) {
      SPDLOG_ERROR("error !! not support the swap operation!!!");
    }
  }

  void TestReadFile() {
    std::ifstream file;
    file.open("../gulinsong_data/tensor.data", std::ios::in | std::ios::binary);
    if (!file) {
      std::cout << "---------error! cannot open file!!" << std::endl;
      return;
    }
    data_.reset(new T[size()]);
    int s = size();
    int floatNum = 0;
    T *pointer = data_.get();
    float test;
    while(!file.eof()) {
      if(floatNum < size()) {
        file.read((char *) pointer, sizeof(T));
        // printf("-----read:i=%d, %f\n", floatNum, (float)*pointer);
        pointer++;
      }
      else {
        printf("lastone: %f\n", *(pointer-1));
        file.read((char *) &test, sizeof(T));
        printf("-------------error!!! out of memory!! v= %f ,i = %d vs size=%d\n",
               test, floatNum, size());
      }
      floatNum++;
    }
  }

 protected:
  //note the size of the image must be the same.

  virtual void swap_channels(const cv::Mat& image) {
    std::vector<cv::Mat> input_channels;
    int width = image.cols;
    int height = image.rows;

    // int buf_type = 0;
    int depth = image.depth();
#ifndef WITH_NNIE
    assert(depth == CV_32F || depth == CV_16S);
#endif    // WITH_NNIE

    T* pdata = data_.get();
    for (int i = 0; i < image.channels(); ++i) {
      cv::Mat imgchannel(height, width, depth, pdata);
      input_channels.push_back(imgchannel);
      pdata += height * width;
    }

    cv::split(image, input_channels);
  }


  virtual void swap_channels(const cv::Mat& image, int offset, int stepForSequece) {
    std::vector<cv::Mat> input_channels;

    int width = image.cols;
    int height = image.rows;

    T *pdata = data_.get() + offset;
    for (int i = 0; i < image.channels(); ++i) {
#ifndef WITH_NNIE
      cv::Mat imgchannel(height, width, CV_32F, pdata);
#else
      cv::Mat imgchannel(height, width, CV_8U, pdata);
#endif
      input_channels.push_back(imgchannel);
      pdata += stepForSequece;
    }
    //printf(" input_channels add= %d, %d, sizeof=%d, stepForSequece=%d size=%d\n",
    //    input_channels[0].data, data_.get(), sizeof(T),stepForSequece, input_channels.size());
    cv::split(image, input_channels);
    /////////////  test ///////////////////
   // printf(" input_channels add= %d, %d, sizeof=%d \n", input_channels[0].data, data_.get(), sizeof(T));
//    for (int i = 0; i < input_channels.size(); ++i) {
//      uchar *d = input_channels[i].data;
//      T *pdatatest = data_.get();
//      T *pdatatest01;
//      //std::cout << "----------addr = "<< d  << "vs data add = " << input_channels[i].data << "; " <<sizeof(T)< std::endl;
//     // printf("addr=%d, data add= %d, %d, sizeof=%d \n", d, input_channels[i].data, data_.get(), sizeof(T));
//      int idx = 0;
//      for (int j = 0; j < width * height; ++j) {
//        std::cout << "i=" << i << "; j=" << j << "; d = " <<(int) d[idx++] << std::endl;
//      }
//    }
//
//    for (int i = 0; i < size(); ++i) {
//      std::cout << "---------test  data i=" << i << "; data="<<(int) data_.get()[i] << std::endl;
//    }

  }
};
}  // namespace arctern
#endif  // ARCTERN_BASE_COMMON_TENSOR5_H_

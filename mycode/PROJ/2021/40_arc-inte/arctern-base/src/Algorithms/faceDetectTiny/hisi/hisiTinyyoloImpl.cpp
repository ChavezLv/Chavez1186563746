/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.23
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include <arm_neon.h>
#include "hisiTinyyoloImpl.h"
#include "src/Processors/Algorithms/detpreprocess.h"

#ifdef __USE_NEON_RESIZE__
#include "neon/ne10_resize.h"
#endif // ifdef __USE_NEON_RESIZE__

using arctern::HisiYoloV3;
using arctern::Tensor;
using arctern::TensorShape;

#include "src/common/neonmathfun.h"


float sigmoid(float x) {
  return 1.0 / (1.0 + exp(-x));
}

HisiYoloV3::HisiYoloV3() {
  // hisi is different
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_BGR888;
}

HisiYoloV3::~HisiYoloV3() throw() {

}

void HisiYoloV3::initPreProcessor() {
  // init pre processor
  DetPreProInitParameter initParam;
  initParam.netH_ = input_h_;
  initParam.netW_ = input_w_;
  initParam.mean_ = 0;
  initParam.std_ = 255.0;
  initParam.alignMent_ = AlignMent::AlignCenter;
  initParam.detType_ = DetType::Face;
  initParam.borderValue_ = 128;
  preProcessor_.reset(new DetPreProcesser(false));
  preProcessor_->Init(&initParam);
}

Tensor<float> HisiYoloV3::transpose(const Tensor<float> &input_tensor) {
  TensorShape input_tensor_shape = input_tensor.shape();
  const float *input_ptr = input_tensor.data();
  int n = input_tensor_shape[0];
  int c = input_tensor_shape[1];
  int h = input_tensor_shape[2];
  int w = input_tensor_shape[3];

  Tensor<float> output_tensor(TensorShape(n, c, w, h));
  float *output_ptr = output_tensor.data();
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < c; ++j) {
      for (int p = 0; p < w; ++p) {
        for (int q = 0; q < h; ++q) {
          int input_index = i * c * w * h + j * w * h + q * w + p;
          int output_index = i * c * w * h + j * w * h + p * h + q;
          output_ptr[output_index] = input_ptr[input_index];
        }
      }
    }
  }
  return output_tensor;
}

Tensor<float> reshape(const Tensor<float> &input_tensor, int new_h, int new_w) {
  TensorShape input_tensor_shape = input_tensor.shape();
  const float *input_ptr = input_tensor.data();
  int n = input_tensor_shape[0];
  int c = input_tensor_shape[1];

  Tensor<float> output_tensor(TensorShape(n, c, new_h, new_w));
  float *output_ptr = output_tensor.data();
  memcpy(output_ptr, input_ptr, n * c * new_h * new_w * sizeof(float));
  return output_tensor;
}

Tensor<float> HisiYoloV3::NNIE_custom_layers(const std::vector<Tensor<float>> &outputs) {
  Tensor<float> output0 = transpose(outputs[0]);
  Tensor<float> output1 = transpose(outputs[1]);
  Tensor<float> output2;
  if (nstrides_ == 3) {
    output2 = transpose(outputs[2]);
  }

  int n = output0.shape()[0];
  int output_dim = input_dim_ / 32;
  int output_dim2 = input_dim2_ / 32;
  int output_pixels = output_dim * output_dim2;
  int w = num_classes_ + 5;
//reshape
  Tensor<float> output0_ = reshape(output0, output_pixels * 3, w);
  Tensor<float> output1_ = reshape(output1, output_pixels * 12, w);
  Tensor<float> output2_;
  if (nstrides_ == 3) {
    output2_ = reshape(output2, output_pixels * 48, w);
  } // yolo

//concat
  int tmpConcat = 0;
  if (nstrides_ == 3) {
    tmpConcat = 63;
  } else { // yolo
    tmpConcat = 15;
  } // tiny-yolo

  Tensor<float> output_tensor(TensorShape(n, 1, tmpConcat * output_pixels, w));
  float *output_data = output_tensor.data();
  const float *yoloLayer_0 = output0_.data();
  const float *yoloLayer_1 = output1_.data();
  float *yoloLayer_2 = nullptr;
  if (nstrides_ == 3) {
    yoloLayer_2 = output2_.data();
  } // yolo

  for (int i = 0; i < n; ++i) {
    memcpy(output_data, yoloLayer_0, 3 * w * output_pixels * sizeof(float));
    output_data += 3 * w * output_pixels;
    yoloLayer_0 += 3 * w * output_pixels;

    memcpy(output_data, yoloLayer_1, 12 * w * output_pixels * sizeof(float));
    output_data += 12 * w * output_pixels;
    yoloLayer_1 += 12 * w * output_pixels;

    if (nstrides_ == 3) { // yolo
      memcpy(output_data, yoloLayer_2, 48 * w * output_pixels * sizeof(float));
      output_data += 48 * w * output_pixels;
      yoloLayer_2 += 48 * w * output_pixels;
    }
  }

//sigmoid
  int h = 0;
  if (nstrides_ == 3) { // yolo
    h = 63 * output_pixels;
  } else { // tiny-yolo
    h = 15 * output_pixels;
  }
  float *prediction = output_tensor.data();
  //std::cout<<" prediction = "<<output_tensor.size()<<std::endl;
  //printf("n = %d, h = %d, w = %d\n", n, h, w);
  //double start = cv::getTickCount();
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < h; ++j) {
      int idx = i * h * w + j * w;
      float32x4_t a = {prediction[idx], prediction[idx + 1], prediction[idx + 4], prediction[idx + 5]};
      float32x4_t b = sigmoid_ps(a);
      prediction[idx] = b[0];
      prediction[idx + 1] = b[1];
      prediction[idx + 4] = b[2];
      prediction[idx + 5] = b[3];
    }
  }
//  double end = cv::getTickCount();
//  std::cout<<"run time = "<<(end - start)/cv::getTickFrequency() * 1000<<"ms"<<std::endl;
  return output_tensor;
}

int HisiYoloV3::forward(const Tensor<uint8> &input_tensor, std::vector<arctern::Tensor<float> > &outputs) {
  std::vector<arctern::Tensor<float> > tmp;
  engins_[0]->forward(input_tensor, tmp);
  auto outputs_NNIE = NNIE_custom_layers(tmp);
  outputs.push_back(outputs_NNIE);
  return 0;
}

arctern::ErrorType HisiYoloV3::Init(const InitParameter *p) {
  // hisi not support batch
  if (nullptr != p) {
    p->batchNum_ = 1;
  }

  return YoloV3::Init(p);
}

/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         fanghuangcai
 *  Last modified:  2020.11.26
 *  email:          fang.huangcai@intellif.com
 ****************************************************************************/
#include "rknnFaceDetectTinyImpl.h"

#include <arm_neon.h>
#ifdef __USE_NEON_RESIZE__
#include "neon/ne10_resize.h"
#endif // ifdef __USE_NEON_RESIZE__
#include "src/common/neonmathfun.h"

using arctern::RknnFaceDetectTinyImpl;
using arctern::Tensor;
using arctern::TensorShape;
using arctern::ErrorType;
//using arctern::RknnYoloV3;

float sigmoid(float x) {
  return 1.0 / (1.0 + exp(-x));
}

RknnFaceDetectTinyImpl::RknnFaceDetectTinyImpl() {
  m_bNormalized = false;
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_BGR888;
}

RknnFaceDetectTinyImpl::~RknnFaceDetectTinyImpl() noexcept {
}

arctern::ErrorType RknnFaceDetectTinyImpl::Init(const InitParameter *p) {
  // rknn not support batch
  assert(p->batchNum_ == 1);

  return faceDetectTinyImpl::Init(p);
}

Tensor<float> RknnFaceDetectTinyImpl::transpose(const Tensor<float> &input_tensor) {
  const TensorShape &input_tensor_shape = input_tensor.shape();

  int n = input_tensor_shape[0];
  int c = input_tensor_shape[1];
  int h = input_tensor_shape[2];
  int w = input_tensor_shape[3];

  // transpose (2, 3, 0, 1)
  Tensor<float> output_tensor(TensorShape(h, w, n, c));
  float *output_ptr = output_tensor.data();
  const float *input_ptr = input_tensor.data();

  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      for (int p = 0; p < n; ++p) {
        for (int q = 0; q < c; ++q) {
          int input_index = (p * c * h * w) + (q * h * w) + (i * w) + j;
          int output_index = (i * w * n * c) + (j * n * c) + (p * c) + q;
          output_ptr[output_index] = input_ptr[input_index];
        }
      }
    }
  }

  return output_tensor;
}

Tensor<float> RknnFaceDetectTinyImpl::process_output_layers(std::vector<Tensor<float>> &outputs) {
  int n = 3;
  int c = 6;

  int input_dim = yolo_v_3_ ? yolo_v_3_->input_dim_ : 0;
  int input_dim2 = yolo_v_3_ ? yolo_v_3_->input_dim2_ : 0;
  int nstrides = yolo_v_3_ ? yolo_v_3_->nstrides_ : 0;

  int grid0_w = (input_dim / 32);
  int grid0_h = (input_dim2 / 32);

  int grid1_w = (grid0_w * 2);
  int grid1_h = (grid0_h * 2);

  int grid2_w = (grid1_w * 2);
  int grid2_h = (grid1_h * 2);

  // transpose
  Tensor<float> output0 = transpose(
      outputs[0].reshape(TensorShape(n, c, grid0_h, grid0_w)));
  Tensor<float> output1 = transpose(
      outputs[1].reshape(TensorShape(n, c, grid1_h, grid1_w)));
  Tensor<float> output2;
  if (3 == nstrides) { // yolo
    output2 = transpose(outputs[2].reshape(TensorShape(n, c, grid2_h, grid2_w)));
  }

  // concat
  int w = c /* num_classes_ + 5 */;
  int h = (grid0_h * grid0_w) + (grid1_h * grid1_w);
  if (3 == nstrides) { // yolo
    h += (grid2_h * grid2_w);
  }

  h *= n;
  Tensor<float> output_tensor(TensorShape(1, 1, h, w));
  float *output_data = output_tensor.data();
  const float *yoloLayer_0 = output0.data();
  const float *yoloLayer_1 = output1.data();
  const float *yoloLayer_2 = nullptr;
  if (3 == nstrides) { // yolo
    yoloLayer_2 = output2.data();
  }

  memcpy(output_data, yoloLayer_0, output0.size() * sizeof(float));
  output_data += output0.size();

  memcpy(output_data, yoloLayer_1, output1.size() * sizeof(float));
  output_data += output1.size();

  if (3 == nstrides) { // yolo
    memcpy(output_data, yoloLayer_2, output2.size() * sizeof(float));
    output_data += output2.size();
  }

  //sigmoid
  float *prediction = output_tensor.data();

  for (int i = 0; i < h; ++i) {
    int idx = (i * w);
    float32x4_t a = {
        prediction[idx], prediction[idx + 1],
        prediction[idx + 4], prediction[idx + 5]
    };
    float32x4_t b = sigmoid_ps(a);

    prediction[idx] = b[0];
    prediction[idx + 1] = b[1];
    prediction[idx + 4] = b[2];
    prediction[idx + 5] = b[3];
  }

  return output_tensor;
}

int RknnFaceDetectTinyImpl::forward(const Tensor<uint8> &input_tensor, std::vector<arctern::Tensor<float>> &outputs) {
  std::vector<arctern::Tensor<float> > tmp;
  engines_[0]->forward(input_tensor, tmp);
  auto outputs_NNIE = process_output_layers(tmp);
  outputs.push_back(outputs_NNIE);
  return 0;
}


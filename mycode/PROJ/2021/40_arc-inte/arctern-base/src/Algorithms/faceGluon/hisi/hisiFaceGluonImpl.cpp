/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.11.2
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "hisiFaceGluonImpl.h"
#include "src/Engin/src/hisi/hisiengin.h"
#include "Processors/Algorithms/attrpreprocess.h"
#include "Processors/Algorithms/attrpreproRunParameter.h"
using arctern::HisiFaceGluonImpl;
using arctern::ErrorType;

HisiFaceGluonImpl::HisiFaceGluonImpl() {
  isNormaliized_ = false;
  // hisi is different
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_BGR888;
}

HisiFaceGluonImpl::~HisiFaceGluonImpl() {
}

ErrorType HisiFaceGluonImpl::Init(const InitParameter *p) {
  // hisi not support batch
  if (nullptr != p) {
    p->batchNum_ = 1;
  }
  return FaceGluonImpl::Init(p);
}

// out image blob, original image blob, STN params
void HisiFaceGluonImpl::STNProcessing(Tensor<uint8_t> &output, Tensor<uint8_t> &input, Tensor<float> &theta_) {
  float *theta = theta_.data();
  uint8_t *_out = output.data();
  uint8_t *_in = input.data();
  TensorShape input_shape = input.shape();
  TensorShape output_shape = output.shape();

  const int oheight = output_shape[2];
  const int owidth = output_shape[3];

  const int inchan = input_shape[1];
  const int inheight = input_shape[2];
  const int inwidth = input_shape[3];
  float Matrix[6];
  float fx, fy, d0, d1, d2;
  int sx, sy;
  float s0, s1, s2, s3;
  int src_ich_off = inheight * inwidth;
  int dst_ich_off = oheight * owidth;

  for (int i = 0; i < 6; i++) {
    Matrix[i] = *(theta + i);
  }
  Matrix[2] = (Matrix[2] + 1 - Matrix[0] - Matrix[1]) * (inwidth - 1) * 0.5;
  Matrix[5] = (Matrix[5] + 1 - Matrix[3] - Matrix[4]) * (inheight - 1) * 0.5;
  Matrix[0] = Matrix[0] * (inwidth - 1) / (owidth - 1);
  Matrix[1] = Matrix[1] * (inwidth - 1) / (oheight - 1);
  Matrix[3] = Matrix[3] * (inheight - 1) / (owidth - 1);
  Matrix[4] = Matrix[4] * (inheight - 1) / (oheight - 1);

  for (int i = 0; i < inchan; i++) {
    uint8_t *src_data = _in + i * src_ich_off;
    uint8_t *dst_data = _out + i * dst_ich_off;

    for (int j = 0; j < oheight; j++) {
      for (int k = 0; k < owidth; k++) {
        s0 = 0;
        s1 = 0;
        s2 = 0;
        s3 = 0;
        fx = k * Matrix[0] + j * Matrix[1] + Matrix[2];
        fy = k * Matrix[3] + j * Matrix[4] + Matrix[5];

        if (fx < 0)
          sx = (int) (fx - 1);
        else
          sx = (int) (fx);

        if (fy < 0)
          sy = (int) (fy - 1);
        else
          sy = (int) (fy);

        fx = fx - sx;
        fy = fy - sy;

        if (sx >= 0 && sx < inwidth && sy >= 0 && sy < inheight)
          s0 = (float(src_data[sy * inwidth + sx]) - 127.5) / 128.0;
        if (sx + 1 >= 0 && sx + 1 < inwidth && sy >= 0 && sy < inheight)
          s1 = (float(src_data[sy * inwidth + sx + 1]) - 127.5) / 128.0;
        if (sx >= 0 && sx < inwidth && sy + 1 >= 0 && sy + 1 < inheight)
          s2 = (float(src_data[(sy + 1) * inwidth + sx]) - 127.5) / 128.0;
        if (sx + 1 >= 0 && sx + 1 < inwidth && sy + 1 >= 0 && sy + 1 < inheight)
          s3 = (float(src_data[(sy + 1) * inwidth + sx + 1]) - 127.5) / 128.0;

        d0 = s0 * (1 - fx) + s1 * fx;
        d1 = s2 * (1 - fx) + s3 * fx;
        d2 = d0 * (1 - fy) + d1 * fy;
        d2 = d2 * 128.0 + 127.5;

        int res = (d2 > 0) ? int(d2 + 0.5) : 0;
        res = (res < 256) ? res : 255;
        dst_data[j * owidth + k] = (uint8_t) res;
      }
    }
  }
}

ErrorType HisiFaceGluonImpl::CommonProProcess(const RunParameter *p) {
  ErrorType ret = ErrorType::ERR_SUCCESS;
  this->result_->resize(p->imageV_.size());
  int imagesNum = p->imageV_.size();
  int startId = 0;
  const int batchNum = initParam_.batchNum_;
  int endId = startId + initParam_.batchNum_;
  endId = endId >= imagesNum ? imagesNum : endId;
  while (startId < imagesNum) {
    int processImageNum = endId - startId;
    std::vector<cv::Mat> preprocessed_imgs(processImageNum);
    // std::vector<cv::Mat> preprocessed_imgs;
    auto it = preprocessed_imgs_.begin();

    preprocessed_imgs.assign(it + startId, it + startId + processImageNum);

    std::vector<Tensor<float>> outputs1;
    std::vector<Tensor<float>> outputs2;

    // for hisi
    Tensor<uint8> input_tensor;
    input_tensor.from_cvmat(preprocessed_imgs, batchNum, swapChannel());

    engines_[0]->forward(input_tensor, outputs1);

    Tensor<uint8> input_tensor2;
    input_tensor2.from_cvmat(preprocessed_imgs, batchNum, swapChannel());

    STNProcessing(input_tensor2, input_tensor, outputs1[2]);

    engines_[1]->forward(input_tensor2, outputs2);


    std::vector<Tensor<float>> outputs;
    outputs.push_back(outputs2[outputs2.size() - 1]);

    LocalPostProcess(p, startId, endId, outputs);

    startId = endId;
    endId += batchNum;
    endId = endId > imagesNum ? imagesNum : endId;
  }

  return ret;
}

cv::Mat HisiFaceGluonImpl::LocalPreProcess(const cv::Mat &image, const RunParameter *p, int idx){
  const auto implRunP = dynamic_cast<const FaceGluonRunParameter *>(p);
  AttrPreProRunParameter runParameter;

  if (needLandmark_) {
    // hisi different ,so dirty code. hehe
    cv::Mat fimg;
    image.convertTo(fimg, cv::DataType<float>::type);
    runParameter.iMat_ = fimg;
    auto &landmarks = implRunP->faceLandmarksV_[idx];
    runParameter.lmk_ = calcAlignFacePoints(image.cols, image.rows, landmarks);
  } else {
    auto &arctRect = implRunP->rectsV_[idx];
    const cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);
    cv::Rect face = getFace(image.cols, image.rows, oriRect, exthScale, extwScale);
    cv::Mat cropImage = image(face);
    runParameter.iMat_ = cropImage;
  }

  auto result = preProcessor_->Process(&runParameter);
  assert(result.error_type_ == ErrorType::ERR_SUCCESS);

  if(needLandmark_) {
    result.oMat_.convertTo(result.oMat_, cv::DataType<uint8_t>::type, 1.0, 0.5); // uint_8(round(wipe_face))=uint_8(wipe_face+0.5)
  }

  return result.oMat_;
}

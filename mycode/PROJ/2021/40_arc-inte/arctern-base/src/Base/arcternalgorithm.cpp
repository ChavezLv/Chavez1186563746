/****************************************************************************
 *  Filename:       arcternalgorithm.cpp
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:    i
 *  author:         juwenqi
 *  Last modified:  2020.02.20
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/
#include <vector>
#include "src/log/logger.h"
#include "src/common/imageConvert.h"
#include "arcternalgorithm.h"
#include "src/Processors/ImageFormatConverters/CpuAnyWhereFormatConverter.h"
#include "src/common/testUtil.h"
#include "common/common.h"
#define CHECK_ERR(err) \
        if(err != ErrorType::ERR_SUCCESS) return err;

using arctern::ArcternAlgorithm;
using arctern::ErrorType;
using arctern::Result;

ArcternAlgorithm::ArcternAlgorithm() : result_(nullptr) {
}

ArcternAlgorithm::~ArcternAlgorithm() {
  ArcternAlgorithm::Release();
}

ErrorType ArcternAlgorithm::Init(const InitParameter *p) {
  ErrorType ret = ErrorType::ERR_SUCCESS;

  auto lv = CheckInitParam(p);
  if (lv != ErrorType::ERR_SUCCESS) return lv;

  initParam_ = *p;

  imageFormatChanger_.reset(new CpuAnyWhereFormatConverter);

  InitPreprocessorParam();
  return ret;
}

void ArcternAlgorithm::Release() {
  preprocessed_imgs_.clear();
  engines_.clear();
}

ErrorType ArcternAlgorithm::CheckRunParam(const RunParameter *p) {
  if (nullptr == p || p->imageV_.empty()) {
    SPDLOG_ERROR("error!!! run parameter is nullptr or imagev_ is empty.");
    return ErrorType::ERR_NO_IMAGES;
  }
  ErrorType sizeError = p->CheckSize();
  if (sizeError != ErrorType::ERR_SUCCESS) {
    SPDLOG_ERROR("error!!!! images size not equal to rects size.");
    return ErrorType::ERR_RUN_PARM;
  }
  size_t total = p->imageV_.size();
  for (size_t i = 0; i < total; ++i) {
    arctern::ArcternImage ai = p->imageV_[i];
    if (ai.width <= 0 || ai.height <= 0 || ai.imgdata.gdata == nullptr) {
      SPDLOG_ERROR("error!!! width={}, height={}, data={}, some input image is empty.",
                   ai.width, ai.height, ai.imgdata.gdata);
      return ErrorType::ERR_NO_IMAGE_DATA;
    }
  }

  return ErrorType::ERR_SUCCESS;
}

ErrorType ArcternAlgorithm::CommonPreProcess(const RunParameter *p) {
  ErrorType ret = ErrorType::ERR_SUCCESS;

  preprocessed_imgs_.clear();
  size_t total = p->imageV_.size();
  for (size_t loopId = 0; loopId < total; loopId++) {
    LocalPreProcess(p, loopId);
  }
  return ret;
}

ErrorType ArcternAlgorithm::CommonProProcess(const RunParameter *p) {
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

    auto it = preprocessed_imgs_.begin();
    preprocessed_imgs.assign(it + startId, it + startId + processImageNum);

    std::vector<arctern::Tensor<float>> outputs;

    if (it->depth() == CV_8U) {

      Tensor<uint8> input_tensor;
      input_tensor.from_cvmat(preprocessed_imgs, batchNum, swapChannel());
      auto ret = forward(input_tensor, outputs);
      if(ret  < 0 ) {
        return ERR_UNEXPECT;
      }
    } else {
      Tensor<float> input_tensor;

      input_tensor.from_cvmat(preprocessed_imgs, batchNum, swapChannel());
      forward(input_tensor, outputs);
    }

    LocalPostProcess(p, startId, endId, outputs);

    startId = endId;
    endId += batchNum;
    endId = endId > imagesNum ? imagesNum : endId;
  }

  return ret;
}

ErrorType ArcternAlgorithm::Process(const RunParameter *p, Result *r) {
  auto err = CheckRunParam(p);
  CHECK_ERR(err)

  if (r == nullptr) err = ERR_NULL_POINT;
  CHECK_ERR(err)

  result_ = r;
  result_->Release();
  err = CommonPreProcess(p);
  CHECK_ERR(err)

  err = CommonProProcess(p);

  return err;
}

ErrorType ArcternAlgorithm::CheckInitParam(const InitParameter *p) {
  if (nullptr == p || p->model_path.empty() || p->batchNum_ < 1) {
    ArcternAlgorithm::Release();
    return ErrorType::ERR_INIT_PARAM;
  }
  if (!isExist(p->model_path)) {
    return ErrorType::ERR_INVALID_MODEL;
  }
  return ErrorType::ERR_SUCCESS;
}

bool ArcternAlgorithm::swapChannel() {
  if (NetworkShape::NCHW == netShape_) {
    return true;
  }
  return false;
}

ErrorType ArcternAlgorithm::LocalPreProcess( const arctern::RunParameter *p, int idx) {
  OpencvPreprocessorRunRt runParameter;
  runParameter.cvt_type_ = GetColorConversionCodes(p->imageV_[idx].format, dstImageFormatForForward_);
  cv::Mat image;
  ArcternImage2Mat(p->imageV_[idx], image);
  runParameter.image_ = image;

  runParameter.roi_ = LocalGetRoi(p, idx);

  InitPreprocessorRunParam(&runParameter);
  auto result = preProcessor_->Process(&runParameter);

  //CHECK_RESULT()
  preprocessed_imgs_.emplace_back(result->mat_);
  return ERR_SUCCESS;
}



void ArcternAlgorithm::InitPreprocessorParam() {
  preProcessor_.reset(new OpencvPreprocessor);
  OpencvPreprocessorInitPt init_pt;
  init_pt.net_h_ = input_h_;
  init_pt.net_w_ = input_w_;
  init_pt.mean_ = 127.5;
  init_pt.std_ = 128.0;
  init_pt.border_value = 0;
  auto code = preProcessor_->Init(&init_pt);
  assert(code == ErrorType::ERR_SUCCESS);
}

int ArcternAlgorithm::forward(const Tensor<float> &input_tensor, std::vector<arctern::Tensor<float>> &outputs) {
//  cv::Mat mat(input_tensor.shape()[2], input_tensor.shape()[3],
//              CV_MAKETYPE(CV_32F, input_tensor.shape()[1]),
//              static_cast<void *>(const_cast<float *>(input_tensor.data())));
  //printfImg<float>(mat,0,0,"mxnet_input_img.data");
  return engines_[0]->forward(input_tensor, outputs);
}

int ArcternAlgorithm::forward(const Tensor<uint8> &input_tensor, std::vector<arctern::Tensor<float>> &outputs) {
  auto ret = engines_[0]->forward(input_tensor, outputs);
  return ret;
}


arctern::ArcternRect ArcternAlgorithm::LocalGetRoi(const RunParameter *p, int idx) {
    float height = p->imageV_[idx].height;
    float width = p->imageV_[idx].width;
    return ArcternRect{0, 0, width, height};
}


void ArcternAlgorithm::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
}
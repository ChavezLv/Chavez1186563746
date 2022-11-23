/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.21
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include <map>
#include <string>
#include <vector>
#include "faceGluonImpl.h"
#include "common/algUtil.h"
#include "common/featureUtil.h"
#include "common/file_buffer.h"
#include "src/common/imageConvert.h"

using namespace arctern::gluon;
using arctern::FaceGluonImpl;
using arctern::ErrorType;
using arctern::Result;

using arctern::ArcternRect;
using arctern::ArcternPoint;

FaceGluonImpl::FaceGluonImpl() : isNormaliized_(true) {
}

FaceGluonImpl::~FaceGluonImpl() noexcept {
  FaceGluonImpl::Release();
}

ErrorType FaceGluonImpl::LocalPostProcess(const RunParameter *p,
                                          int startId, int endId, std::vector<arctern::Tensor<float>> &outputs) {
  assert(p != nullptr);
  assert(startId >= 0);
  auto r = dynamic_cast<FaceGluonResult *>(result_);

  int total = endId - startId;

  int stride = outputs[0].size() / outputs[0].shape()[0];
  int offset = 0;

  for (int subId = 0; subId < total; subId++) {
    float *data = outputs[0].data() + stride * subId;
    std::vector<float> data_cls(stride);
    for (size_t i = 0; i < data_cls.size(); ++i) {
      data_cls[i] = data[i];
    }

    arctern::l2norm(data_cls);
    r->features_[startId + subId] = gluon::encryptFeature(data_cls);
    offset += stride;
  }

  return ErrorType::ERR_SUCCESS;
}

int FaceGluonImpl::GetConfigInfo(arctern::MxnetModel *model) {
  version_ = engines_[0]->GetVersion();
  return 0;
}

void FaceGluonImpl::InitPreprocessorParam() {
  using gluon::LocalPreProcessV;
  type_ = gluon::getProcessV(version_);
  maskWipe_ = gluon::LocalPreProcessV::V4 == type_;

  preProcessor_.reset(new OpencvPreprocessor);
  if (type_ == LocalPreProcessV::V1) {
    input_h_ = 128;
    input_w_ = 112;
    OpencvPreprocessorInitPt init_pt;
    init_pt.net_h_ = input_h_;
    init_pt.net_w_ = input_w_;
    init_pt.mean_ = 127.5;
    init_pt.std_ = 128.0;
    init_pt.border_value = 0;
    auto code = preProcessor_->Init(&init_pt);

    exthScale[0] = 0.12f;
    exthScale[1] = 0.12f;
    extwScale[0] = 0.06f;
    extwScale[1] = 0.06f;

    needLandmark_ = false;
  } else if (type_ == LocalPreProcessV::V2) {
    input_h_ = 112;
    input_w_ = 96;
    OpencvPreprocessorInitPt init_pt;
    init_pt.net_h_ = input_h_;
    init_pt.net_w_ = input_w_;
    init_pt.mean_ = 127.5;
    init_pt.std_ = 128.0;
    init_pt.border_value = 0;
    auto code = preProcessor_->Init(&init_pt);

    exthScale[0] = 0.083f;
    exthScale[1] = 0.083f;
    extwScale[0] = 0.f;
    extwScale[1] = 0.f;
    needLandmark_ = false;
  } else if (type_ == LocalPreProcessV::V3 || type_ == LocalPreProcessV::V4) {
    input_h_ = 128;
    input_w_ = 112;
    OpencvPreprocessorInitPt init_pt;
    init_pt.net_h_ = input_h_;
    init_pt.net_w_ = input_w_;
    init_pt.mean_ = 127.5;
    init_pt.std_ = 128.0;
    init_pt.border_value = 0;
    init_pt.alignFace_ = AlignFace::AlignFaceWith2point;
    init_pt.lmk_.push_back(ArcternPoint{38.2946, 51.6963});
    init_pt.lmk_.push_back(ArcternPoint{73.5318, 51.5014});
    auto code = preProcessor_->Init(&init_pt);
    needLandmark_ = true;
  } else if (type_ == LocalPreProcessV::V5 || type_ == LocalPreProcessV::V6) {
    input_h_ = 160;
    input_w_ = 144;

    OpencvPreprocessorInitPt init_pt;
    init_pt.net_h_ = input_h_;
    init_pt.net_w_ = input_w_;
    init_pt.mean_ = 127.5;
    init_pt.std_ = 128.0;
    init_pt.border_value = 0;

    assert(preProcessor_);

    auto code = preProcessor_->Init(&init_pt);
    assert(code == ErrorType::ERR_SUCCESS);

    exthScale[0] = 0.12f;
    exthScale[1] = 0.12f;
    extwScale[0] = 0.06f;
    extwScale[1] = 0.06f;

    needLandmark_ = false;
  }
}

ErrorType FaceGluonImpl::LocalPreProcess(const arctern::RunParameter *p, int idx) {
  if (type_ == gluon::LocalPreProcessV::V1 || type_ == gluon::LocalPreProcessV::V2 || type_ == gluon::LocalPreProcessV::V6) {
    return ArcternAlgorithm::LocalPreProcess(p, idx);
  }

  if (type_ == gluon::LocalPreProcessV::V3 || type_ == gluon::LocalPreProcessV::V4) {
    const auto implRunP = dynamic_cast<const FaceGluonRunParameter *>(p);
    OpencvPreprocessorRunRt runParameter;
    runParameter.cvt_type_ = GetColorConversionCodes(p->imageV_[idx].format, dstImageFormatForForward_);
    cv::Mat image;
    ArcternImage2Mat(p->imageV_[idx], image);
    runParameter.image_ = image;
    auto &landmarks = implRunP->faceLandmarksV_[idx];
    runParameter.lmk_ = gluon::calcAlignFacePoints(image.cols, image.rows, landmarks);
    InitPreprocessorRunParam(&runParameter);

    if (maskWipe_ && !implRunP->faceMaskV_.empty() && implRunP->faceMaskV_[idx]) {
      runParameter.sequence_.push_back(OperatorType::faceWipe);  // need wipe mask
    }
    auto result = preProcessor_->Process(&runParameter);


    //CHECK_RESULT()
    preprocessed_imgs_.emplace_back(result->mat_);
  }

  return ERR_SUCCESS;
}

void FaceGluonImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  if (type_ == gluon::LocalPreProcessV::V1 || type_ == gluon::LocalPreProcessV::V2) {
    runParm->sequence_.push_back(OperatorType::crop);
    runParm->sequence_.push_back(OperatorType::cvtColor);
    if (isNormaliized_) {
      runParm->sequence_.push_back(OperatorType::normalized);
    }
    runParm->sequence_.push_back(OperatorType::resized);

  } else if (type_ == gluon::LocalPreProcessV::V3) {
    runParm->sequence_.push_back(OperatorType::affine);
    runParm->sequence_.push_back(OperatorType::cvtColor);
    if (isNormaliized_) {
      runParm->sequence_.push_back(OperatorType::normalized);
    }

  } else if (type_ == gluon::LocalPreProcessV::V6) {
    runParm->sequence_.push_back(OperatorType::faceFeat);
    runParm->sequence_.push_back(OperatorType::cvtColor);
    //runParm->sequence_.push_back(OperatorType::resized);
    if (isNormaliized_) {
      runParm->sequence_.push_back(OperatorType::normalized);
    }
  
  }

}

static ArcternRect convert2Rect(cv::Rect &rect) {
  ArcternRect result;
  result.x = rect.x;
  result.y = rect.y;
  result.width = rect.width;
  result.height = rect.height;
  return result;
}

ArcternRect FaceGluonImpl::LocalGetRoi(const RunParameter *p, int idx) {
  if (type_ == gluon::LocalPreProcessV::V1 || type_ == gluon::LocalPreProcessV::V2) {
    auto &arctRect = p->rectsV_[idx];
    const cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);
    auto face = gluon::getFace(p->imageV_[idx].width, p->imageV_[idx].height, oriRect, exthScale, extwScale);
    face = check_shrink_box(face, p->imageV_[idx].width, p->imageV_[idx].height);
    return convert2Rect(face);
  }

  if (type_ == gluon::LocalPreProcessV::V6) {
    auto &arctRect = p->rectsV_[idx];
    const cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);
    auto roi_rect = gluon::getFaceV6(p->imageV_[idx].width, p->imageV_[idx].height, oriRect, exthScale[0], extwScale[0]);
    //auto face = check_shrink_box(roi_rect, p->imageV_[idx].width, p->imageV_[idx].height);
    return convert2Rect(roi_rect);
  }

  return ArcternAlgorithm::LocalGetRoi(p, idx);
}




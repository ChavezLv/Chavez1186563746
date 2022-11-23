/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.19
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include <src/common/imageConvert.h>
#include <vector>
#include "facialLandmarkImpl.h"
#include "common/algUtil.h"

#include "utils/faceLandmarkUtils.h"


using arctern::FacialLandmarkImpl;
using arctern::ErrorType;
using arctern::Result;

using arctern::FacialLandmarkRunParameter;

FacialLandmarkImpl::FacialLandmarkImpl() : isNormaliized_(true) {
  input_h_ = 128;
  input_w_ = 128;
}

FacialLandmarkImpl::~FacialLandmarkImpl() noexcept {
  FacialLandmarkImpl::Release();
}

ErrorType FacialLandmarkImpl::Init(const InitParameter *p) {
  auto ret = ArcternAlgorithm::Init(p);
  if (ret != ErrorType::ERR_SUCCESS) {
    FacialLandmarkImpl::Release();
    return ret;
  }

  num_landmark_ = GetLandmarkNum(p->model_path);
  return ErrorType::ERR_SUCCESS;
}

ErrorType FacialLandmarkImpl::CheckRunParam(const RunParameter *p) {
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(p);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) return arcternBaseCheck;
  return ErrorType::ERR_SUCCESS;
}

ErrorType FacialLandmarkImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                               std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<FacialLandmarkResult *>(result_);

  int offset = 0;
  int total = endId - startId;
  float *ret = outputs[0].data();
  int stride = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    if (num_landmark_ == 68) {
      for (int i = 0; i < num_landmark_; i++) {
        float tmpY = ret[i * 2 + 1 + offset];
        float tmpX = ret[i * 2 + offset];

        auto point = geneOriLandmark(tmpX, tmpY, faceRects_[startId + subId]);

        r->landmarks_[startId + subId].push_back(point);
      }
    } else {
      for (int i = 0; i < num_landmark_; i++) {
        int coor_y =
            static_cast<int>((*(ret + i * 2 + 1 + offset)) * faceRects_[startId + subId].height
                + faceRects_[startId + subId].y);
        int coor_x =
            static_cast<int>((*(ret + i * 2 + offset)) * faceRects_[startId + subId].width
                + faceRects_[startId + subId].x);
        r->landmarks_[startId + subId].push_back(ArcternPoint({static_cast<float>(coor_x),
                                                               static_cast<float>(coor_y)}));
      }
    }
    offset += stride;
  }
  return ErrorType::ERR_SUCCESS;
}

int FacialLandmarkImpl::GetConfigInfo(arctern::MxnetModel *model) {
  return 0;
}

arctern::ArcternRect FacialLandmarkImpl::LocalGetRoi(const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const FacialLandmarkRunParameter *>(p);

  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);
  auto rect = check_shrink_box(oriRect, p->imageV_[idx].width, p->imageV_[idx].height);
  if (num_landmark_ == 68) {
    rect = getFace68(p->imageV_[idx].width, p->imageV_[idx].height, rect);
  } else {
    rect = getFaceTiny(p->imageV_[idx].width, p->imageV_[idx].height, rect);
  }
  faceRects_.push_back(rect);
  arctern::ArcternRect result;
  result.x = rect.x;
  result.y = rect.y;
  result.width = rect.width;
  result.height = rect.height;

  return result;
}


void FacialLandmarkImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  if(isNormaliized_) {
    runParm->sequence_.push_back(OperatorType::normalized);
  }
  runParm->sequence_.push_back(OperatorType::resized);
}


ErrorType FacialLandmarkImpl::Process(const RunParameter *p, Result *r) {
  faceRects_.clear();
  return ArcternAlgorithm::Process(p, r);
}

// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.02
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#include "faceJoyImpl.h"
#include "common/imageConvert.h"
#include "common/file_buffer.h"
#include "faceJoy.h"
#include "log/logger.h"


using namespace arctern;

FaceJoyImpl::FaceJoyImpl()
    : m_aLandmarksExtractIdx{72, 73, 78, 79,  84,  85,  90,
                             91, 60, 61, 120, 121, 128, 129} {
  input_h_ = 60;
  input_w_ = 60;
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_RGB888;
}

FaceJoyImpl::~FaceJoyImpl() { FaceJoyImpl::Release(); }

int FaceJoyImpl::GetConfigInfo(arctern::MxnetModel *model) { return 0; }

ErrorType FaceJoyImpl::LocalPreProcess(const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const FaceJoyRunParameter *>(p);

  auto &landmarks = implRunP->faceLandmarksV_[idx];

  OpencvPreprocessorRunRt runParameter;
  runParameter.cvt_type_ = GetColorConversionCodes(p->imageV_[idx].format, dstImageFormatForForward_);
  SetLandmarks(landmarks, runParameter.lmk_);
  cv::Mat image;
  ArcternImage2Mat(p->imageV_[idx], image);
  runParameter.image_ = image;
  runParameter.sequence_.push_back(OperatorType::faceJoy);
  runParameter.sequence_.push_back(OperatorType::cvtColor);
  runParameter.sequence_.push_back(OperatorType::normalized);

  InitPreprocessorRunParam(&runParameter);
  auto result = preProcessor_->Process(&runParameter);

  //CHECK_RESULT()
  preprocessed_imgs_.emplace_back(result->mat_);

  return ERR_SUCCESS;
}

ErrorType
FaceJoyImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                              std::vector<arctern::Tensor<float>> &outputs) {
  auto res = dynamic_cast<FaceJoyResult *>(result_);
  int total = endId - startId;
  int stride = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data = outputs[0].data() + subId * stride;
    std::vector<float> data_cls(stride);
    memcpy(data_cls.data(), data, stride * sizeof(float));

    int classid = 1;
    float score = data_cls[0];
    score = std::min(std::max(score, 0.0f), 1.0f);

    res->m_vJoyResults[startId + subId] =
        std::vector<std::pair<int, float>>({std::make_pair(classid, score)});
  }

  return ErrorType::ERR_SUCCESS;
}

ErrorType FaceJoyImpl::CheckRunParam(const RunParameter *p) {
   auto rp = dynamic_cast<const FaceJoyRunParameter *>(p);
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(rp);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) {
    return arcternBaseCheck;
  }

  return ErrorType::ERR_SUCCESS;
}

void FaceJoyImpl::SetLandmarks(const std::vector<float> &landmarks,
                                  std::vector<ArcternPoint> &validmarks) {
  if (landmarks.empty()) {
    return;
  }

  validmarks.clear();
  for (int i = 0; i < m_aLandmarksExtractIdx.size(); i += 2) {
    ArcternPoint point;
    point.x = landmarks.at(m_aLandmarksExtractIdx.at(i));
    point.y = landmarks.at(m_aLandmarksExtractIdx.at(i + 1));
    validmarks.emplace_back(point);
  }
}
//
// Created by Admin on 2021/6/18.
//

#include "TrtFaceJoyImpl.h"
#include "src/common/common.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
namespace arctern {

TrtFaceJoyImpl::TrtFaceJoyImpl() : m_aLandmarksExtractIdx{72, 73, 78, 79,  84,  85,  90,
                                                          91, 60, 61, 120, 121, 128, 129} {
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
}

TrtFaceJoyImpl::~TrtFaceJoyImpl() {

}

ErrorType
TrtFaceJoyImpl::GetModelsConfigInfo(const MxnetModels *models) {
  m_modelsInputHeight = 60;
  m_modelsInputWidth = 60;
  return arctern::ERR_SUCCESS;
}

ErrorType TrtFaceJoyImpl::LocalPostProcess(const RunParameter *p,
                                           int startId,
                                           int endId,
                                           vector<arctern::OutputTensor<float>> &outputs) {
  auto res = dynamic_cast<FaceJoyResult *>(m_pResult);
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

ErrorType TrtFaceJoyImpl::LocalPreProcess(const RunParameter *p, const int &idx) {
  const auto implRunP = dynamic_cast<const FaceJoyRunParameter *>(p);
  auto &landmarks = implRunP->faceLandmarksV_[idx];

  NppPreprocessorRunPt runParameter;
  SetLandmarks(landmarks, runParameter.m_lmk);

  runParameter.m_cvtColorCode = GetColorConversionCodes(p->imageV_[idx].format, m_dstImageFormatForForward);
  runParameter.m_roi = LocalGetROI(p, idx);
  runParameter.arcternImage->resize(p->imageV_[idx].height, p->imageV_[idx].width,
                                    GetArcternImageChannels(p->imageV_[idx].format),
                                    nvinfer1::DataType::kINT8, p->imageV_[idx].step);

  ArcternImage2DeviceBuffer( p->imageV_[idx], *(runParameter.arcternImage));
  InitPreprocessorRunParam(&runParameter);
  auto result = m_pPreprocessor->Process(&runParameter);
  if (m_pPreprocessor->GetLastError() != 0) {
    m_lastErrorCode = ErrorType::ERR_UNEXPECT;
  }

  m_vPreprocessRet.emplace_back(std::move(result));

  return m_lastErrorCode;
}

void TrtFaceJoyImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::faceJoy);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}

void TrtFaceJoyImpl::SetLandmarks(const std::vector<float> &landmarks,
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

}

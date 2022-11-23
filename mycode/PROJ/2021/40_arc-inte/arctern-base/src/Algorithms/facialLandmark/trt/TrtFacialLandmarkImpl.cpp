//
// Created by Admin on 2021/6/18.
//

#include "TrtFacialLandmarkImpl.h"
#include "src/common/common.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
#include "../utils/faceLandmarkUtils.h"
namespace arctern {

TrtFacialLandmarkImpl::TrtFacialLandmarkImpl() {
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;

}

TrtFacialLandmarkImpl::~TrtFacialLandmarkImpl() {

}

ErrorType
TrtFacialLandmarkImpl::GetModelsConfigInfo(const MxnetModels *models) {
  m_modelsInputHeight = 128;
  m_modelsInputWidth = 128;
  return ErrorType::ERR_SUCCESS;
}
ErrorType TrtFacialLandmarkImpl::Process(const RunParameter *p,Result *r){
  m_vFaceRects.clear();
  return ArcternAlgorithmEx::Process(p,r);
}
ErrorType TrtFacialLandmarkImpl::LocalPostProcess(const RunParameter *p,
                                                  int startId,
                                                  int endId,
                                                  vector<arctern::OutputTensor<float>> &outputs) {
  auto r = dynamic_cast<FacialLandmarkResult *>(m_pResult);

  int offset = 0;
  int total = endId - startId;
  float *ret = outputs[0].data();
  int stride = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    if (m_landmarkNum == 68) {
      for (int i = 0; i < m_landmarkNum; i++) {
        float tmpY = ret[i * 2 + 1 + offset];
        float tmpX = ret[i * 2 + offset];

        auto point = geneOriLandmark(tmpX, tmpY, m_vFaceRects[startId + subId]);

        r->landmarks_[startId + subId].push_back(point);
      }
    } else {
      for (int i = 0; i < m_landmarkNum; i++) {
        int coor_y =
            static_cast<int>((*(ret + i * 2 + 1 + offset)) * m_vFaceRects[startId + subId].height
                + m_vFaceRects[startId + subId].y);
        int coor_x =
            static_cast<int>((*(ret + i * 2 + offset)) * m_vFaceRects[startId + subId].width
                + m_vFaceRects[startId + subId].x);
        r->landmarks_[startId + subId].push_back(ArcternPoint({static_cast<float>(coor_x),
                                                               static_cast<float>(coor_y)}));
      }
    }
    offset += stride;
  }
  return ErrorType::ERR_SUCCESS;
}

ArcternRect TrtFacialLandmarkImpl::LocalGetROI(const RunParameter *p, int idx) {

  const auto pRunParam = dynamic_cast<const RunParameter *>(p);
  auto &faceRect = pRunParam->rectsV_[idx];
  auto rect = CheckShrinkBox(faceRect, p->imageV_[idx].width, p->imageV_[idx].height);
  m_landmarkNum = GetLandmarkNum(m_initParam.model_path);
  if (m_landmarkNum == 68) {
    rect = getFace68(p->imageV_[idx].width, p->imageV_[idx].height, rect);
  } else {
    rect = getFaceTiny(p->imageV_[idx].width, p->imageV_[idx].height, rect);
  }
  m_vFaceRects.push_back(rect);
  return rect;
}

void TrtFacialLandmarkImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}


}

//
// Created by gh on 2020/12/17.
//

#include "atlasAgeGenderImpl.h"
#include "src/common/atlas/atlasAclDataType.h"

#include "Engin/src/atlas/atlasengin.h"
#include "common/algUtil.h"
#include "src/Algorithms/ageGender/utils/AgeGenderUtil.h"
namespace arctern{

AtlasAgeGenderImpl::AtlasAgeGenderImpl(){
   m_modelsInputHeight = 128;
   m_modelsInputWidth = 112;
}

AtlasAgeGenderImpl::~AtlasAgeGenderImpl() {
  
}

ErrorType AtlasAgeGenderImpl::GetModelsConfigInfo(const TrtModels* models){
  m_extScale[0] = 0.0;
  m_extScale[2] = 0.0;
  auto model = models->models()->Get(0);
  if(model->version()->str() == "0.1.0") {
    m_extScale[1] = 0.3;
    m_extScale[3] = 0.3;
  } else {
    m_extScale[1] = 0.5;
    m_extScale[3] = 0.1;
  }
  return ErrorType::ERR_SUCCESS;
}


cv::Rect AtlasAgeGenderImpl::GetFace(int width, int height,const cv::Rect &bbox) {
  float x1 = bbox.tl().x;
  float y1 = bbox.tl().y;
  float x2 = bbox.br().x;
  float y2 = bbox.br().y;
  float w = x2 - x1 + 1;
  float h = y2 - y1 + 1;

  y1 = y1 - h * m_extScale[1];
  y2 = y2 + h * m_extScale[3];
  float by1 = std::min(std::max(0, static_cast<int>(y1)), height);
  float by2 = std::min(std::max(0, static_cast<int>(y2)), height);
  float hh = by2 - by1 + 1;
  float ww = hh;
  if (ww > w) {
    int wx = static_cast<int>((ww - w) / 2);
    x1 = x1 - wx;
    x2 = x2 + wx;
  }

  float bx1 = std::min(std::max(0, static_cast<int>(x1)), width);
  float bx2 = std::min(std::max(0, static_cast<int>(x2)), width);

  return cv::Rect(cv::Point(bx1, by1), cv::Point(bx2, by2));
}

ErrorType AtlasAgeGenderImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
std::vector<std::vector<float>> &outputs) {
  auto r = dynamic_cast<AgeGenderResult *>(m_pResult);

  int total = endId - startId;
  
  // const auto batchNum = initParam_.batchNum_;
  int stride0 = outputs[0].size() / total;
  int stride1 = outputs[1].size() / total;

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    const float *data1 = outputs[1].data() + subId * stride1;

    auto gender = calc_gender(data0, stride0);
    auto age = calc_age(data1, stride1);

    r->ageGenders_[startId + subId].age = age.first;
    r->ageGenders_[startId + subId].ageScore = age.second;
    r->ageGenders_[startId + subId].gender = gender.first == 0 ? MALE : FEMALE;
    r->ageGenders_[startId + subId].genderScore = gender.second;
  }

  return ErrorType::ERR_SUCCESS;
}

cv::Rect AtlasAgeGenderImpl::LocalGetROI(const RunParameter *p, int idx){
  auto &arctRect = p->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);
  auto rect = AtlasAgeGenderImpl::GetFace(p->imageV_[idx].width, p->imageV_[idx].height, oriRect);
  return rect;
}

ErrorType AtlasAgeGenderImpl::LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
std::vector<DvppDataInfo>& vOutputInfos,
std::vector<cv::Rect>& vRects) {

  int size = vInputInfo.size();
  std::vector<DvppCropInputInfo> vCropInputInfo(size);
  for(int i = 0; i < size; ++i){
    vCropInputInfo[i].dataInfo = vInputInfo[i];
    vCropInputInfo[i].roi.left = vRects[i].x;
    vCropInputInfo[i].roi.up = vRects[i].y;
    vCropInputInfo[i].roi.right = vRects[i].x + vRects[i].width - 1;
    vCropInputInfo[i].roi.down = vRects[i].y + vRects[i].height - 1;
  }

  auto ret = m_pPreprocessor->CombineCropResizeBatchProcess(vCropInputInfo,vOutputInfos,true);

  if (ret != APP_ERR_OK) {
    LOGASDError << "failed to do altas preprocess";
    m_lastErrorCode = ErrorType::ERR_PRE_PROCESS;
  }
  return m_lastErrorCode;
}


}

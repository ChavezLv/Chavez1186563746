//
// Created by gh on 2020/12/17.
//


#include "atlasPersonHeadwearImpl.h"
#include "src/common/atlas/atlasAclDataType.h"

#include "Engin/src/atlas/atlasengin.h"
#include "common/algUtil.h"
#include "src/Algorithms/personHeadwear/utils/personHeadwearUtil.h"
namespace arctern{

AtlasPersonHeadwearImpl::AtlasPersonHeadwearImpl(): m_extScale{0.0, 0.15, 0.0, 0.0}{
   m_modelsInputWidth = 128;
   m_modelsInputHeight = 320;
}

AtlasPersonHeadwearImpl::~AtlasPersonHeadwearImpl() {
  
}

ErrorType AtlasPersonHeadwearImpl::GetModelsConfigInfo(const TrtModels* models){

  return ErrorType::ERR_SUCCESS;
}




ErrorType AtlasPersonHeadwearImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
std::vector<std::vector<float>> &outputs) {
  auto r = dynamic_cast<PersonHeadwearResult *>(m_pResult);

  int total = endId - startId;

  // const auto batchNum = initParam_.batchNum_;
  int stride0 = outputs[0].size() / total;

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    auto headwear = personHeadwear::calcHeadwear(data0, stride0);

    r->PersonHeadwears_[startId + subId].headwearType = PersonHeadwearType(headwear.first);
    r->PersonHeadwears_[startId + subId].typeScore = headwear.second;
  }

  return ErrorType::ERR_SUCCESS;
}


cv::Rect AtlasPersonHeadwearImpl::LocalGetROI(const RunParameter *p, int idx)
{
  const auto implRunP = dynamic_cast<const PersonHeadwearRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  auto rect = check_shrink_box(oriRect, p->imageV_[idx].width , p->imageV_[idx].height);
  rect = get_face(p->imageV_[idx].width, p->imageV_[idx].height, rect, m_extScale, Float2Int::TRUNCATION, false);
  return rect;
}

ErrorType AtlasPersonHeadwearImpl::LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
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

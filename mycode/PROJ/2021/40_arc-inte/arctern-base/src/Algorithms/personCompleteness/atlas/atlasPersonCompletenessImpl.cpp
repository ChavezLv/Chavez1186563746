//
// Created by gh on 2020/12/17.
//

#include "atlasPersonCompletenessImpl.h"
#include "src/common/atlas/atlasAclDataType.h"

#include "Engin/src/atlas/atlasengin.h"
#include "common/algUtil.h"
#include "src/Algorithms/personCompleteness/utils/personCompletenessUtil.h"
namespace arctern{

AtlasPersonCompletenessImpl::AtlasPersonCompletenessImpl(): m_extScale{0.0, 0.0, 0.0, 0.0}{
   m_modelsInputWidth = 128;
   m_modelsInputHeight = 256;
}

AtlasPersonCompletenessImpl::~AtlasPersonCompletenessImpl() {
  
}

ErrorType AtlasPersonCompletenessImpl::GetModelsConfigInfo(const TrtModels* models){

  return ErrorType::ERR_SUCCESS;
}




ErrorType AtlasPersonCompletenessImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
std::vector<std::vector<float>> &outputs) {
  auto r = dynamic_cast<PersonCompletenessResult *>(m_pResult);
  int total = endId - startId;
  int stride0 = outputs[0].size() / total; // 2
  int stride1 = outputs[1].size() / total; // 5
  int stride2 = 128;

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    const float *data1 = outputs[1].data() + subId * stride1;
    const float *data2 = outputs[2].data() + subId * stride2;
    parse_truncation(data0, stride0, r->personCompleteness_[startId + subId].truncation);
    parse_visible_parts(data1, stride1,
                        r->personCompleteness_[startId + subId].visibleParts,
                        r->personCompleteness_[startId + subId].visiblePartsScore);
    parse_soft_mask(data2, stride2,
                    r->personCompleteness_[startId + subId].softMasks,
                    r->personCompleteness_[startId + subId].softMasksScore);
  }

  return ErrorType::ERR_SUCCESS;
}

cv::Rect AtlasPersonCompletenessImpl::get_roi( const int &img_width,
          const int &img_height,
          const cv::Rect &bbox,
          const std::array<float, 4> ext_scale) const {
    int height = img_height;
    int width = img_width;
    int w = bbox.width;
    int h = bbox.height;
    float x1 = bbox.tl().x;
    float y1 = bbox.tl().y;
    float x2 = bbox.br().x;
    float y2 = bbox.br().y;

    x1 -= w * ext_scale[0];
    y1 -= h * ext_scale[1];
    x2 += w * ext_scale[2];
    y2 += h * ext_scale[3];
    float bx1 = std::min(std::max(0, static_cast<int>(round(x1))), width - 1);
    float by1 = std::min(std::max(0, static_cast<int>(round(y1))), height - 1);
    float bx2 = std::min(std::max(0, static_cast<int>(round(x2))), width - 1);
    float by2 = std::min(std::max(0, static_cast<int>(round(y2))), height - 1);
    return cv::Rect(cv::Point(bx1, by1), cv::Point(bx2 + 1, by2 + 1));
}
cv::Rect AtlasPersonCompletenessImpl::LocalGetROI(const RunParameter *p, int idx)
{
  const auto implRunP = dynamic_cast<const PersonCompletenessRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  auto rect = check_shrink_box(oriRect, p->imageV_[idx].width, p->imageV_[idx].height);
  rect = get_roi(p->imageV_[idx].width, p->imageV_[idx].height, rect, m_extScale);
  return rect;
}

ErrorType AtlasPersonCompletenessImpl::LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
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

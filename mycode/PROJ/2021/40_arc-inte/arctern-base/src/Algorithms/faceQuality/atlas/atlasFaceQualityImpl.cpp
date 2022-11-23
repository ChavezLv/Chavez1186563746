//
// Created by gh on 2020/12/17.
//

#include "atlasFaceQualityImpl.h"
#include "src/common/atlas/atlasAclDataType.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "Engin/src/atlas/atlasengin.h"
#include "common/algUtil.h"

namespace arctern{

AtlasFaceQualityImpl::AtlasFaceQualityImpl(){
  m_modelsInputWidth = m_modelsInputHeight = 50;
}

AtlasFaceQualityImpl::~AtlasFaceQualityImpl() {
  
}

ErrorType AtlasFaceQualityImpl::GetModelsConfigInfo(const TrtModels* models){

  auto model = models->models()->Get(0);

  // bool ret = try_get_value(*model, "GRAY_IMG_INPUT", m_bGrayInput);
  // assert(ret == true);

  // ret = try_get_value(*model, "BRANCH_NUM", m_outputNum);
  // assert(ret == true);

  return ErrorType::ERR_SUCCESS;
}




ErrorType AtlasFaceQualityImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
std::vector<std::vector<float>> &outputs) {
   auto r = dynamic_cast<FaceQualityResult *>(m_pResult);

  int total = endId - startId;
  int stride0 = outputs[0].size() / total;

  bool secondOutput = outputs.size() > 1;
  int stride1 = secondOutput ? outputs[1].size() / total : 0;

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    const float *data1 = secondOutput ? outputs[1].data() + subId * stride1 : nullptr;
    r->faceQuality_[startId + subId] = CalculateResult(data0, data1);
  }

  return ErrorType::ERR_SUCCESS;
}

float AtlasFaceQualityImpl::CalculateResult(const float *data0, const float *data1){
  if (m_outputNum > 1) {
    return data1[0] > data1[1] ? 0 : data0[0];
  } else {
    return data0[0];
  }
}

cv::Rect AtlasFaceQualityImpl::LocalGetROI(const RunParameter *p, int idx){
  
  auto &arctRect = p->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  auto rect = check_shrink_box(oriRect, p->imageV_[idx].width, p->imageV_[idx].height);

  return rect;
}

ErrorType AtlasFaceQualityImpl::LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
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

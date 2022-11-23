
#include "atlasPreventionAttrImpl.h"
#include "src/common/atlas/atlasAclDataType.h"

#include "Engin/src/atlas/atlasengin.h"
#include "common/algUtil.h"
#include "src/common/arcterndefs_config_analyzer.h"
namespace arctern{

AtlasPreventionAttrImpl::AtlasPreventionAttrImpl(){
   m_modelsInputHeight = 256;
   m_modelsInputWidth = 128;
}

AtlasPreventionAttrImpl::~AtlasPreventionAttrImpl() {
  
}

ErrorType AtlasPreventionAttrImpl::GetModelsConfigInfo(const TrtModels* models){

  return ErrorType::ERR_SUCCESS;
}




ErrorType AtlasPreventionAttrImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
std::vector<std::vector<float>> &outputs) {
   auto r = dynamic_cast<PreventionAttrResult *>(m_pResult);

  int total = endId - startId;

  int stride0 = outputs[0].size() / total;
  int stride1 = outputs[1].size() / total;
  int stride2 = outputs[2].size() / total;
 

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    auto result = CalculateResult(data0, stride0);
    r->PreventionAttrs_[startId + subId].exposeNeckband = ExposeNeckband(result.first);
    r->PreventionAttrs_[startId + subId].exposeNeckbandScore = result.second;

    const float *data1 = outputs[1].data() + subId * stride1;
    auto result1 = CalculateResult(data1, stride1);
    r->PreventionAttrs_[startId + subId].touchMask = TouchMask(result1.first);
    r->PreventionAttrs_[startId + subId].touchMaskScore = result1.second;

    const float *data2 = outputs[2].data() + subId * stride2;
    auto result2 = CalculateResult(data2, stride2);
    r->PreventionAttrs_[startId + subId].playPhone = PlayPhone(result2.first);
    r->PreventionAttrs_[startId + subId].playPhoneScore = result2.second;

  }

  return ErrorType::ERR_SUCCESS;
}

pair<int, float> AtlasPreventionAttrImpl::CalculateResult(const float *data, int step) {
  std::vector<float> prob;
  for (size_t j = 0; j < step; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}


cv::Rect AtlasPreventionAttrImpl::LocalGetROI(const RunParameter *p, int idx){
  auto &arctRect = p->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);
  return oriRect;
}

ErrorType AtlasPreventionAttrImpl::LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
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

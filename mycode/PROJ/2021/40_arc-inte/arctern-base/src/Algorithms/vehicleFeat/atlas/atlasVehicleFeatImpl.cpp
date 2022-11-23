//
// Created by gh on 2020/12/17.
//

#include "atlasVehicleFeatImpl.h"
#include "src/common/atlas/atlasAclDataType.h"
#include "common/algUtil.h"
#include "common/featureUtil.h"
#include "common/arcterndefs_config_analyzer.h"
namespace arctern{

AtlasVehicleFeatImpl::AtlasVehicleFeatImpl(){
  m_modelsInputHeight = 224;
  m_modelsInputWidth = 224;
}

AtlasVehicleFeatImpl::~AtlasVehicleFeatImpl() {
  
}

ErrorType AtlasVehicleFeatImpl::GetModelsConfigInfo(const TrtModels* models){

  return ERR_SUCCESS;
}

ErrorType AtlasVehicleFeatImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                      std::vector<std::vector<float>> &outputs) {
  auto res = dynamic_cast<vehicleFeatResult *>(m_pResult);
  int total = endId - startId;
  int stride = outputs[0].size() / total;

  for (int subId = 0; subId < total; subId++) {
    const float *data = outputs[0].data() + subId * stride;
    std::vector<float> data_cls(stride);
    memcpy(data_cls.data(), data, stride * sizeof(float));

    l2norm(data_cls);

    res->m_vFeatResults[startId + subId] = encrypt_feature(data_cls);
  }

  return ErrorType::ERR_SUCCESS;
}

ErrorType AtlasVehicleFeatImpl::LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
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

cv::Rect AtlasVehicleFeatImpl::LocalGetROI(const RunParameter *p, int idx){
  const auto implRunP = dynamic_cast<const vehicleFeatRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);
  auto rect = check_shrink_box(oriRect, p->imageV_[idx].width, p->imageV_[idx].height);
 
  return rect;
}


}

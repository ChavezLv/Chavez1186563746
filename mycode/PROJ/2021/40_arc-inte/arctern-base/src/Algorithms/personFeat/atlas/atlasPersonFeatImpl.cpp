//
// Created by gh on 2020/12/17.
//

#include "atlasPersonFeatImpl.h"
#include "src/common/atlas/atlasAclDataType.h"
#include "common/algUtil.h"
#include "common/featureUtil.h"
#include "common/arcterndefs_config_analyzer.h"
namespace arctern{

AtlasPersonFeatImpl::AtlasPersonFeatImpl(){
  m_modelsInputHeight = 384;
  m_modelsInputWidth = 128;
}

AtlasPersonFeatImpl::~AtlasPersonFeatImpl() {
  
}

ErrorType AtlasPersonFeatImpl::GetModelsConfigInfo(const TrtModels* models){
  auto model = models->models()->Get(0);
  int input_dim_w, input_dim_h;
  bool ret = try_get_value<int>(*model, "w", input_dim_w);
  ret &= try_get_value(*model, "h", input_dim_h);
  if(ret) {
    m_modelsInputHeight = input_dim_h;
    m_modelsInputWidth = input_dim_w;
  }

  return ERR_SUCCESS;
}

ErrorType AtlasPersonFeatImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                      std::vector<std::vector<float>> &outputs) {
  assert(p != nullptr);
  assert(startId >= 0);
  auto r = dynamic_cast<PersonFeatResult *>(m_pResult);

  int total = endId - startId;

  int stride = outputs[0].size() / total;
  int offset = 0;

  for (int subId = 0; subId < total; subId++) {
    float *data = outputs[0].data() + stride * subId;
    std::vector<float> data_cls(stride);
    for (size_t i = 0; i < data_cls.size(); ++i) {
      data_cls[i] = data[i];
    }

    arctern::l2norm(data_cls);

    r->features_[startId + subId] = encrypt_feature(data_cls);

    offset += stride;
  }

  return ErrorType::ERR_SUCCESS;
}

ErrorType AtlasPersonFeatImpl::LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
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

cv::Rect AtlasPersonFeatImpl::LocalGetROI(const RunParameter *p, int idx){
  const auto implRunP = dynamic_cast<const PersonFeatRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);
  auto rect = check_shrink_box(oriRect, p->imageV_[idx].width, p->imageV_[idx].height);
 
  return rect;
}


}

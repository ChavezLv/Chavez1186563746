//
// Created by lzm on 2020/12/17.
//

#include "atlasRoadWaterImpl.h"
#include "src/common/atlas/atlasAclDataType.h"

#include "Engin/src/atlas/atlasengin.h"
#include "common/algUtil.h"

namespace arctern{

AtlasRoadWaterImpl::AtlasRoadWaterImpl(){

}

AtlasRoadWaterImpl::~AtlasRoadWaterImpl() {
  
}

ErrorType AtlasRoadWaterImpl::GetModelsConfigInfo(const TrtModels* models){
  m_modelsInputHeight = m_modelsInputWidth = 512;
  return ErrorType::ERR_SUCCESS;
}




ErrorType AtlasRoadWaterImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
std::vector<std::vector<float>> &outputs) {
  
  auto r = dynamic_cast<RoadWaterResult *>(m_pResult);

  int total = endId - startId;

  const float *pdata = outputs[0].data();
  int start = 0;
  int stride = outputs[0].size() / total;
  for (size_t j = 0; j < total; ++j) {
  

    std::vector<float> prob;
    for (int l = start; l < start + stride; ++l) {
      prob.push_back(pdata[l]);
    }
    
    std::copy(prob.begin(), prob.end(), std::ostream_iterator<float>(std::cout, " "));
    std::cout<<std::endl;

    float maxval;
    int maxidx = softmax(prob, maxval);
    maxval = *max_element(prob.begin(),prob.end());
    
    r->roadWaterInfos_[startId+j].confidence = maxval;
    r->roadWaterInfos_[startId+j].type = (RoadWaterType) maxidx;
    start += stride;
  }

  return m_lastErrorCode;

}

ErrorType AtlasRoadWaterImpl::LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
std::vector<DvppDataInfo>& vOutputInfos,
std::vector<cv::Rect>& vRects) {
  
  auto ret = m_pPreprocessor->CombineResizeBatchProcess(vInputInfo, vOutputInfos, true, VpcProcessType::VPC_PT_PADDING);

  if (ret != APP_ERR_OK) {
    LOGASDError << "failed to do altas preprocess";
    m_lastErrorCode = ErrorType::ERR_PRE_PROCESS;
  }
  return m_lastErrorCode;
}


}

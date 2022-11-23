//
// Created by gh on 2020/12/17.
//

#include "atlasLicenseRecogImpl.h"
#include "src/common/atlas/atlasAclDataType.h"

#include "Engin/src/atlas/atlasengin.h"
#include "common/algUtil.h"

namespace arctern{

AtlasLicenseRecogImpl::AtlasLicenseRecogImpl(){
   m_modelsInputHeight = 48;
  m_modelsInputWidth = 192;
}

AtlasLicenseRecogImpl::~AtlasLicenseRecogImpl() {
  
}

ErrorType AtlasLicenseRecogImpl::GetModelsConfigInfo(const TrtModels* models){

  return ErrorType::ERR_SUCCESS;
}

std::vector<float> AtlasLicenseRecogImpl::doTopK(float* data, int size, int strip){
  std::vector<float> outputs;

  for(int pos = 0; pos < size; pos+=strip){
    float* data_t = data + pos;
    int i = 0;
    while(i < strip){
      float index;
      float max = -100.0;
      for(int j = 0; j < 70; ++j){
        auto value = *(data_t + i + j);
        if(value > max){
          max = value;
          index = j;
        }
      }
      outputs.push_back(index);
      i += 70;
    }
  }

  return outputs;
}

ErrorType AtlasLicenseRecogImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
std::vector<std::vector<float>> &outputs) {
  auto r = dynamic_cast<LicenseRecogResult *>(m_pResult);

  int total = endId - startId;
  int stride = outputs[0].size() / total;
  auto realOutput = doTopK(outputs[0].data(),outputs[0].size(),stride);
  int stride0 = realOutput.size() / total;

  for (int subId = 0; subId < total; subId++) {
    std::vector<float> res;
    const float *data0 = realOutput.data() + subId * stride0;
    if (*(data0) != INDEXFORSPACE) {
      res.push_back(*(data0));
    }

    for (int i = 1; i < stride0; i++) {
      if(*(data0 + i) == *(data0 + i - 1) ||
          *(data0 + i) == INDEXFORSPACE) {
        continue;
      }
      res.push_back(*(data0 + i));
    }
    r->LicenseRecogs_[startId + subId] = CalculateResult(res);
  }
  return ErrorType::ERR_SUCCESS;
}

std::string AtlasLicenseRecogImpl::CalculateResult(const std::vector<float> &output) {
  std::string license = "";
  for (size_t i = 0; i < output.size(); i++) {
    int index = static_cast<int>(output[i]) < 0 ? 0 : static_cast<int>(output[i]);
    index = index > INDEXFORSPACE ? INDEXFORSPACE : index;
    if (LicenseCharacters[index] == "^") {
      break;
    }
    license = license + LicenseCharacters[index];
  }
  return license;
}

cv::Rect AtlasLicenseRecogImpl::LocalGetROI(const RunParameter *p, int idx){
  auto &arctRect = p->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);
  auto rect = check_shrink_box(oriRect, p->imageV_[idx].width, p->imageV_[idx].height);
  return rect;
}

ErrorType AtlasLicenseRecogImpl::LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
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

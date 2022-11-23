//
// Created by gh on 2020/12/17.
//

#include "atlasVehicleCycleAttrImpl.h"
#include "src/common/atlas/atlasAclDataType.h"

#include "Engin/src/atlas/atlasengin.h"
#include "common/algUtil.h"
#include "src/common/arcterndefs_config_analyzer.h"
namespace arctern{

AtlasVehicleCycleAttrImpl::AtlasVehicleCycleAttrImpl():  m_extScale{0.1, 0.4, 0.1, 0}{
  m_modelsInputHeight = 224;
  m_modelsInputWidth = 128;
}

AtlasVehicleCycleAttrImpl::~AtlasVehicleCycleAttrImpl() {
  
}

ErrorType AtlasVehicleCycleAttrImpl::GetModelsConfigInfo(const TrtModels* models){
  return ErrorType::ERR_SUCCESS;
}




ErrorType AtlasVehicleCycleAttrImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
std::vector<std::vector<float>> &outputs) {
   auto r = dynamic_cast<VehicleCycleAttrResult *>(m_pResult);

  int total = endId - startId;

  for (size_t i = 0; i < outputs.size(); ++i) {
    int stride = outputs[i].size() / total;
    for (int subId = 0; subId < total; subId++) {
      const float *data0 = outputs[i].data() + subId * stride;
      std::pair<int, float> attr_res = CalculateResult(data0, stride);
      auto &attr = r->vehicleCycleAttrs_[startId + subId];
      switch (i) {
        case 0 : {
          attr.vehicleNoMotorType = VehicleNoMotorType(attr_res.first);
          attr.vehicleNoMotorTypeScore = attr_res.second;
          break;
        }
        case 1 : {
          attr.hasUmbrella = !attr_res.first;
          attr.hasUmbrellaScore = attr_res.second;
          break;
        }
        case 2 : {
          attr.isMultiPerson = !attr_res.first;
          attr.isMultiPersonScore = attr_res.second;
          break;
        }
        case 3 : {
          attr.isCycling = !attr_res.first;
          attr.isCyclingScore = attr_res.second;
          break;
        }
        case 4 : {
          attr.orientationType = VehicleCycleOrientationType(attr_res.first);
          attr.orientationTypeScore = attr_res.second;
          break;
        }
        default: {
          break;
        }
      } // switch
    } // images id
  } // attributes id
  return ErrorType::ERR_SUCCESS;
}

pair<int, float> AtlasVehicleCycleAttrImpl::CalculateResult(const float *data, int step) {
  std::vector<float> prob;
  for (size_t j = 0; j < step; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  max_score = *max_element(prob.begin(), prob.end());
  return std::make_pair(max_idx, max_score);
}


cv::Rect AtlasVehicleCycleAttrImpl::get_vehicle(const int &width, const int &height, const cv::Rect &bbox) {
  float x1 = bbox.tl().x;
  float y1 = bbox.tl().y;
  float x2 = bbox.br().x;
  float y2 = bbox.br().y;
  float bw = x2 - x1;
  float bh = y2 - y1;
  x1 = x1 - 0.1 * bw;
  y1 = y1 - 0.4 * bh;
  x2 = x2 + 0.1 * bw;
  auto sx1 = (int) (x1 < 0 ? 0 : x1);
  auto sy1 = (int) (y1 < 0 ? 0 : y1);
  auto sx2 = (int) (x2 < width - 1 ? x2 : width - 1);
  auto sy2 = (int) (y2 < height - 1 ? y2 : height - 1);
  auto ew = (int) (sx2 - sx1 + 1);
  auto eh = (int) (sy2 - sy1 + 1);
  cv::Rect vehicle_rect = cv::Rect(sx1, sy1, ew, eh);
  return vehicle_rect;
}


cv::Rect AtlasVehicleCycleAttrImpl::LocalGetROI(const RunParameter *p, int idx){
  cv::Rect roi (p->rectsV_[idx].x,p->rectsV_[idx].y,p->rectsV_[idx].width,p->rectsV_[idx].height);
  auto rect = get_vehicle(p->imageV_[idx].width,p->imageV_[idx].height,roi);
  return rect;
}

ErrorType AtlasVehicleCycleAttrImpl::LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
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

//
// Created by gh on 2020/12/17.
//

#include "atlasVehicleAttrImpl.h"
#include "src/common/atlas/atlasAclDataType.h"

#include "Engin/src/atlas/atlasengin.h"
#include "common/algUtil.h"
#include "src/common/arcterndefs_config_analyzer.h"
namespace arctern{

AtlasVehicleAttrImpl::AtlasVehicleAttrImpl():  m_extScale{0, 0, 0, 0}{
  m_modelsInputHeight = 256;
  m_modelsInputWidth = 256;
}

AtlasVehicleAttrImpl::~AtlasVehicleAttrImpl() {
  
}

ErrorType AtlasVehicleAttrImpl::GetModelsConfigInfo(const TrtModels* models){
 
  char* CarBrand;
  size_t CarBrandSize;
  std::string CarBrandIndex;
  /// parse the mxnet file_buffer to get the car_label_index,
  bool getOk = try_get_opt(*models, "CarBrand", &CarBrand, CarBrandSize);

  assert(getOk);
  CarBrandIndex = std::string(CarBrand, CarBrandSize);
  delete[] CarBrand; // need manual free
  std::istringstream carBrandIndexStream(CarBrandIndex);
  std::string CarBrandName;
  while(getline(carBrandIndexStream, CarBrandName)) {
    m_carBrandNames.push_back(CarBrandName);
  }
  return arctern::ERR_SUCCESS;
}




ErrorType AtlasVehicleAttrImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
std::vector<std::vector<float>> &outputs) {
   auto r = dynamic_cast<VehicleAttrResult *>(m_pResult);

  int total = endId - startId;

  // const auto batchNum = initParam_.batchNum_;

  for (size_t i = 0; i < outputs.size(); ++i) {
    int stride = outputs[i].size() / total;
    for (int subId = 0; subId < total; subId++) {
      const float *data0 = outputs[i].data() + subId * stride;
      std::pair<int, float> attr_res = CalculateResult(data0, stride);
      auto &attr = r->vehicleAttrs_[startId + subId];
      switch (i) {
        case 0 : {
          attr.vehicleBrand = m_carBrandNames.at(attr_res.first);
          attr.vehicleBrandScore = attr_res.second;
          break;
        }
        case 1 : {
          attr.vehicleColor = carColor.at(attr_res.first);
          attr.vehicleColorScore = attr_res.second;
          break;
        }
        case 2 : {
          attr.vehicleType = carType.at(attr_res.first);
          attr.vehicleTypeScore = attr_res.second;
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

pair<int, float> AtlasVehicleAttrImpl::CalculateResult(const float *data, int step) {
  std::vector<float> prob;
  for (size_t j = 0; j < step; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}

cv::Rect AtlasVehicleAttrImpl::get_vehicle(const int& width, const int& height, const ArcternRect &bbox) {
  float x1 = bbox.x;
  float y1 = bbox.y;
  float x2 = bbox.x + bbox.width;
  float y2 = bbox.y + bbox.height;
  auto sx1 = (int) (x1 < 0 ? 0 : x1);
  auto sy1 = (int) (y1 < 0 ? 0 : y1);
  auto sx2 = (int) (x2 < width ? x2 : width);
  auto sy2 = (int) (y2 < height ? y2 : height);
  auto ew  = (int) (sx2 - sx1);
  auto eh  = (int) (sy2 - sy1);

  return cv::Rect(sx1,sy1,ew,eh);
}
  
cv::Rect AtlasVehicleAttrImpl::LocalGetROI(const RunParameter *p, int idx){
  
  auto ret = get_vehicle(p->imageV_[idx].width,p->imageV_[idx].height, p->rectsV_[idx]);

  return ret;
}

ErrorType AtlasVehicleAttrImpl::LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
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

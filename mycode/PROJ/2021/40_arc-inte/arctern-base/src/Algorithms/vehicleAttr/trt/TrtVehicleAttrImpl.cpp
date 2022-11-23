//
// Created by Admin on 2021/6/21.
//

#include "TrtVehicleAttrImpl.h"
#include "src/common/algUtil.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
namespace arctern{
TrtVehicleAttrImpl::TrtVehicleAttrImpl() : m_extScale{0, 0, 0, 0}{
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
  m_modelsInputHeight = 256;
  m_modelsInputWidth = 256;
};

TrtVehicleAttrImpl::~TrtVehicleAttrImpl(){

};

ErrorType
TrtVehicleAttrImpl::GetModelsConfigInfo(const MxnetModels *models) {
 // auto model = models->models()->Get(0);
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
};

void TrtVehicleAttrImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
 runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::resized);
 runParm->sequence_.push_back(OperatorType::cvtColor);
 runParm->sequence_.push_back(OperatorType::normalized);

 runParm->sequence_.push_back(OperatorType::extractChannel);
}




pair<int,float> TrtVehicleAttrImpl::CalculateResult(const float *data, int step){
  std::vector<float> prob;
  for (size_t j = 0; j < step; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}


ErrorType TrtVehicleAttrImpl::LocalPostProcess(const RunParameter *p, int startId,
                                   int endId, vector<arctern::OutputTensor<float>> &outputs) {

  auto r = dynamic_cast<VehicleAttrResult *>(m_pResult);

  int total = endId - startId;

  // const auto batchNum = initParam_.batchNum_;

  for (size_t i = 0; i < outputs.size(); ++i) {
    int stride = outputs[i].size() / outputs[i].shape()[0];
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
};

ArcternRect TrtVehicleAttrImpl::get_vehicle(const int& width, const int& height, const ArcternRect &bbox) {
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
  ArcternRect ret;
  ret.x = sx1;
  ret.y = sy1;
  ret.width = ew;
  ret.height = eh;
  return ret;
}

ArcternRect TrtVehicleAttrImpl::LocalGetROI(const RunParameter *p, int idx){
  auto rect = get_vehicle(p->imageV_[idx].width, p->imageV_[idx].height, p->rectsV_[idx]);
  return rect;
}


}
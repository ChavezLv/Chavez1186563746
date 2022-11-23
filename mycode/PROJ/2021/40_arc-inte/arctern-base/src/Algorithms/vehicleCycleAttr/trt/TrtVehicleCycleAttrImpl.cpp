//
// Created by Admin on 2021/6/21.
//

#include "TrtVehicleCycleAttrImpl.h"
#include "src/common/algUtil.h"
namespace arctern{
TrtVehicleCycleAttrImpl::TrtVehicleCycleAttrImpl() : m_extScale{0.1, 0.4, 0.1, 0}{
  m_modelsInputHeight = 224;
  m_modelsInputWidth = 128;
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
};

TrtVehicleCycleAttrImpl::~TrtVehicleCycleAttrImpl(){

};

ArcternRect TrtVehicleCycleAttrImpl::GetVehicle(const int& width, const int& height, const ArcternRect &bbox){
  float x1 = bbox.x;
  float y1 = bbox.y;
  float x2 = bbox.x + bbox.width;
  float y2 = bbox.y + bbox.height;
  float bw = x2 - x1;
  float bh = y2 - y1;
  x1 = x1 - 0.1 * bw;
  y1 = y1 - 0.4 * bh;
  x2 = x2 + 0.1 * bw;
  auto sx1 = (int) (x1 < 0 ? 0 : x1);
  auto sy1 = (int) (y1 < 0 ? 0 : y1);
  auto sx2 = (int) (x2 < width - 1 ? x2 : width - 1);
  auto sy2 = (int) (y2 < height - 1 ? y2 : height - 1);
  auto ew  = (int) (sx2 - sx1 + 1);
  auto eh  = (int) (sy2 - sy1 + 1);
  ArcternRect vehicle_rect{(float )sx1, (float )sy1, (float )ew, (float )eh};
  return vehicle_rect;
}

ErrorType
TrtVehicleCycleAttrImpl::GetModelsConfigInfo(const MxnetModels *models) {
  return ErrorType::ERR_SUCCESS;
};

void TrtVehicleCycleAttrImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
 runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::resized);
 runParm->sequence_.push_back(OperatorType::cvtColor);
 runParm->sequence_.push_back(OperatorType::normalized);
 runParm->sequence_.push_back(OperatorType::extractChannel);
}




pair<int,float> TrtVehicleCycleAttrImpl::CalculateResult(const float *data, int step){
  std::vector<float> prob;
  for (size_t j = 0; j < step; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  max_score = *max_element(prob.begin(), prob.end());
  return std::make_pair(max_idx, max_score);
}


ErrorType TrtVehicleCycleAttrImpl::LocalPostProcess(const RunParameter *p, int startId,
                                   int endId, vector<arctern::OutputTensor<float>> &outputs) {
  auto r = dynamic_cast<VehicleCycleAttrResult *>(m_pResult);

  int total = endId - startId;

  for (size_t i = 0; i < outputs.size(); ++i) {
    int stride = outputs[i].size() / outputs[i].shape()[0];
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
};




ArcternRect TrtVehicleCycleAttrImpl::LocalGetROI(const RunParameter *p, int idx){

  auto rect = GetVehicle(p->imageV_[idx].width,p->imageV_[idx].height,p->rectsV_[idx]);
  return rect;
}


}
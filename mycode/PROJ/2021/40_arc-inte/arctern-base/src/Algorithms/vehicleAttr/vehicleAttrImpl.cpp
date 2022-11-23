/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.08
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#include <utility>
#include <vector>
#include "vehicleAttrImpl.h"
#include "common/file_buffer.h"
#include "common/imageConvert.h"
#include "common/algUtil.h"
#include "common/arcterndefs_config_analyzer.h"
#include "log/logger.h"

using arctern::VehicleAttrImpl;
using arctern::ErrorType;
using arctern::Result;
using arctern::carColor;
using arctern::carType;
using arctern::ArcternRect;

VehicleAttrImpl::VehicleAttrImpl() : extScale_{0,0,0,0} {
  input_h_ = 256;
  input_w_ = 256;
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_RGB888;
}

VehicleAttrImpl::~VehicleAttrImpl() noexcept {
  VehicleAttrImpl::Release();
}

ErrorType VehicleAttrImpl::CheckRunParam(const RunParameter *p) {
  return ArcternAlgorithm::CheckRunParam(p);
}

ErrorType VehicleAttrImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                        std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<VehicleAttrResult *>(result_);

  int total = endId - startId;

  // const auto batchNum = initParam_.batchNum_;

  for (size_t i = 0; i < outputs.size(); ++i) {
    int stride = outputs[i].size() / outputs[i].shape()[0];
    for (int subId = 0; subId < total; subId++) {
      const float *data0 = outputs[i].data() + subId * stride;
      std::pair<int, float> attr_res = calcAttr(data0, stride);
      auto &attr = r->vehicleAttrs_[startId + subId];
      switch (i) {
        case 0 : {
          attr.vehicleBrand = carBrandNames_.at(attr_res.first);
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

/// \brief calculate mask label and corresponding score
std::pair<int, float> VehicleAttrImpl::calcAttr(const float *data, size_t num_class) {
  std::vector<float> prob;
  for (size_t j = 0; j < num_class; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}

int VehicleAttrImpl::GetModelsConfigInfo(arctern::MxnetModels *model) {
  char* CarBrand;
  size_t CarBrandSize;
  std::string CarBrandIndex;
  /// parse the mxnet file_buffer to get the car_label_index,
  bool getOk = try_get_opt(*model, "CarBrand", &CarBrand, CarBrandSize);
  //ARCTERN_ASSERT_MSG(getOk, "get CarBrandIndex failed!");
  assert(getOk);
  CarBrandIndex = std::string(CarBrand, CarBrandSize);
  delete[] CarBrand; // need manual free
  std::istringstream carBrandIndexStream(CarBrandIndex);
  std::string CarBrandName;
  while(getline(carBrandIndexStream, CarBrandName)) {
    carBrandNames_.push_back(CarBrandName);
  }
  //initPreProcessor();
  return 0;
}

ArcternRect VehicleAttrImpl::LocalGetRoi(const RunParameter *p, int idx)  {
  const auto implRunP = dynamic_cast<const VehicleAttrRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  cv::Rect rect = get_face(p->imageV_[idx].width, p->imageV_[idx].height, oriRect, extScale_, Float2Int::TRUNCATION, false);
  ArcternRect result;
  result.x = rect.x;
  result.y = rect.y;
  result.width = rect.width;
  result.height = rect.height;
  return result;
}
void VehicleAttrImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::normalized);
}
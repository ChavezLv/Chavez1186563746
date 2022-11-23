/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.09
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#include <utility>
#include <vector>
#include "vehicleCycleAttrImpl.h"
#include "common/file_buffer.h"
#include "common/imageConvert.h"
#include "common/algUtil.h"
#include "common/arcterndefs_config_analyzer.h"
#include "log/logger.h"

using arctern::VehicleCycleAttrImpl;
using arctern::ErrorType;
using arctern::Result;
using arctern::ArcternRect;

VehicleCycleAttrImpl::VehicleCycleAttrImpl() : extScale_{0.1, 0.4, 0.1, 0} {
  input_h_ = 224;
  input_w_ = 128;
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_BGR888;
}

VehicleCycleAttrImpl::~VehicleCycleAttrImpl() noexcept {
  VehicleCycleAttrImpl::Release();
}


ErrorType VehicleCycleAttrImpl::CheckRunParam(const RunParameter *p) {
  return ArcternAlgorithm::CheckRunParam(p);
}

ErrorType VehicleCycleAttrImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                                 std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<VehicleCycleAttrResult *>(result_);

  int total = endId - startId;

  for (size_t i = 0; i < outputs.size(); ++i) {
    int stride = outputs[i].size() / outputs[i].shape()[0];
    for (int subId = 0; subId < total; subId++) {
      const float *data0 = outputs[i].data() + subId * stride;
      std::pair<int, float> attr_res = calcAttr(data0, stride);
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

/// \brief calculate mask label and corresponding score
std::pair<int, float> VehicleCycleAttrImpl::calcAttr(const float *data, size_t num_class) {
  std::vector<float> prob;
  for (size_t j = 0; j < num_class; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  max_score = *max_element(prob.begin(), prob.end());
  return std::make_pair(max_idx, max_score);
}

/** get region of interest **/
cv::Rect VehicleCycleAttrImpl::get_vehicle(const int &width, const int &height, const cv::Rect &bbox) {
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

ArcternRect VehicleCycleAttrImpl::LocalGetRoi(const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const VehicleCycleAttrRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  //cv::Rect rect = get_face(image.cols, image.rows, oriRect, extScale_, Float2Int::TRUNCATION, false);
  cv::Rect rect = get_vehicle(p->imageV_[idx].width, p->imageV_[idx].height, oriRect);
  ArcternRect result;
  result.x = rect.x;
  result.y = rect.y;
  result.width = rect.width;
  result.height = rect.height;
  return result;
}
void VehicleCycleAttrImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::normalized);

}
//
// Created by chengaohua on 2021/7/2.
//
#include "trtVehicleCycleFeatImpl.h"
#include "../vehicleCycleFeat.h"
#include "common/algUtil.h"
#include "common/featureUtil.h"

namespace arctern {
TrtVehicleCycleFeatImpl::TrtVehicleCycleFeatImpl() {
  m_modelsInputHeight = 192;
  m_modelsInputWidth = 192;
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
}

TrtVehicleCycleFeatImpl::~TrtVehicleCycleFeatImpl() throw() {

}

ErrorType TrtVehicleCycleFeatImpl::GetModelsConfigInfo(const MxnetModels *models) {
  return ERR_SUCCESS;
}

ErrorType TrtVehicleCycleFeatImpl::LocalPostProcess(const RunParameter *p,
                                                    int startId,
                                                    int endId,
                                                    vector<arctern::OutputTensor<float>> &outputs) {
  auto res = dynamic_cast<VehicleCycleFeatResult *>(m_pResult);
  int total = endId - startId;
  int stride = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data = outputs[0].data() + subId * stride;
    std::vector<float> data_cls(stride);
    memcpy(data_cls.data(), data, stride * sizeof(float));

    l2norm(data_cls);

    res->m_vFeatResults[startId + subId] = encrypt_feature(data_cls);
  }

  return ErrorType::ERR_SUCCESS;
}

ArcternRect TrtVehicleCycleFeatImpl::LocalGetROI(const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const VehicleCycleFeatRunParameter *>(p);
  auto &arct_rect = implRunP->rectsV_[idx];
  cv::Rect ori_rect(arct_rect.x, arct_rect.y, arct_rect.width,
                    arct_rect.height);
  cv::Rect bbox_rect = check_shrink_box(ori_rect, p->imageV_[idx].width, p->imageV_[idx].height);
  ArcternRect  result;
  result.x = bbox_rect.x;
  result.y = bbox_rect.y;
  result.width = bbox_rect.width;
  result.height = bbox_rect.height;
  return result;
}

void TrtVehicleCycleFeatImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}

}
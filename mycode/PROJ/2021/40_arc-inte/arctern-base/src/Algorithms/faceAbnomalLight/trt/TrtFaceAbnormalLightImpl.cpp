//
// Created by Admin on 2021/5/19.
//
#include "TrtFaceAbnormalLightImpl.h"
#include "src/common/common.h"
namespace arctern {

TrtFaceAbnormalLightImpl::TrtFaceAbnormalLightImpl() {

}

TrtFaceAbnormalLightImpl::~TrtFaceAbnormalLightImpl() {

}

ErrorType TrtFaceAbnormalLightImpl::GetModelsConfigInfo(const MxnetModels *models) {
  int dim = 0;
  auto model = models->models()->Get(0);
  std::string key = "INPUT_DIM";
  bool ret = try_get_value<int>(*model, key, dim);

  assert(ret == true);

  m_modelsInputHeight = dim;
  m_modelsInputWidth = dim;

  return ERR_SUCCESS;
}


ErrorType TrtFaceAbnormalLightImpl::LocalPostProcess(const RunParameter *p,
                                                     int startId,
                                                     int endId,
                                                     vector<arctern::OutputTensor<float>> &outputs) {
  auto result = dynamic_cast<FaceAbnomalLightResult *>(m_pResult);
  int total = endId - startId;
  int stride0 = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    auto algResult = CalculateResult(data0, stride0);

    result->faceAbnomalLights_[startId + subId].type = AbnomalLightType(algResult.first);
    result->faceAbnomalLights_[startId + subId].score = algResult.second;
  }

  return ErrorType::ERR_SUCCESS;
}
void TrtFaceAbnormalLightImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::crop);
 // runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}
ArcternRect TrtFaceAbnormalLightImpl::LocalGetROI(const RunParameter *p, int idx) {
  const auto pRunParam = dynamic_cast<const RunParameter *>(p);
  auto &faceRect = pRunParam->rectsV_[idx];
  return check_shrink_box(faceRect, p->imageV_[idx].width, p->imageV_[idx].height);
}

ArcternRect TrtFaceAbnormalLightImpl::check_shrink_box(const ArcternRect& box, int image_width,int image_height) {
  int x1 = (int) box.x;
  int y1 = (int) box.y;
  int x2 = (int) (x1 + box.width);
  int y2 = (int) (y1 + box.height);

  x1 = std::max(0, std::min(x1, image_width));
  y1 = std::max(0, std::min(y1, image_height));
  x2 = std::max(0, std::min(x2, image_width));
  y2 = std::max(0, std::min(y2, image_height));

  ArcternRect ret;
  ret.x = x1;
  ret.y = y1;
  ret.width = x2 - x1;
  ret.height = y2 - y1;
  return ret;
}

pair<int, float> TrtFaceAbnormalLightImpl::CalculateResult(const float *data, int step) {
  auto biggest = std::max_element(data, data + step);
  int typeId = std::distance(data, biggest);
  return std::make_pair(typeId, *biggest);
}

}

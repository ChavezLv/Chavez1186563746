//
// Created by Admin on 2021/6/18.
//

#include "TrtFaceTypeImpl.h"
#include "src/common/common.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
namespace arctern {

TrtFaceTypeImpl::TrtFaceTypeImpl():m_extScale{-1.0f, 0.2f, -1.0f, 0.1f} {
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
}

TrtFaceTypeImpl::~TrtFaceTypeImpl() {

}

ErrorType
TrtFaceTypeImpl::GetModelsConfigInfo(const MxnetModels *models) {

  m_modelsInputHeight = 128;
  m_modelsInputWidth = 128;
  return ERR_SUCCESS;
}

ErrorType TrtFaceTypeImpl::LocalPostProcess(const RunParameter *p,
                                            int startId,
                                            int endId,
                                            vector<arctern::OutputTensor<float>> &outputs) {
  auto r = dynamic_cast<FaceTypeResult *>(m_pResult);

  int i_total = endId - startId;
  int i_stride = outputs[0].size() / outputs[0].shape()[0];
  for (int subId = 0; subId < i_total; subId++) {
    const float *p_data = outputs[0].data() + subId * i_stride;
    auto type_res = CalculateResult(p_data, i_stride);
    r->m_vTypeResults[startId + subId].first = type_res.first;
    r->m_vTypeResults[startId + subId].second = type_res.second;
  }

  return ErrorType::ERR_SUCCESS;
}
ArcternRect TrtFaceTypeImpl::GetFace(const int &width, const int &height,
                               const ArcternRect &bbox) {
  int w = bbox.width + 1;
  int h = bbox.height + 1;
  float x1 = bbox.x;
  float y1 = bbox.y;
  float x2 = x1 + bbox.width;
  float y2 = y1 + bbox.height;

  y1 = y1 - h * m_extScale[1];
  y2 = y2 + h * m_extScale[3];

  float by1 = std::min(std::max(0, static_cast<int>(y1 + 0.5)), height - 1);
  float by2 = std::min(std::max(0, static_cast<int>(y2 + 0.5)), height - 1);
  float hh = by2 - by1 + 1;
  float ww = hh;

  if (ww > w) {
    x1 = x1 - (ww - w) / 2.f;
    x2 = x2 + (ww - w) / 2.f;
  }

  float bx1 = std::min(std::max(0, static_cast<int>(x1 + 0.5f)), width - 1);
  float bx2 = std::min(std::max(0, static_cast<int>(x2 + 0.5f)), width - 1);


  ArcternRect ret;
  ret.x=bx1;
  ret.y=by1;
  ret.width = bx2 - bx1;
  ret.height = by2 -by1;
  return ret;
}

ArcternRect TrtFaceTypeImpl::LocalGetROI(const RunParameter *p, int idx) {

  int endPos = m_initParam.model_path.find_last_of(".") ;
  m_version = m_initParam.model_path.substr(endPos-5,5);
  if(m_version == "0.1.0"){
    m_extScale[1] = 0.5;
  }

  const auto pRunParam = dynamic_cast<const RunParameter *>(p);
  auto &faceRect = pRunParam->rectsV_[idx];
  auto rect = CheckShrinkBox(faceRect,p->imageV_[idx].width, p->imageV_[idx].height);
  rect = TrtFaceTypeImpl::GetFace(p->imageV_[idx].width, p->imageV_[idx].height, rect);
  return rect;
}

void TrtFaceTypeImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}

pair<int, float> TrtFaceTypeImpl::CalculateResult(const float *data, int step) {
  if (nullptr == data || step <= 0) {
    return std::make_pair(-1, 0.f);
  }
  std::vector<float> v_prob(step);
  memcpy(v_prob.data(), data, step * sizeof(float));

  float f_max_val = 0.f;
  int i_max_idx;
  if(m_version == "0.1.0"){
    i_max_idx = get_label(v_prob, f_max_val, 0.2434);
  }
  else{
    i_max_idx = softmax(v_prob, f_max_val);
  }

  return std::make_pair(i_max_idx, f_max_val);
}

}

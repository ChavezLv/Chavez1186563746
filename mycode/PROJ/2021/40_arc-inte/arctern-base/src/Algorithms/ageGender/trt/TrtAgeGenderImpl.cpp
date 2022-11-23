//
// Created by Admin on 2021/6/18.
//

#include "TrtAgeGenderImpl.h"
#include "src/common/common.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
#include "../utils/AgeGenderUtil.h"
namespace arctern {

TrtAgeGenderImpl::TrtAgeGenderImpl(){
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
}

TrtAgeGenderImpl::~TrtAgeGenderImpl() {

}

void TrtAgeGenderImpl::CreateResult() {
  m_pResult = make_unique<AgeGenderResult>();
}

ErrorType TrtAgeGenderImpl::GetModelsConfigInfo(const MxnetModels *models) {

  m_modelsInputHeight = 128;
  m_modelsInputWidth = 112;
  m_extScale[0] = 0.0;
  m_extScale[2] = 0.0;
  auto model = models->models()->Get(0);
  if(model->version()->str() == "0.1.0") {
    m_extScale[1] = 0.3;
    m_extScale[3] = 0.3;
  } else {
    m_extScale[1] = 0.5;
    m_extScale[3] = 0.1;
  }
  return ErrorType::ERR_SUCCESS;
}

ErrorType TrtAgeGenderImpl::LocalPostProcess(const RunParameter *p,
                                             int startId,
                                             int endId,
                                             vector<arctern::OutputTensor<float>> &outputs) {
    auto r = dynamic_cast<AgeGenderResult *>(m_pResult);

    int total = endId - startId;

    int stride0 = outputs[0].size() / outputs[0].shape()[0];
    int stride1 = outputs[1].size() / outputs[1].shape()[0];

    for (int subId = 0; subId < total; subId++) {
      const float *data0 = outputs[0].data() + subId * stride0;
      const float *data1 = outputs[1].data() + subId * stride1;

      auto gender = calc_gender(data0, stride0);
      auto age = calc_age(data1, stride1);

      r->ageGenders_[startId + subId].age = age.first;
      r->ageGenders_[startId + subId].ageScore = age.second;
      r->ageGenders_[startId + subId].gender = gender.first == 0 ? MALE : FEMALE;
      r->ageGenders_[startId + subId].genderScore = gender.second;
    }

    return ErrorType::ERR_SUCCESS;
}

ArcternRect TrtAgeGenderImpl::GetFace(int width, int height,
                                       const ArcternRect &bbox) {
  float x1 = bbox.x;
  float y1 = bbox.y;
  float x2 = bbox.x + bbox.width;
  float y2 = bbox.y + bbox.height;
  float w = x2 - x1 + 1;
  float h = y2 - y1 + 1;

  y1 = y1 - h * m_extScale[1];
  y2 = y2 + h * m_extScale[3];
  float by1 = std::min(std::max(0, static_cast<int>(y1)), height);
  float by2 = std::min(std::max(0, static_cast<int>(y2)), height);
  float hh = by2 - by1 + 1;
  float ww = hh;
  if (ww > w) {
    int wx = static_cast<int>((ww - w) / 2);
    x1 = x1 - wx;
    x2 = x2 + wx;
  }

  float bx1 = std::min(std::max(0, static_cast<int>(x1)), width);
  float bx2 = std::min(std::max(0, static_cast<int>(x2)), width);

  ArcternRect ret{static_cast<float >(bx1),
                  static_cast<float >(by1),
                  static_cast<float >(bx2 - bx1),
                  static_cast<float >(by2 - by1)};
  return ret;
}

ArcternRect TrtAgeGenderImpl::LocalGetROI(const RunParameter *p, int idx) {
  const auto pRunParam = dynamic_cast<const RunParameter *>(p);
  auto &faceRect = pRunParam->rectsV_[idx];
  auto rect = GetFace(p->imageV_[idx].width, p->imageV_[idx].height, faceRect);
  return rect;
}

void TrtAgeGenderImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}

pair<int, float> TrtAgeGenderImpl::CalculateResult(const float *data, int step) {
  std::vector<float> prob;
  for (size_t j = 0; j < step; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}

}

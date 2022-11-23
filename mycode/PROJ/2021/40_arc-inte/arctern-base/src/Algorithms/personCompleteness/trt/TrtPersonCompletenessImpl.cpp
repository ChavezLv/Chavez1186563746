//
// Created by Admin on 2021/6/21.
//

#include "TrtPersonCompletenessImpl.h"
#include "src/common/algUtil.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
#include "../utils/personCompletenessUtil.h"
namespace arctern{
TrtPersonCompletenessImpl::TrtPersonCompletenessImpl() : m_extScale{0, 0, 0, 0}{
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
};

TrtPersonCompletenessImpl::~TrtPersonCompletenessImpl(){

};

void TrtPersonCompletenessImpl::InitPreprocessorParam(){
  m_pStream = m_vEngines[0]->getContext()->getCudaStream();
  m_pPreprocessor.reset(new NppPreprocessor(m_pStream));
  NppPreprocessorInitPt pPreprocessorInitParam ;

  pPreprocessorInitParam.netH_ = m_modelsInputHeight;
  pPreprocessorInitParam.netW_ = m_modelsInputWidth;
  pPreprocessorInitParam.meanV_ = {0.4914, 0.4822,0.4465}; // rgb
  pPreprocessorInitParam.stdV_ = {0.2023, 0.1994, 0.2010};  // rgb

  assert(m_pPreprocessor);
  m_lastErrorCode = m_pPreprocessor->Init(&pPreprocessorInitParam);
  assert(m_lastErrorCode == ErrorType::ERR_SUCCESS);
}

ErrorType
TrtPersonCompletenessImpl::GetModelsConfigInfo(const MxnetModels *models) {
  
  m_modelsInputWidth = 128;
  m_modelsInputHeight = 256;
  return  ErrorType::ERR_SUCCESS;
};

void TrtPersonCompletenessImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::splitNormalized);

  //runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}

ErrorType TrtPersonCompletenessImpl::LocalPostProcess(const RunParameter *p, int startId,
                                   int endId, vector<arctern::OutputTensor<float>> &outputs) {
  auto r = dynamic_cast<PersonCompletenessResult *>(m_pResult);
  int total = endId - startId;
  int stride0 = outputs[0].size() / outputs[0].shape()[0]; // 2
  int stride1 = outputs[1].size() / outputs[1].shape()[0]; // 5
  int stride2 = 128;

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    const float *data1 = outputs[1].data() + subId * stride1;
    const float *data2 = outputs[2].data() + subId * stride2;
    parse_truncation(data0, stride0, r->personCompleteness_[startId + subId].truncation);
    parse_visible_parts(data1, stride1,
                        r->personCompleteness_[startId + subId].visibleParts,
                        r->personCompleteness_[startId + subId].visiblePartsScore);
    parse_soft_mask(data2, stride2,
                    r->personCompleteness_[startId + subId].softMasks,
                    r->personCompleteness_[startId + subId].softMasksScore);
  }

  return ErrorType::ERR_SUCCESS;
};

ArcternRect TrtPersonCompletenessImpl::GetROI(const int &img_width,const int &img_height,
                                              const ArcternRect &bbox,
                                              const std::array<float, 4> ext_scale) const {
  int height = img_height;
  int width = img_width;
  int w = bbox.width;
  int h = bbox.height;
  float x1 = bbox.x;
  float y1 = bbox.y;
  float x2 =bbox.width + bbox.x;
  float y2 =bbox.height + bbox.y;

  x1 -= w * ext_scale[0];
  y1 -= h * ext_scale[1];
  x2 += w * ext_scale[2];
  y2 += h * ext_scale[3];
  float bx1 = std::min(std::max(0, static_cast<int>(round(x1))), width - 1);
  float by1 = std::min(std::max(0, static_cast<int>(round(y1))), height - 1);
  float bx2 = std::min(std::max(0, static_cast<int>(round(x2))), width - 1);
  float by2 = std::min(std::max(0, static_cast<int>(round(y2))), height - 1);
  ArcternRect ret;
  ret.x = bx1;
  ret.y = by1;
  ret.width = bx2 + 1 - bx1;
  ret.height = by2 + 1 - by1;
  return ret;
}

ArcternRect TrtPersonCompletenessImpl::LocalGetROI(const RunParameter *p, int idx){
  const auto pRunParam = dynamic_cast<const PersonCompletenessRunParameter *>(p);
  auto &faceRect = pRunParam->rectsV_[idx];
  auto rect = CheckShrinkBox(faceRect, p->imageV_[idx].width, p->imageV_[idx].height);
  rect = TrtPersonCompletenessImpl::GetROI(p->imageV_[idx].width, p->imageV_[idx].height, rect ,m_extScale);
  return rect;
}


}
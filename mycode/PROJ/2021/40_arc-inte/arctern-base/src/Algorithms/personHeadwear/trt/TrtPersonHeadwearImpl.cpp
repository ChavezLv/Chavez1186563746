//
// Created by chengaohua on 2021/6/24.
//

#include "TrtPersonHeadwearImpl.h"
#include "../personHeadwear.h"
#include "../utils/personHeadwearUtil.h"
#include "common/algUtil.h"

namespace arctern {
TrtPersonHeadwearImpl::TrtPersonHeadwearImpl() : extScale_{0, 0.15, 0, 0} {
  m_modelsInputWidth = 128;
  m_modelsInputHeight = 320;
 // m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;

}

TrtPersonHeadwearImpl::~TrtPersonHeadwearImpl() throw() {

}

ErrorType TrtPersonHeadwearImpl::GetModelsConfigInfo(const MxnetModels *models) {
  return ErrorType::ERR_SUCCESS;
}

ErrorType TrtPersonHeadwearImpl::LocalPostProcess(const RunParameter *p,
                                                  int startId,
                                                  int endId,
                                                  vector<arctern::OutputTensor<float>> &outputs) {
  auto r = dynamic_cast<PersonHeadwearResult *>(m_pResult);

  int total = endId - startId;

  // const auto batchNum = initParam_.batchNum_;
  int stride0 = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    auto headwear = personHeadwear::calcHeadwear(data0, stride0);

    r->PersonHeadwears_[startId + subId].headwearType = PersonHeadwearType(headwear.first);
    r->PersonHeadwears_[startId + subId].typeScore = headwear.second;
  }

  return ErrorType::ERR_SUCCESS;
}

ArcternRect TrtPersonHeadwearImpl::LocalGetROI(const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const PersonHeadwearRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  auto rect = check_shrink_box(oriRect, p->imageV_[idx].width , p->imageV_[idx].height);
  rect = get_face(p->imageV_[idx].width, p->imageV_[idx].height, rect, extScale_, Float2Int::TRUNCATION, false);

  ArcternRect  result;
  result.x = rect.x;
  result.y = rect.y;
  result.width = rect.width;
  result.height = rect.height;
  return result;

}

void TrtPersonHeadwearImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}
}
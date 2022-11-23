//
// Created by Admin on 2021/6/18.
//

#include "TrtPersonUmbrellaImpl.h"
#include "src/common/common.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
namespace arctern {

TrtPersonUmbrellaImpl::TrtPersonUmbrellaImpl() : m_extScale{0, 0.2, 0, 0}{
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
}

TrtPersonUmbrellaImpl::~TrtPersonUmbrellaImpl() {

}

ErrorType
TrtPersonUmbrellaImpl::GetModelsConfigInfo(const MxnetModels *models) {

  m_modelsInputHeight = 256;
  m_modelsInputWidth = 128;

  return ErrorType::ERR_SUCCESS;
}

ErrorType TrtPersonUmbrellaImpl::LocalPostProcess(const RunParameter *p,
                                                     int startId,
                                                     int endId,
                                                     vector<arctern::OutputTensor<float>> &outputs) {
  PersonUmbrellaResult *p_res = dynamic_cast<PersonUmbrellaResult *>(m_pResult);

  int i_total = endId - startId;
  int i_stride = outputs[0].size() / outputs[0].shape()[0];
  for (int subId = 0; subId < i_total; subId++) {
    const float *p_data = outputs[0].data() + subId * i_stride;
    auto type_res = CalculateResult(p_data, i_stride);
    p_res->m_vClassifyResults[startId + subId].first = type_res.first;
    p_res->m_vClassifyResults[startId + subId].second = type_res.second;
  }

  return ErrorType::ERR_SUCCESS;
}
ArcternRect TrtPersonUmbrellaImpl::CheckRect(const int width, const int height,
                                   const ArcternRect bbox) {
  float x1 = bbox.x;
  float y1 = bbox.y;
  float x2 = bbox.x + bbox.width;
  float y2 = bbox.y + bbox.height;
  auto sx1 = (int) (x1 < 0 ? 0 : x1);
  auto sy1 = (int) (y1 < 0 ? 0 : y1);
  auto sx2 = (int) (x2 <= width ? x2 : width);
  auto sy2 = (int) (y2 <= height ? y2 : height);
  y1 = (int) (sy1 - 0.15 * (sy2 - sy1));
  sy1 = (int) (y1 < 0 ? 0 : y1);
  x1 = (int) (sx1 - 0.10 * (sx2 - sx1));
  sx1 = (int) (x1 < 0 ? 0 : x1);
  x2 = (int) (sx2 + 0.10 * (sx2 - sx1));
  sx2 = (int) (x2 <= width ? x2 : width);
  auto ew  = (int) (sx2 - sx1);
  auto eh  = (int) (sy2 - sy1);

  ArcternRect ret;
  if (ew > 0 && eh > 0) {
    ret.x = sx1;
    ret.y = sy1;
    ret.width = ew;
    ret.height = eh;

  }
  else {
    ret.x = 0;
    ret.y = 0;
    ret.width = width;
    ret.height = height;
  }
  return ret;
}

ArcternRect TrtPersonUmbrellaImpl::LocalGetROI(const RunParameter *p, int idx) {
  const auto pRunParam = dynamic_cast<const RunParameter *>(p);
  auto &faceRect = pRunParam->rectsV_[idx];
  auto rect = CheckRect(p->imageV_[idx].width, p->imageV_[idx].height, faceRect);
  return rect;
}

void TrtPersonUmbrellaImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}

pair<int, float> TrtPersonUmbrellaImpl::CalculateResult(const float *data, int step) {
  if (nullptr == data || step <= 0) {
    return std::make_pair(-1, 0.f);
  }

  std::vector<float> v_prob(step);
  memcpy(v_prob.data(), data, step * sizeof(float));

  float f_max_val = 0.f;
  int i_max_idx = softmax(v_prob, f_max_val);

  return std::make_pair(i_max_idx, f_max_val);
}

}

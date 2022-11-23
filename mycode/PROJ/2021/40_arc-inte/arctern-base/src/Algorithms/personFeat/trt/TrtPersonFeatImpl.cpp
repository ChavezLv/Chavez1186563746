//
// Created by chengaohua on 2021/6/22.
//

#include <Algorithms/personFeat/personFeat.h>
#include "TrtPersonFeatImpl.h"
#include "common/algUtil.h"
#include "common/featureUtil.h"

namespace arctern {

TrtPersonFeatImpl::TrtPersonFeatImpl() {
  m_modelsInputHeight = 384;
  m_modelsInputWidth = 128;
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
}

TrtPersonFeatImpl::~TrtPersonFeatImpl() throw() {

}

ErrorType TrtPersonFeatImpl::GetModelsConfigInfo(const MxnetModels *models) {
  auto model = models->models()->Get(0);
  int input_dim_w, input_dim_h;
  bool ret = try_get_value<int>(*model, "w", input_dim_w);
  ret &= try_get_value(*model, "h", input_dim_h);
  if(ret) {
    m_modelsInputHeight = input_dim_h;
    m_modelsInputWidth = input_dim_w;
  }

  return ERR_SUCCESS;
}


ErrorType TrtPersonFeatImpl::LocalPostProcess(const RunParameter *p,
                                              int startId,
                                              int endId,
                                              vector<arctern::OutputTensor<float>> &outputs) {
  assert(p != nullptr);
  assert(startId >= 0);
  auto r = dynamic_cast<PersonFeatResult *>(m_pResult);

  int total = endId - startId;

  int stride = outputs[0].size() / outputs[0].shape()[0];
  int offset = 0;

  for (int subId = 0; subId < total; subId++) {
    float *data = outputs[0].data() + stride * subId;
    std::vector<float> data_cls(stride);
    for (size_t i = 0; i < data_cls.size(); ++i) {
      data_cls[i] = data[i];
    }

    arctern::l2norm(data_cls);

    r->features_[startId + subId] = encryptFeature(data_cls);

    offset += stride;
  }

  return ErrorType::ERR_SUCCESS;
}

ArcternRect TrtPersonFeatImpl::LocalGetROI(const RunParameter *p, int idx) {
  const auto implRunP = dynamic_cast<const PersonFeatRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);
  auto rect = check_shrink_box(oriRect, p->imageV_[idx].width, p->imageV_[idx].height);
  ArcternRect result;
  result.x = rect.x;
  result.y = rect.y;
  result.width = rect.width;
  result.height = rect.height;
  return result;
}

std::vector<uint8> TrtPersonFeatImpl::encryptFeature(const std::vector<float> &feat) {
  return encrypt_feature(feat);
}

void TrtPersonFeatImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}

}

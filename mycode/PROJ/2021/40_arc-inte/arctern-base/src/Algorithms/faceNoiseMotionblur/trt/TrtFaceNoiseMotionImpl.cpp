//
// Created by Admin on 2021/6/18.
//

#include "TrtFaceNoiseMotionImpl.h"
#include "src/common/common.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
namespace arctern {

TrtFaceNoiseMotionImpl::TrtFaceNoiseMotionImpl() {

}

TrtFaceNoiseMotionImpl::~TrtFaceNoiseMotionImpl() {

}

ErrorType
TrtFaceNoiseMotionImpl::GetModelsConfigInfo(const MxnetModels *models) {
  ErrorType vResult;

  int dim = 0;
  auto model = models->models()->Get(0);
  auto ret = try_get_value(*model, "INPUT_DIM", dim);
  assert(ret == true);

  try_get_value(*model, "USEFUL_POS", m_USE_POS);

  m_modelsInputWidth = dim;
  m_modelsInputHeight = dim;

  return vResult;
}

ErrorType TrtFaceNoiseMotionImpl::LocalPostProcess(const RunParameter *p,
                                                   int startId,
                                                   int endId,
                                                   vector<arctern::OutputTensor<float>> &outputs) {
  auto r = dynamic_cast<FaceNoiseMotionblurResult *>(m_pResult);
  int total = endId - startId;
  int stride0 = outputs[m_USE_POS].size() / outputs[m_USE_POS].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[m_USE_POS].data() + subId * stride0;
    auto ALR = CalculateResult(data0, stride0);

    r->faceNoiseMotionblurs_[startId + subId].type = NoiseType(ALR.first);
    r->faceNoiseMotionblurs_[startId + subId].score = ALR.second;
  }

  return ErrorType::ERR_SUCCESS;
}
ArcternRect TrtFaceNoiseMotionImpl::GetFace( int img_width, int img_height,const ArcternRect &face) {
  float x1 = face.x;
  float y1 = face.y;
  int width = face.width;
  int height = face.height;

  const float old_size = (width + height) / 2.0f;
  const float center_x = x1 + width / 2.0f;
  const float center_y = y1 + height / 2.0f;

  int size = (width < 64 || height < 64) ? static_cast<int>(old_size * 1.5f) : static_cast<int>(old_size * 0.9f);

  float halfsize = size / 2.0f;
  int sx = arctern::python_round(center_x - 0.9f * halfsize);
  int sy = arctern::python_round(center_y - halfsize);
  int ex = arctern::python_round(center_x + 0.9f * halfsize);
  int ey = arctern::python_round(center_y + halfsize);

  sx = std::max(0, sx);
  sy = std::max(0, sy);
  ex = std::min(ex, img_width - 1);
  ey = std::min(ey, img_height - 1);

  ArcternRect ret;
  ret.x = sx;
  ret.y = sy;
  ret.width = ex - sx;
  ret.height = ey - sy;
  return ret;
}
ArcternRect TrtFaceNoiseMotionImpl::LocalGetROI(const RunParameter *p, int idx) {
  const auto pRunParam = dynamic_cast<const RunParameter *>(p);
  auto &faceRect = pRunParam->rectsV_[idx];
  auto rect = TrtFaceNoiseMotionImpl::GetFace(p->imageV_[idx].width, p->imageV_[idx].height, faceRect);
  return rect;
}

void TrtFaceNoiseMotionImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::crop);
  //runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::twiceResized);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}

pair<int, float> TrtFaceNoiseMotionImpl::CalculateResult(const float *data, int step) {
  auto biggest = std::max_element(data, data + step);
  auto prob = *biggest;
  int classId = std::distance(data, biggest);
  return std::make_pair(classId, prob);
}

}

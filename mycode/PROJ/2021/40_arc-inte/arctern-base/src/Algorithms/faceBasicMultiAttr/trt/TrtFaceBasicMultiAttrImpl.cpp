//
// Created by Admin on 2021/6/21.
//

#include "TrtFaceBasicMultiAttrImpl.h"
#include "src/common/algUtil.h"
namespace arctern{
TrtFaceBasicMultiAttrImpl::TrtFaceBasicMultiAttrImpl():m_extScale{0.0,0.7,0.0,0.5}{
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
};

TrtFaceBasicMultiAttrImpl::~TrtFaceBasicMultiAttrImpl(){

};

ErrorType
TrtFaceBasicMultiAttrImpl::GetModelsConfigInfo(const MxnetModels *models) {

  auto model = models->models()->Get(0);

  m_modelsInputHeight = 112;
  m_modelsInputWidth = 128;
  bool ret = try_get_value(*model, "INPUT_HEIGHT", m_modelsInputHeight);
  assert(ret == true);

  ret= try_get_value(*model, "INPUT_WIDTH", m_modelsInputWidth);
  assert(ret == true);

  ret= try_get_value(*model, "TASK_NUM", m_taskNum);
  assert(ret == true);

  ret= try_get_value(*model, "AGE_IDX", m_ageIndex);
  assert(ret == true);

  ret= try_get_value(*model, "AGE_RANGE", m_ageRange);
  assert(ret == true);

  ret= try_get_value(*model, "EXT_SCALE0", m_extScale[0]);
  assert(ret == true);

  ret= try_get_value(*model, "EXT_SCALE1", m_extScale[1]);
  assert(ret == true);

  ret= try_get_value(*model, "EXT_SCALE2", m_extScale[2]);
  assert(ret == true);

  ret= try_get_value(*model, "EXT_SCALE3", m_extScale[3]);
  assert(ret == true);

  return  ERR_SUCCESS;
};

void TrtFaceBasicMultiAttrImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}

pair<int,float> TrtFaceBasicMultiAttrImpl::CalculateResult(const float *data, int step){
  std::vector<float> prob;
  for (size_t j = 0; j < step; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}

pair<int, float> TrtFaceBasicMultiAttrImpl::CalculateAge(const float *data, size_t num_class) {
  float age = 0, score = 0;
  for (size_t i = 0; i < num_class; ++i) {
    age += data[i] * i;
    if (score < data[i]) score = data[i];
  }
  return std::make_pair(python_round(age), score);
}

ErrorType TrtFaceBasicMultiAttrImpl::LocalPostProcess(const RunParameter *p, int startId,
                                   int endId, vector<arctern::OutputTensor<float>> &outputs) {
  auto r = dynamic_cast<FaceBasicMultiAttrResult *>(m_pResult);
  if(r == nullptr) return ERR_NULL_POINT;

  int total = endId - startId;

  int offset = 0;

  int endPos = m_initParam.model_path.find_last_of(".") ;
  std::string modelVersion = m_initParam.model_path.substr(endPos-5,5);

  if(modelVersion.compare("0.1.0") >= 0){
    offset = 4;
  }

  int stride0 = outputs[0 + offset].size() / outputs[0].shape()[0];
  int stride1 = outputs[1 + offset].size() / outputs[1].shape()[0];
  int stride2 = outputs[2 + offset].size() / outputs[2].shape()[0];
  int stride3 = outputs[3 + offset].size() / outputs[3].shape()[0];
  int stride4 = outputs[4 + offset].size() / outputs[4].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0 + offset].data() + subId * stride0;
    auto result = CalculateResult(data0, stride0);
    r->faceBasicMultiAttrs_[startId + subId].gender = arctern::BasicGender(result.first);
    r->faceBasicMultiAttrs_[startId + subId].genderScore = result.second;
    //cout <<outputs[0][0]<<" "<<outputs[0][1]<<endl;

    const float *data1 = outputs[1 + offset].data() + subId * stride1;
    auto result1 = CalculateAge(data1, stride1);
    r->faceBasicMultiAttrs_[startId + subId].age = result1.first;
    r->faceBasicMultiAttrs_[startId + subId].ageScore = result1.second;

    const float *data2 = outputs[2 + offset].data() + subId * stride2;
    auto result2 = CalculateResult(data2, stride2);
    r->faceBasicMultiAttrs_[startId + subId].glasses = arctern::BasicGlasses(result2.first);
    r->faceBasicMultiAttrs_[startId + subId].glassesScore = result2.second;

    const float *data3 = outputs[3 + offset].data() + subId * stride3;
    auto result3 = CalculateResult(data3, stride3);
    r->faceBasicMultiAttrs_[startId + subId].mask = arctern::BasicMask(result3.first);
    r->faceBasicMultiAttrs_[startId + subId].maskScore = result3.second;

    const float *data4 = outputs[4 + offset].data() + subId * stride4;
    auto result4 = CalculateResult(data4, stride4);
    r->faceBasicMultiAttrs_[startId + subId].headwear = arctern::BasicHeadwear(result4.first);
    r->faceBasicMultiAttrs_[startId + subId].headwearScore = result4.second;

  }

  return ErrorType::ERR_SUCCESS;
};
ArcternRect TrtFaceBasicMultiAttrImpl::GetFace(const int& width,const int& height, const ArcternRect &bbox){
  float x1 = bbox.x;
  float y1 = bbox.y;
  float x2 = x1 + bbox.width;
  float y2 = y1 + bbox.height;
  float w = x2 - x1 + 1;
  float h = y2 - y1 + 1;

  y1 = y1 - h * 0.7;
  y2 = y2 + h * 0.1;
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

  ArcternRect ret;
  ret.x = bx1;
  ret.y = by1;
  ret.width = bx2 - bx1;
  ret.height = by2 - by1;
  return ret;
}
ArcternRect TrtFaceBasicMultiAttrImpl::LocalGetROI(const RunParameter *p, int idx){
  const auto pRunParam = dynamic_cast<const RunParameter *>(p);
  auto &faceRect = pRunParam->rectsV_[idx];
  //auto rect = check_shrink_box(faceRect, p->imageV_[idx].width, p->imageV_[idx].height);
  auto rect = TrtFaceBasicMultiAttrImpl::GetFace(p->imageV_[idx].width, p->imageV_[idx].height, faceRect);
  return rect;
}


}
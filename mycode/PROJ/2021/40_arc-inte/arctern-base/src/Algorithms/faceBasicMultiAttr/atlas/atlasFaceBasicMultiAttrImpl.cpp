//
// Created by gh on 2020/12/17.
//

#include "atlasFaceBasicMultiAttrImpl.h"
#include "src/common/atlas/atlasAclDataType.h"

#include "Engin/src/atlas/atlasengin.h"
#include "common/algUtil.h"
#include "src/common/arcterndefs_config_analyzer.h"
namespace arctern{

AtlasFaceBasicMultiAttrImpl::AtlasFaceBasicMultiAttrImpl(): m_extScale{0.0, 0.7, 0.0, 0.5}{
   m_modelsInputHeight = 112;
   m_modelsInputWidth = 128;
}

AtlasFaceBasicMultiAttrImpl::~AtlasFaceBasicMultiAttrImpl() {
  
}

ErrorType AtlasFaceBasicMultiAttrImpl::GetModelsConfigInfo(const TrtModels* models){
  auto model = models->models()->Get(0);
  bool return_res = try_get_value(*model, "INPUT_HEIGHT", m_modelsInputHeight);
  return_res &= try_get_value(*model, "INPUT_WIDTH", m_modelsInputWidth);
  return_res &= try_get_value(*model, "EXT_SCALE0", m_extScale[0]);
  return_res &= try_get_value(*model, "EXT_SCALE1", m_extScale[1]);
  return_res &= try_get_value(*model, "EXT_SCALE2", m_extScale[2]);
  return_res &= try_get_value(*model, "EXT_SCALE3", m_extScale[3]);
  return_res &= try_get_value(*model, "TASK_NUM", m_taskNum);
  return_res &= try_get_value(*model, "AGE_IDX", m_ageIndex);
  return_res &= try_get_value(*model, "AGE_RANGE", m_ageRange);
  return ErrorType::ERR_SUCCESS;
}




ErrorType AtlasFaceBasicMultiAttrImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
std::vector<std::vector<float>> &outputs) {
   auto r = dynamic_cast<FaceBasicMultiAttrResult *>(m_pResult);

  int total = endId - startId;

  int stride0 = outputs[0].size() / total;
  int stride1 = outputs[1].size() / total;
  int stride2 = outputs[2].size() / total;
  int stride3 = outputs[3].size() / total;
  int stride4 = outputs[4].size() / total;

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    auto result = CalculateResult(data0, stride0);
    r->faceBasicMultiAttrs_[startId + subId].gender = arctern::BasicGender(result.first);
    r->faceBasicMultiAttrs_[startId + subId].genderScore = result.second;

    const float *data1 = outputs[1].data() + subId * stride1;
    auto result1 = CalculateAge(data1, stride1);
    r->faceBasicMultiAttrs_[startId + subId].age = result1.first;
    r->faceBasicMultiAttrs_[startId + subId].ageScore = result1.second;

    const float *data2 = outputs[2].data() + subId * stride2;
    auto result2 = CalculateResult(data2, stride2);
    r->faceBasicMultiAttrs_[startId + subId].glasses = arctern::BasicGlasses(result2.first);
    r->faceBasicMultiAttrs_[startId + subId].glassesScore = result2.second;

    const float *data3 = outputs[3].data() + subId * stride3;
    auto result3 = CalculateResult(data3, stride3);
    r->faceBasicMultiAttrs_[startId + subId].mask = arctern::BasicMask(result3.first);
    r->faceBasicMultiAttrs_[startId + subId].maskScore = result3.second;

    const float *data4 = outputs[4].data() + subId * stride4;
    auto result4 = CalculateResult(data4, stride4);
    r->faceBasicMultiAttrs_[startId + subId].headwear = arctern::BasicHeadwear(result4.first);
    r->faceBasicMultiAttrs_[startId + subId].headwearScore = result4.second;

  }

  return ErrorType::ERR_SUCCESS;
}

pair<int, float> AtlasFaceBasicMultiAttrImpl::CalculateResult(const float *data, int step) {
  std::vector<float> prob;
  for (size_t j = 0; j < step; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}

std::pair<int, float> AtlasFaceBasicMultiAttrImpl::CalculateAge(const float *data, size_t num_class) {
  float age = 0, score = 0;
  for (size_t i = 0; i < num_class; ++i) {
    age += data[i] * i;
    if (score < data[i]) score = data[i];
  }
  return std::make_pair(python_round(age), score);
}

cv::Rect AtlasFaceBasicMultiAttrImpl::GetFace(int width, int height,const cv::Rect &bbox) {

  float x1 = bbox.tl().x;
  float y1 = bbox.tl().y;
  float x2 = bbox.br().x;
  float y2 = bbox.br().y;
  float w = x2 - x1 + 1;
  float h = y2 - y1 + 1;

  y1 = y1 - h * m_extScale[1];
  y2 = y2 + h * m_extScale[3];

  float by1 = std::min(std::max(0, static_cast<int>(y1)), height);
  float by2 = std::min(std::max(0, static_cast<int>(y2)), height);

  float hh = by2 - by1 + 1;
  float ww = hh;

  if (ww > w) {
    int wx = (int) (ww - w) / 2;
    x1 = x1 - wx;
    x2 = x2 + wx;
  }

  float bx1 = std::min(std::max(0, static_cast<int>(x1)), width);
  float bx2 = std::min(std::max(0, static_cast<int>(x2)), width);
  return cv::Rect(cv::Point(bx1, by1), cv::Point(bx2, by2));
}

cv::Rect AtlasFaceBasicMultiAttrImpl::LocalGetROI(const RunParameter *p, int idx){
  auto &arctRect = p->rectsV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);
 
  auto rect = AtlasFaceBasicMultiAttrImpl::GetFace(p->imageV_[idx].width, p->imageV_[idx].height, oriRect);

  return rect;
}

ErrorType AtlasFaceBasicMultiAttrImpl::LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
std::vector<DvppDataInfo>& vOutputInfos,
std::vector<cv::Rect>& vRects) {
  int size = vInputInfo.size();
  std::vector<DvppCropInputInfo> vCropInputInfo(size);
  for(int i = 0; i < size; ++i){
    vCropInputInfo[i].dataInfo = vInputInfo[i];
    vCropInputInfo[i].roi.left = vRects[i].x;
    vCropInputInfo[i].roi.up = vRects[i].y;
    vCropInputInfo[i].roi.right = vRects[i].x + vRects[i].width - 1;
    vCropInputInfo[i].roi.down = vRects[i].y + vRects[i].height - 1;
  }

  auto ret = m_pPreprocessor->CombineCropResizeBatchProcess(vCropInputInfo,vOutputInfos,true);

  if (ret != APP_ERR_OK) {
    LOGASDError << "failed to do altas preprocess";
    m_lastErrorCode = ErrorType::ERR_PRE_PROCESS;
  }
  return m_lastErrorCode;
}


}

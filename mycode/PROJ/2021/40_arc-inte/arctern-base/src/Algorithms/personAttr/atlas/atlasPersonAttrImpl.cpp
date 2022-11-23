//
// Created by gh on 2020/12/17.
//

#include "atlasPersonAttrImpl.h"
#include "src/common/atlas/atlasAclDataType.h"

#include "Engin/src/atlas/atlasengin.h"
#include "common/algUtil.h"
#include "src/common/arcterndefs_config_analyzer.h"
namespace arctern{

AtlasPersonAttrImpl::AtlasPersonAttrImpl():  m_extScale{0.15, 0, 0.15, 0}{
  m_modelsInputHeight = 256;
  m_modelsInputWidth = 128;
}

AtlasPersonAttrImpl::~AtlasPersonAttrImpl() {
  
}

ErrorType AtlasPersonAttrImpl::GetModelsConfigInfo(const TrtModels* models){
  return ErrorType::ERR_SUCCESS;
}




ErrorType AtlasPersonAttrImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
std::vector<std::vector<float>> &outputs) {
   auto r = dynamic_cast<PersonAttrResult *>(m_pResult);

  int total = endId - startId;

  for (size_t i = 0; i < outputs.size(); ++i) {
    int stride = outputs[i].size() / total;
    for (int subId = 0; subId < total; subId++) {
      const float *data0 = outputs[i].data() + subId * stride;
      std::pair<int, float> attr_res = CalculateResult(data0, stride);
      auto &attr = r->personAttrs_[startId + subId];
      switch (i) {
        case 0 : {
          attr.age = PersonAgeType(attr_res.first);
          attr.ageScore = attr_res.second;
          break;
        }
        case 1 : {
          attr.hasBackpack = attr_res.first;
          attr.hasBackpackScore = attr_res.second;
          break;
        }
        case 2 : {
          attr.hasCart = attr_res.first;
          attr.hasCartScore = attr_res.second;
          break;
        }
        case 3 : {
          attr.isFrontHold = attr_res.first;
          attr.isFrontHoldScore = attr_res.second;
          break;
        }
        case 4 : {
          attr.gender = PersonGenderType(attr_res.first);
          attr.genderScore = attr_res.second;
          break;
        }
        case 5 : {
          attr.glasses = PersonGlassesType(attr_res.first);
          attr.glassesScore = attr_res.second;
          break;
        }
        case 6 : {
          attr.hairColor = PersonHairColorType(attr_res.first);
          attr.hairColorScore = attr_res.second;
          break;
        }
        case 7 : {
          attr.hairLength = PersonHairLengthType(attr_res.first);
          attr.hairLengthScore = attr_res.second;
          break;
        }
        case 8 : {
          attr.hasHandbag = attr_res.first;
          attr.hasHandbagScore = attr_res.second;
          break;
        }
        case 9 : {
          attr.headwear = PersonHeadwearType(attr_res.first);
          attr.headwearScore = attr_res.second;
          break;
        }
        case 10 : {
          attr.lowerColor = PersonClothesColorType(attr_res.first);
          attr.lowerColorScore = attr_res.second;
          break;
        }
        case 11 : {
          attr.lowerTexture = PersonClothesTextureType(attr_res.first);
          attr.lowerTextureScore = attr_res.second;
          break;
        }
        case 12 : {
          attr.lowerType = PersonLowerClothesType(attr_res.first);
          attr.lowerTypeScore = attr_res.second;
          break;
        }
        case 13 : {
          attr.hasMask = Mask(attr_res.first);
          attr.hasMaskScore = attr_res.second;
          break;
        }
        case 14 : {
          attr.orientation = PersonOrientationType(attr_res.first);
          attr.orientationScore = attr_res.second;
          break;
        }
        case 15 : {
          attr.hasOvercoat = Overcoat( attr_res.first);
          attr.hasOvercoatScore = attr_res.second;
          break;
        }
        case 16 : {
          attr.personType = PersonType(attr_res.first);
          attr.personTypeScore = attr_res.second;
          break;
        }
        case 17 : {
          attr.hasSingleBag = SingleBag( attr_res.first);
          attr.hasSingleBagScore = attr_res.second;
          break;
        }
        case 18 : {
          attr.hasTrolleyCase = attr_res.first;
          attr.hasTrolleyCaseScore = attr_res.second;
          break;
        }
        case 19 : {
          attr.upperColor = PersonClothesColorType(attr_res.first);
          attr.upperColorScore = attr_res.second;
          break;
        }
        case 20 : {
          attr.upperTexture = PersonClothesTextureType(attr_res.first);
          attr.upperTextureScore = attr_res.second;
          break;
        }
        case 21 : {
          attr.upperType = PersonUpperClothesType(attr_res.first);
          attr.upperTypeScore = attr_res.second;
          break;
        }
        case 22 : {
          attr.usePhone = attr_res.first;
          attr.usePhoneScore = attr_res.second;
          break;
        }
        case 23:{
          attr.quality = Quality(attr_res.first);
          attr.qualityScore = attr_res.second;
          break;
        }
        default: {
          break;
        }
      } // switch
    } // images id
  } // attributes id
  return ErrorType::ERR_SUCCESS;
}

pair<int, float> AtlasPersonAttrImpl::CalculateResult(const float *data, int step) {
  std::vector<float> prob;
  for (size_t j = 0; j < step; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}


std::vector<float> AtlasPersonAttrImpl::get_padded_bbox(const cv::Rect &bbox,
const std::vector<float> &pad_scale) {
  float x1 = bbox.tl().x;
  float y1 = bbox.tl().y;
  float x2 = bbox.br().x;
  float y2 = bbox.br().y;
  float h = y2 - y1 + 1;
  float dnorm = (1 - pad_scale[0] - pad_scale[1]);
  float top = h * pad_scale[0] / dnorm;
  float btm = h * pad_scale[1] / dnorm;
  std::vector<float> rect= {x1, y1 - top, x2, y2 + btm};
  return rect;
}

std::vector<cv::Rect> AtlasPersonAttrImpl::get_padded_rect(const int &imgWidth, const int &imgHeight,
const cv::Rect &bbox, const std::vector<float> &pad_scale){
  std::vector<float> padded_bbox = get_padded_bbox(bbox, pad_scale);

  float x1 = padded_bbox[0];
  float y1 = padded_bbox[1];
  float x2 = padded_bbox[2];
  float y2 = padded_bbox[3];
  float w = x2 - x1 + 1;
  float h = y2 - y1 + 1;

  x1 = (int)(x1 - w * m_extScale[0]);
  x2 = (int)(x2 + w * m_extScale[2]);
  y1 = (int)(y1 - h * (m_extScale[1]));
  y2 = (int)(y2 + h * (m_extScale[3]));

  int hh = y2 - y1 + 1;
  int ww = x2 - x1 + 1;

  // calculate coordinate
  auto sx1 = (int) (x1 < 0 ? 0 : x1);
  auto sy1 = (int) (y1 < 0 ? 0 : y1);
  auto sx2 = (int) (x2 < imgWidth - 1 ? x2 : imgWidth - 1);
  auto sy2 = (int) (y2 < imgHeight - 1 ? y2 : imgHeight - 1);
  auto ew  = (int) (sx2 - sx1 + 1);
  auto eh  = (int) (sy2 - sy1 + 1);

  auto dx1 = (int) (x1 < 0 ? -x1 : 0);
  auto dy1 = (int) (y1 < 0 ? -y1 : 0);

  std::vector<cv::Rect> retRect;
  retRect.emplace_back(cv::Rect(sx1, sy1, ew, eh));
  retRect.emplace_back(cv::Rect(dx1, dy1, ew, eh));

  return retRect;
}

  

cv::Rect AtlasPersonAttrImpl::LocalGetROI(const RunParameter *p, int idx){
  const auto implRunP = dynamic_cast<const PersonAttrRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];
  auto &padScale = implRunP->personTruncationV_[idx];
  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);

  //auto rect = check_shrink_box(oriRect, image.cols, image.rows);
  auto ret = get_padded_rect(p->imageV_[idx].width,p->imageV_[idx].height, oriRect, padScale);
  assert(ret.size() > 0);
  return ret[0];
}

ErrorType AtlasPersonAttrImpl::LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
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

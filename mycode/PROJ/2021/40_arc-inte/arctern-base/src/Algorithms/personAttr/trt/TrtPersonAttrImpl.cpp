//
// Created by Admin on 2021/6/21.
//

#include "TrtPersonAttrImpl.h"
#include "src/common/algUtil.h"
namespace arctern{
TrtPersonAttrImpl::TrtPersonAttrImpl() : m_extScale{0.15, 0, 0.15, 0}{
};

TrtPersonAttrImpl::~TrtPersonAttrImpl(){

};

ErrorType
TrtPersonAttrImpl::GetModelsConfigInfo(const MxnetModels *models) {
  ErrorType vResult;
  m_modelsInputHeight = 256;
  m_modelsInputWidth = 128;
  return  vResult;
};

void TrtPersonAttrImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
 runParm->sequence_.push_back(OperatorType::crop);
 runParm->m_paddingParam.assign(m_vPaddingParam.begin(),m_vPaddingParam.end());
 runParm->sequence_.push_back(OperatorType::padding);
 runParm->sequence_.push_back(OperatorType::normalized);
 runParm->sequence_.push_back(OperatorType::resized);
 runParm->sequence_.push_back(OperatorType::extractChannel);
}




pair<int,float> TrtPersonAttrImpl::CalculateResult(const float *data, int step){
  std::vector<float> prob;
  for (size_t j = 0; j < step; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}


ErrorType TrtPersonAttrImpl::LocalPostProcess(const RunParameter *p, int startId,
                                   int endId, vector<arctern::OutputTensor<float>> &outputs) {
  auto r = dynamic_cast<PersonAttrResult *>(m_pResult);

  int total = endId - startId;

  for (size_t i = 0; i < outputs.size(); ++i) {
    int stride = outputs[i].size() / outputs[i].shape()[0];
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
          attr.hasMask = Mask( attr_res.first);
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
};

std::vector<float> TrtPersonAttrImpl::get_padded_bbox(const ArcternRect &bbox,
                                                      const std::vector<float> &pad_scale) {
  float x1 = bbox.x;
  float y1 = bbox.y;
  float x2 = bbox.x + bbox.width;
  float y2 = bbox.y + bbox.height;
  float h = y2 - y1 + 1;
  float dnorm = (1 - pad_scale[0] - pad_scale[1]);
  float top = h * pad_scale[0] / dnorm;
  float btm = h * pad_scale[1] / dnorm;
  std::vector<float> rect= {x1, y1 - top, x2, y2 + btm};
  return rect;
}

ArcternRect
TrtPersonAttrImpl::GetPerson(const int img_height,
                             const int img_width,
                             const ArcternRect &bbox,
                             const std::array<float, 4> &ext_scale,
                             const std::vector<float> &pad_scale) {
  std::vector<float> padded_bbox = get_padded_bbox(bbox, pad_scale);
  int height = img_height;
  int width = img_width;

  float x1 = padded_bbox[0];
  float y1 = padded_bbox[1];
  float x2 = padded_bbox[2];
  float y2 = padded_bbox[3];
  float w = x2 - x1 + 1;
  float h = y2 - y1 + 1;

  x1 = (int)(x1 - w * ext_scale[0]);
  x2 = (int)(x2 + w * ext_scale[2]);
  y1 = (int)(y1 - h * (ext_scale[1]));
  y2 = (int)(y2 + h * (ext_scale[3]));

  int hh = y2 - y1 + 1;
  int ww = x2 - x1 + 1;

  // calculate coordinate
  auto sx1 = (int) (x1 < 0 ? 0 : x1);
  auto sy1 = (int) (y1 < 0 ? 0 : y1);
  auto sx2 = (int) (x2 < width - 1 ? x2 : width - 1);
  auto sy2 = (int) (y2 < height - 1 ? y2 : height - 1);
  auto ew  = (int) (sx2 - sx1 + 1);
  auto eh  = (int) (sy2 - sy1 + 1);

  auto dx1 = (int) (x1 < 0 ? -x1 : 0);
  auto dy1 = (int) (y1 < 0 ? -y1 : 0);

  m_vPaddingParam.clear();
  m_vPaddingParam.reserve(4);
  m_vPaddingParam.push_back(hh);
  m_vPaddingParam.push_back(ww);
  m_vPaddingParam.push_back(dy1);
  m_vPaddingParam.push_back(dx1);

  ArcternRect aRet;
  aRet.x = sx1;
  aRet.y = sy1;
  aRet.width = ew;
  aRet.height = eh;

  return aRet;
}

ArcternRect TrtPersonAttrImpl::LocalGetROI(const RunParameter *p, int idx){
  const auto pRunParam = dynamic_cast<const PersonAttrRunParameter *>(p);
  auto &faceRect = pRunParam->rectsV_[idx];
  auto &padScale = pRunParam->personTruncationV_[idx];
  auto rect = TrtPersonAttrImpl::GetPerson(p->imageV_[idx].height,
                                           p->imageV_[idx].width,
                                           faceRect,
                                           m_extScale,
                                           padScale);
  return rect;
}


}
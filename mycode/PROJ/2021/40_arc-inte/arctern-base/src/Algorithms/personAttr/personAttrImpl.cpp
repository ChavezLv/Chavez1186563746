/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.07
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#include <utility>
#include <vector>
#include "personAttrImpl.h"
#include "common/imageConvert.h"
#include "common/algUtil.h"

using arctern::PersonAttrImpl;
using arctern::ErrorType;
using arctern::Result;
using arctern::ArcternRect;

PersonAttrImpl::PersonAttrImpl() : extScale_{0.15, 0, 0.15, 0} {
  input_h_ = 256;
  input_w_ = 128;
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_BGR888;
}

PersonAttrImpl::~PersonAttrImpl() noexcept {
  PersonAttrImpl::Release();
}

//void PersonAttrImpl::Release() {
//
//  ArcternAlgorithm::Release();
//}

//ErrorType PersonAttrImpl::Init(const InitParameter *p) {
//  auto ret = ArcternAlgorithm::Init(p);
//  if (ret != ErrorType::ERR_SUCCESS) {
//    PersonAttrImpl::Release();
//    return ret;
//  }
//
//  preProcessor_.reset(new AttrPreProcesser);
//
//  arctern::AttrPreProInitParameter initParam;
//  initParam.netH_ = input_h_;
//  initParam.netW_ = input_w_;
//  initParam.mean_ = 127.5;
//  initParam.std_ = 128.0;
//  initParam.alignFace_ = AlignFace::NoAlignFace;
//
//  assert(preProcessor_);
//
//  auto code = preProcessor_->Init(&initParam);
//  assert(code == ErrorType::ERR_SUCCESS);
//
//  return ErrorType::ERR_SUCCESS;
//}

ErrorType PersonAttrImpl::CheckRunParam(const RunParameter *p) {
  return ArcternAlgorithm::CheckRunParam(p);
}


//cv::Mat PersonAttrImpl::LocalGetFace(const cv::Mat &image, const RunParameter *p, int idx) {
//  const auto implRunP = dynamic_cast<const PersonAttrRunParameter *>(p);
//  auto &arctRect = implRunP->rectsV_[idx];
//  auto &padScale = implRunP->personTruncationV_[idx];
//  cv::Rect oriRect(arctRect.x, arctRect.y, arctRect.width, arctRect.height);
//
//  //auto rect = check_shrink_box(oriRect, image.cols, image.rows);
//  return get_person(image, oriRect, extScale_, padScale);
//}

ErrorType PersonAttrImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                           std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<PersonAttrResult *>(result_);

  int total = endId - startId;

  // const auto batchNum = initParam_.batchNum_;

  for (size_t i = 0; i < outputs.size(); ++i) {
    int stride = outputs[i].size() / outputs[i].shape()[0];
    for (int subId = 0; subId < total; subId++) {
      const float *data0 = outputs[i].data() + subId * stride;
      std::pair<int, float> attr_res = calcAttr(data0, stride);
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
          attr.hasOvercoat =Overcoat( attr_res.first);
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

/// \brief calculate mask label and corresponding score
std::pair<int, float> PersonAttrImpl::calcAttr(const float *data, size_t num_class) {
  std::vector<float> prob;
  for (size_t j = 0; j < num_class; ++j) {
    prob.push_back(data[j]);
  }
  float max_score;
  int max_idx = arctern::softmax(prob, max_score);
  return std::make_pair(max_idx, max_score);
}

std::vector<float> PersonAttrImpl::get_padded_bbox(const cv::Rect &bbox,
                                                   const std::vector<float> &pad_scale) {
  float x1 = bbox.tl().x;
  float y1 = bbox.tl().y;
  float x2 = bbox.br().x;
  float y2 = bbox.br().y;
  float h = y2 - y1 + 1;
  float dnorm = (1 - pad_scale[0] - pad_scale[1]);
  float top = h * pad_scale[0] / dnorm;
  float btm = h * pad_scale[1] / dnorm;
  std::vector<float> rect = {x1, y1 - top, x2, y2 + btm};
  return rect;
}

//ArcternRect PersonAttrImpl::get_person(const cv::Mat &image, const cv::Rect &bbox,
//      const std::array<float, 4> ext_scale, const std::vector<float> &pad_scale) {
//  std::vector<float> padded_bbox = get_padded_bbox(bbox, pad_scale);
//  int height = image.rows;
//  int width = image.cols;
//
//  float x1 = padded_bbox[0];
//  float y1 = padded_bbox[1];
//  float x2 = padded_bbox[2];
//  float y2 = padded_bbox[3];
//  float w = x2 - x1 + 1;
//  float h = y2 - y1 + 1;
//
//  x1 = (int)(x1 - w * ext_scale[0]);
//  x2 = (int)(x2 + w * ext_scale[2]);
//  y1 = (int)(y1 - h * (ext_scale[1]));
//  y2 = (int)(y2 + h * (ext_scale[3]));
//
//  int hh = y2 - y1 + 1;
//  int ww = x2 - x1 + 1;
//
//  // calculate coordinate
//  auto sx1 = (int) (x1 < 0 ? 0 : x1);
//  auto sy1 = (int) (y1 < 0 ? 0 : y1);
//  auto sx2 = (int) (x2 < width - 1 ? x2 : width - 1);
//  auto sy2 = (int) (y2 < height - 1 ? y2 : height - 1);
//  auto ew  = (int) (sx2 - sx1 + 1);
//  auto eh  = (int) (sy2 - sy1 + 1);
//
//  auto dx1 = (int) (x1 < 0 ? -x1 : 0);
//  auto dy1 = (int) (y1 < 0 ? -y1 : 0);
//
//  cv::Mat padded(hh, ww, CV_8UC3, cv::Scalar(0));
//  cv::Rect dst_rect = cv::Rect(dx1, dy1, ew, eh);
//  cv::Rect org_rect = cv::Rect(sx1, sy1, ew, eh);
//
//  image(org_rect).copyTo(padded(dst_rect));
//  return padded;
//}


ArcternRect PersonAttrImpl::LocalGetRoi(const RunParameter *p, int idx) {
// ext, no padding
  int height = p->imageV_[idx].height;
  int width = p->imageV_[idx].width;

  float x1 = p->rectsV_[idx].x;
  float y1 = p->rectsV_[idx].y;
  float x2 = p->rectsV_[idx].x + p->rectsV_[idx].width;
  float y2 = p->rectsV_[idx].y + p->rectsV_[idx].height;
  auto sx1 = (int) (x1 < 0 ? 0 : x1);
  auto sy1 = (int) (y1 < 0 ? 0 : y1);
  auto sx2 = (int) (x2 <= width ? x2 : width);
  auto sy2 = (int) (y2 <= height ? y2 : height);

  float w = x2 - x1;

  x1 = (int) (sx1 - std::round(w * 0.15));
  sx1 = (int) (x1 < 0 ? 0 : x1);
  x2 = (int) (sx2 + std::round(w * 0.15));
  sx2 = (int) (x2 <= width ? x2 : width);
  auto ew = (int) (sx2 - sx1);
  auto eh = (int) (sy2 - sy1);
  cv::Rect person_rect;
  if (ew > 0 && eh > 0) {
    person_rect = cv::Rect(sx1, sy1, ew, eh);
  } else {
    person_rect = cv::Rect(0, 0, width, height);
  }
  ArcternRect aRet;
  aRet.x = person_rect.x;
  aRet.y = person_rect.y;
  aRet.width = person_rect.width;
  aRet.height = person_rect.height;
  return aRet;

}
void PersonAttrImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::normalized);
}
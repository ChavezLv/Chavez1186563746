/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
*  author:         chenbin
 *  Last modified:  2020.12.02
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#include <vector>
#include "personQualityImpl.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "common/algUtil.h"



using arctern::PersonQualityImpl;
using arctern::ErrorType;
using arctern::Result;
using arctern::ArcternRect;

PersonQualityImpl::PersonQualityImpl() {
  input_h_ = 100;
  input_w_ = 50;
}

PersonQualityImpl::~PersonQualityImpl() noexcept {
  PersonQualityImpl::Release();
}

ErrorType PersonQualityImpl::CheckRunParam(const RunParameter *p) {
  ErrorType arcternBaseCheck = ArcternAlgorithm::CheckRunParam(p);
  if (arcternBaseCheck != ErrorType::ERR_SUCCESS) return arcternBaseCheck;
  return ErrorType::ERR_SUCCESS;
}

ErrorType PersonQualityImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                            std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<PersonQualityResult *>(result_);
  int total = endId - startId;
  const float* output_res_br = outputs[RES_BRANCH_INDEX].data();
  int start_index_res = 0;
  int start_index_person = 0;
  int start_index_faceback = 0;

  for (int subId = 0; subId < total; subId++) {
      //int faceback_cls = 0;
    float scale_output_res = output_res_br[start_index_res++];
    if (PERSON_CLS_BRANCH_INDEX >= 0 && FACEBACK_BRANCH_INDEX >= 0) {
      const float* output_person_cls_br = outputs[PERSON_CLS_BRANCH_INDEX].data();
      const float* output_faceback_cls_br = outputs[FACEBACK_BRANCH_INDEX].data();
      float cls_not_person = output_person_cls_br[start_index_person++];
      float cls_person = output_person_cls_br[start_index_person++];
      float cls_back = output_faceback_cls_br[start_index_faceback++];
      float cls_face = output_faceback_cls_br[start_index_faceback++];

      if (cls_face > cls_back) {
        scale_output_res += 0.1;
        //faceback_cls = 1;
      }
      if (cls_not_person > cls_person) {
        scale_output_res = 0;
      }
    }
    if (scale_output_res > 1) { scale_output_res = 1.0; }
    if (scale_output_res < 0) { scale_output_res = 0.0; }
    r->personQuality_[startId + subId] = scale_output_res;
  }
  return ErrorType::ERR_SUCCESS;
}

int PersonQualityImpl::GetConfigInfo(arctern::MxnetModel *model) {
  bool res = try_get_value(*model, "GRAY_IMG_INPUT", isGrayInput_);
  res &= try_get_value(*model, "BBOX_EXTSACL_TOP", BBOX_EXTSACL_TOP);
  res &= try_get_value(*model, "INPUT_W", input_w_);
  res &= try_get_value(*model, "INPUT_H", input_h_);
  res &= try_get_value(*model, "FACEBACK_BRANCH_INDEX", FACEBACK_BRANCH_INDEX);
  res &= try_get_value(*model, "PERSON_CLS_BRANCH_INDEX", PERSON_CLS_BRANCH_INDEX);
  res &= try_get_value(*model, "RES_BRANCH_INDEX", RES_BRANCH_INDEX);
 // assert(res);

  dstImageFormatForForward_ = isGrayInput_ ? ARCTERN_IMAGE_FORMAT_GRAY : ARCTERN_IMAGE_FORMAT_RGB888;
  //initPreProcessor();
  return 0;
}


ArcternRect PersonQualityImpl::LocalGetRoi(const RunParameter *p, int idx) {
    const auto implRunP = dynamic_cast<const PersonQualityRunParameter *>(p);
  auto &arctRect = implRunP->rectsV_[idx];

  int ymin = static_cast<int>(arctRect.y - arctRect.height * BBOX_EXTSACL_TOP);
  int pbbox_ext_h = static_cast<int>(arctRect.height + arctRect.height * BBOX_EXTSACL_TOP);
  cv::Rect oriRect(arctRect.x, ymin, arctRect.width, pbbox_ext_h);

  auto rect = check_shrink_box(oriRect, p->imageV_[idx].width, p->imageV_[idx].height);
  ArcternRect result;
  result.x = rect.x;
  result.y = rect.y;
  result.width = rect.width;
  result.height = rect.height;
  return result;
}
void PersonQualityImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::normalized);
}

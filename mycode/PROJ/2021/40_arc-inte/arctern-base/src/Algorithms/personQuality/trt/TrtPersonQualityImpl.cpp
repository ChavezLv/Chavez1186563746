//
// Created by Admin on 2021/5/19.
//
#include "TrtPersonQualityImpl.h"
#include "src/common/common.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
namespace arctern {

TrtPersonQualityImpl::TrtPersonQualityImpl() {

}

TrtPersonQualityImpl::~TrtPersonQualityImpl() {

}

ErrorType
TrtPersonQualityImpl::GetModelsConfigInfo(const MxnetModels *models) {
  ErrorType vResult;
  auto model = models->models()->Get(0);
  m_modelsInputHeight = 100;
  m_modelsInputWidth = 50;
  bool ret = try_get_value(*model, "GRAY_IMG_INPUT", m_bGrayInput);
  assert(ret == true);

  ret = try_get_value(*model, "BBOX_EXTSACL_TOP", BBOX_EXTSACL_TOP);
  assert(ret == true);

  ret = try_get_value(*model, "INPUT_W", m_modelsInputWidth);
  assert(ret == true);

  ret = try_get_value(*model, "INPUT_H", m_modelsInputHeight);
  assert(ret == true);

  ret = try_get_value(*model, "FACEBACK_BRANCH_INDEX", FACEBACK_BRANCH_INDEX);
  assert(ret == true);

  ret = try_get_value(*model, "PERSON_CLS_BRANCH_INDEX", PERSON_CLS_BRANCH_INDEX);
  assert(ret == true);

  ret = try_get_value(*model, "RES_BRANCH_INDEX", RES_BRANCH_INDEX);
  assert(ret == true);

  m_dstImageFormatForForward = m_bGrayInput ? ARCTERN_IMAGE_FORMAT_GRAY : ARCTERN_IMAGE_FORMAT_RGB888;

  return vResult;
}

ErrorType TrtPersonQualityImpl::LocalPostProcess(const RunParameter *p,
                                                 int startId,
                                                 int endId,
                                                 vector<arctern::OutputTensor<float>> &outputs) {
  auto r = dynamic_cast<PersonQualityResult *>(m_pResult);
  int total = endId - startId;
  const float* output_res_br = outputs[RES_BRANCH_INDEX].data();
  int start_index_res = 0;
  int start_index_person = 0;
  int start_index_faceback = 0;

  for (int subId = 0; subId < total; subId++) {
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
void TrtPersonQualityImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
  //runParm->sequence_.push_back(OperatorType::extractChannel);
}
ArcternRect TrtPersonQualityImpl::LocalGetROI(const RunParameter *p, int idx) {
  const auto runParame = dynamic_cast<const PersonQualityRunParameter *>(p);
  auto &arctRect = runParame->rectsV_[idx];

  int ymin = static_cast<int>(arctRect.y - arctRect.height * BBOX_EXTSACL_TOP);
  int pbbox_ext_h = static_cast<int>(arctRect.height + arctRect.height * BBOX_EXTSACL_TOP);

  ArcternRect roiRect;
  roiRect.x = arctRect.x;
  roiRect.y = ymin;
  roiRect.width = arctRect.width;
  roiRect.height = pbbox_ext_h;

  return CheckShrinkBox(roiRect, runParame->imageV_[idx].width, runParame->imageV_[idx].height);
}
}

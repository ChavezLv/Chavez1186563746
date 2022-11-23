// ****************************************************************************
// *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
// *  Description:
// *  author:         kuanghongshen
// *  Last modified:  2021.06.01
// *  email:          kuang.hongshen@intellif.com
// ****************************************************************************

#include "personPoseImpl.h"
#include "common/algUtil.h"
#include "common/file_buffer.h"
#include "personPose.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include <string>
#include "utils/personPoseUtils.h"

using namespace arctern;

personPoseImpl::personPoseImpl()
    : m_fBBoxExtSaclTop(1.25f) {
  input_h_ = 256;
  input_w_ = 192;
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_RGB888;
}

personPoseImpl::~personPoseImpl() { personPoseImpl::Release(); }

ErrorType personPoseImpl::Process(const RunParameter *p,Result *r) {
  m_vUpBBoxes.clear();
  return ArcternAlgorithm::Process(p,r);
}

ErrorType
personPoseImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                 std::vector<arctern::Tensor<float>> &outputs) {
  auto *p_res = dynamic_cast<PersonPoseResult *>(result_);

  int i_total = endId - startId;
  int i_stride = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < i_total; subId++) {
    float *p_data = outputs[0].data() + subId * i_stride;
    auto type_res = personPose::heatmap_to_coord(p_data, m_vUpBBoxes[startId + subId]);

    PersonPosePoints kps;
    std::for_each(type_res.begin(), type_res.end(), [&kps](std::pair<cv::Point, float> &item) {
      std::pair<ArcternPoint, float> result;
      result.first.x = item.first.x;
      result.first.y = item.first.y;
      result.second = item.second;
      kps.push_back(result);
    });

    p_res->results_[startId + subId] = kps;
  }

  return ErrorType::ERR_SUCCESS;
}

int personPoseImpl::GetConfigInfo(arctern::MxnetModel *model) {
  //bool ret = try_get_value(*model, "RESIZE_TYPE", m_sResizeType);
  bool ret = try_get_value(*model, "NET_INPUT_H", input_h_);
  ret &= try_get_value(*model, "NET_INPUT_W", input_w_);

  if (!ret) {
    input_h_ = 256;
    input_w_ = 192;
   // m_sResizeType = "linear";
    assert(false);
  }

  return 0;
}


ArcternRect personPoseImpl::LocalGetRoi(const RunParameter *p, int idx) {
  const auto p_run_param = dynamic_cast<const PersonPoseRunParameter *>(p);
  auto &arct_rect = p_run_param->rectsV_[idx];
  cv::Rect ori_rect(arct_rect.x, arct_rect.y, arct_rect.width,
                    arct_rect.height);
  cv::Rect person_ext = personPose::upscaleBbox(ori_rect, m_fBBoxExtSaclTop);
  cv::Rect person_roi = check_shrink_box(person_ext, p->imageV_[idx].width, p->imageV_[idx].height);
  m_vUpBBoxes.push_back(person_roi);
  ArcternRect result;
  result.x = person_roi.x;
  result.y = person_roi.y;
  result.width = person_roi.width;
  result.height = person_roi.height;
  return result;
}

void personPoseImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
}

void personPoseImpl::InitPreprocessorParam() {
  preProcessor_.reset(new OpencvPreprocessor);
  OpencvPreprocessorInitPt init_pt;
  init_pt.net_h_ = input_h_;
  init_pt.net_w_ = input_w_;
  init_pt.mean_ = 127.5;
  init_pt.std_ = 127.5f;
  init_pt.border_value = 0;
  auto code = preProcessor_->Init(&init_pt);
  assert(code == ErrorType::ERR_SUCCESS);
}
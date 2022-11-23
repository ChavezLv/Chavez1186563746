/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         lvchaolin
 *  create:         2021.12.20
 *  email:          lv.chaolin@intellif.com
 ****************************************************************************/

#include "manHoleCoverDetImpl.h"

using namespace arctern;

ManHoleCoverDetImpl::ManHoleCoverDetImpl(){
}

ManHoleCoverDetImpl::~ManHoleCoverDetImpl() {}

int ManHoleCoverDetImpl::GetConfigInfo(arctern::MxnetModel *model) {
  // todo check
  m_yolo_v_4 = std::make_unique<YoloV4>();
  auto ret = m_yolo_v_4->getConfigInfo(model);
  input_h_ = m_yolo_v_4->getNetH();
  input_w_ = m_yolo_v_4->getNetW();

  return ret;
}

ErrorType
ManHoleCoverDetImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                 std::vector<arctern::Tensor<float>> &outputs) {
  auto r = dynamic_cast<ManHoleCoverDetResult*>(result_);
  int total = endId - startId;
  float *prediction = outputs[0].data();
  int stride = outputs[0].shape().size() / outputs[0].shape()[0];

  int offset = 0;
  for (int subId = 0; subId < total; ++ subId) {
    vector<Bbox> boxes;
    boxes = m_yolo_v_4->postProcess(prediction + offset, p->imageV_[startId + subId].width,\
                    p->imageV_[startId + subId].height, initParam_.thresh_);

    for (auto &box : boxes) {
      MANHOLECOVER_DETECT_TINY_INFO info;
      info.rect.x = box.x1;
      info.rect.y = box.y1;
      info.rect.width = box.x2 - box.x1;
      info.rect.height = box.y2 - box.y1;
      info.score = box.score;

      info.type = arctern::ManholeCoverAbnormalType(box.label);

      r->detInfos_[startId + subId].push_back(info);
    }
    offset += stride;
  }

  return ErrorType::ERR_SUCCESS;
}

void ManHoleCoverDetImpl::InitPreprocessorParam() {
  preProcessor_.reset(new OpencvPreprocessor);
  OpencvPreprocessorInitPt initPt;
  initPt.net_h_ = input_h_;
  initPt.net_w_ = input_w_;
  initPt.mean_ = 0.0;
  initPt.std_ = 255.0;
  initPt.alignMent_ = AlignMent::AlignCenter;
  initPt.detType_ = DetType::Face;
  initPt.border_value = 128;

  auto retErrType = preProcessor_->Init(&initPt); 
  assert(ErrorType::ERR_SUCCESS == retErrType);
}

void ManHoleCoverDetImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
}

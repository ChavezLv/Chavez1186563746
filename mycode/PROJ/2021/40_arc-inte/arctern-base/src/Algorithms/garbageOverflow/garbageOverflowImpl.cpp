/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         lvchaolin
 *  create:         2021.12.10
 *  email:          lv.chaolin@intellif.com
 ****************************************************************************/

#include "garbageOverflowImpl.h"

using namespace arctern;

GarbageOverflowImpl::GarbageOverflowImpl() : m_bNormalized(true) {
}

GarbageOverflowImpl::~GarbageOverflowImpl() {}

int GarbageOverflowImpl::GetConfigInfo(arctern::MxnetModel *model) {
  // todo check
  m_yolo_v_3 = std::make_unique<YoloV3>();
  auto ret = m_yolo_v_3->getConfigInfo(model);
  input_h_ = m_yolo_v_3->getNetH();
  input_w_ = m_yolo_v_3->getNetW();

  m_square_face = false;
  //initPreProcessor();
  return ret;
}

ErrorType
GarbageOverflowImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                 std::vector<arctern::Tensor<float>> &outputs) {
  auto r = dynamic_cast<GarbageOverflowResult *>(result_);
  int total = endId - startId;
  float *prediction = outputs[0].data();
  int stride = outputs[0].shape().size() / outputs[0].shape()[0];

  int offset = 0;
  for (int subId = 0; subId < total; ++ subId) {
    vector<Bbox> boxes;
    m_yolo_v_3->postProcess3(prediction + offset, p->imageV_[startId + subId].width,
                           p->imageV_[startId + subId].height, initParam_.thresh_, boxes);

    if (m_square_face) {
      ConvertSquare(boxes, m_exth_scale_face, m_extw_scale_face);
    }

    for (auto &box : boxes) {
      GARBAGE_OVERFLOW_DETECT_TINY_INFO info;
      info.rect.x = box.x1;
      info.rect.y = box.y1;
      info.rect.width = box.x2 - box.x1;
      info.rect.height = box.y2 - box.y1;
      info.score = box.score;

      info.type = arctern::GarbageOverflowType(box.label);

      r->detInfos_[startId + subId].push_back(info);
    }
    offset += stride;
  }

  return ErrorType::ERR_SUCCESS;
}

void GarbageOverflowImpl::InitPreprocessorParam() {
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

void GarbageOverflowImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::resized);
  /* runParm->sequence_.push_back(OperatorType::detFace); */
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
}

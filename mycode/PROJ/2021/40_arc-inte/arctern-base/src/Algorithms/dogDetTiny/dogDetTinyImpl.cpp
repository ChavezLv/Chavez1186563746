/****************************************************************************
 *  Copyright:      Copyright © 2021 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         kuanghongshen
 *  modified:       2021.06.04
 *  email:          kuang.hongshen@intellif.com
 ****************************************************************************/

#include "dogDetTinyImpl.h"

using namespace arctern;

dogDetTinyImpl::dogDetTinyImpl() : m_bNormalized(true) {
}

dogDetTinyImpl::~dogDetTinyImpl() {}

int dogDetTinyImpl::GetConfigInfo(arctern::MxnetModel *model) {
  // todo check
  yolo_v_3_ = std::make_unique<YoloV3>();
  auto ret = yolo_v_3_->getConfigInfo(model);
  input_h_ = yolo_v_3_->getNetH();
  input_w_ = yolo_v_3_->getNetW();

  square_face_ = false;
  //initPreProcessor();
  return ret;
}

ErrorType
dogDetTinyImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                 std::vector<arctern::Tensor<float>> &outputs) {
  auto r = dynamic_cast<dogDetTinyResult *>(result_);
  int total = endId - startId;
  float *prediction = outputs[0].data();
  int stride = outputs[0].shape().size() / outputs[0].shape()[0];

  int offset = 0;
  for (int subId = 0; subId < total; subId++) {
    vector<Bbox> boxes;
    yolo_v_3_->postProcess(prediction + offset, p->imageV_[startId + subId].width,
                           p->imageV_[startId + subId].height, initParam_.thresh_, boxes);

    if (square_face_) {
      ConvertSquare(boxes, exth_scale_face_, extw_scale_face_);
    }

    for (auto &box : boxes) {
      DOG_DETECT_TINY_INFO info;
      info.rect.x = box.x1;
      info.rect.y = box.y1;
      info.rect.width = box.x2 - box.x1;
      info.rect.height = box.y2 - box.y1;
      info.score = box.score;
      r->detInfos_[startId + subId].push_back(info);
    }
    offset += stride;
  }

  return ErrorType::ERR_SUCCESS;
}

void dogDetTinyImpl::InitPreprocessorParam() {
  preProcessor_.reset(new OpencvPreprocessor);
  OpencvPreprocessorInitPt initPt;
  initPt.net_h_ = input_h_;
  initPt.net_w_ = input_w_;
  initPt.mean_ = 0.0;
  initPt.std_ = 255.0;
  initPt.alignMent_ = AlignMent::AlignCenter;
  initPt.detType_ = DetType::Face;
  initPt.border_value = 128;

  auto code = preProcessor_->Init(&initPt);
  assert(code == ErrorType::ERR_SUCCESS);
}

void dogDetTinyImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::detFace);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
}

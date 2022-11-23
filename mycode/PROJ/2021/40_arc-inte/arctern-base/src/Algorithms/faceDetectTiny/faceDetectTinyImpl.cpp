/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  modified:       2020.08.14
 *  email:          chen.gaohua@intellif.com
 *  Last modified:  2021.06.04 by kuanghongshen
 ****************************************************************************/

#include "faceDetectTinyImpl.h"

namespace arctern {

faceDetectTinyImpl::faceDetectTinyImpl() : m_bNormalized(true) {

}

faceDetectTinyImpl::~faceDetectTinyImpl() {
}

int faceDetectTinyImpl::GetConfigInfo(arctern::MxnetModel *model) {
  // todo check
  std::string yolo;
  auto bRet = try_get_value(*model, "type", yolo);
  if (bRet && yolo == "yolov5") {
    m_version = 5;
  }
  else {
    m_version = 3;
  }

  int ret;
  if (m_version == 5) {
    yolo_v_5_ = std::make_unique<YoloV5>();
    ret = yolo_v_5_->getConfigInfo(model);
    input_h_ = yolo_v_5_->getNetH();
    input_w_ = yolo_v_5_->getNetW();
    square_face_ = false;

  } else {
    yolo_v_3_ = std::make_unique<YoloV3>();
    ret = yolo_v_3_->getConfigInfo(model);
    input_h_ = yolo_v_3_->getNetH();
    input_w_ = yolo_v_3_->getNetW();
    square_face_ = true;
  }

//  initPreProcessor();
  return ret;
}

ErrorType faceDetectTinyImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                               std::vector<arctern::Tensor<float>> &outputs) {
  auto r = dynamic_cast<FaceDetectTinyResult *>(result_);
  int total = endId - startId;
  float *prediction = outputs[0].data();
  int stride = outputs[0].shape().size() / outputs[0].shape()[0];

  float *prediction1 = nullptr;
  float *prediction2 = nullptr;
  int stride1 = 0;
  int stride2 = 0;

  if (yolo_v_5_) {
    prediction1 = outputs[1].data();
    prediction2 = outputs[2].data();
    stride1 = outputs[1].shape().size() / outputs[1].shape()[0];
    stride2 = outputs[2].shape().size() / outputs[2].shape()[0];
  }

  int offset = 0;
  int offset1 = 0;
  int offset2 = 0;
  for (int subId = 0; subId < total; subId++) {
    vector<Bbox> boxes;
    if (m_version == 5) {
      boxes = yolo_v_5_->postProcess2(prediction + offset, prediction1 + offset1, prediction2 + offset2,
                                     p->imageV_[startId + subId].width,
                                     p->imageV_[startId + subId].height, initParam_.thresh_);
    } else {
      yolo_v_3_->postProcess(prediction + offset, p->imageV_[startId + subId].width,
                                     p->imageV_[startId + subId].height, initParam_.thresh_, boxes);
    }

    if (square_face_) {
      ConvertSquare(boxes, exth_scale_face_, extw_scale_face_);
    }

    for (auto &box : boxes) {
      FACE_DETECT_TINY_INFO info;
      info.rect.x = box.x1;
      info.rect.y = box.y1;
      info.rect.width = box.x2 - box.x1;
      info.rect.height = box.y2 - box.y1;
      info.score = box.score;
      r->detInfos_[startId + subId].push_back(info);
    }
    offset += stride;
    offset1 += stride1;
    offset2 += stride2;
  }

  return ErrorType::ERR_SUCCESS;
}

void faceDetectTinyImpl::InitPreprocessorParam() {
  preProcessor_.reset(new OpencvPreprocessor);
  OpencvPreprocessorInitPt initPt;
  initPt.net_h_ = input_h_;
  initPt.net_w_ = input_w_;
  initPt.mean_ = 0.0;
  initPt.std_ = 255.0;
  initPt.alignMent_ = AlignMent::AlignCenter;
  initPt.detType_ = DetType::Face;
  if (m_version == 3) {
    initPt.border_value = 128;
  } else if (m_version == 5) {
    initPt.border_value = 114;
  }
  auto code = preProcessor_->Init(&initPt);
  assert(code == ErrorType::ERR_SUCCESS);
}

void faceDetectTinyImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {

  runParm->sequence_.push_back(OperatorType::detFace);

  runParm->sequence_.push_back(OperatorType::cvtColor);
  if(m_bNormalized) {
    runParm->sequence_.push_back(OperatorType::normalized);
  }

}

}

//
// Created by Admin on 2021/12/22.
//

#include "fireDetImpl.h"
#include "fireDet.h"

using namespace arctern;

FireDetImpl::FireDetImpl(){
  dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_RGB888;
}

FireDetImpl::~FireDetImpl() {}

int FireDetImpl::GetConfigInfo(arctern::MxnetModel *model) {
  // todo check
  m_yolo_v_4 = std::make_unique<YoloV4>();
  auto ret = m_yolo_v_4->getConfigInfo(model);
  input_h_ = m_yolo_v_4->getNetH();
  input_w_ = m_yolo_v_4->getNetW();
  auto config = model->configs();
  if(config != nullptr){
    for(auto it = config->begin();it != config->end(); it++){
      std::string tmp = it->key()->str();
      if(tmp == "CLASS_NAME")
      {
        type = it->value()->str();
      }
    }
  }
  return ret;
}

ErrorType
FireDetImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                         std::vector<arctern::Tensor<float>> &outputs) {
  auto r = dynamic_cast<FireDetResult*>(result_);
  int total = endId - startId;
  float *prediction = outputs[0].data();
  int stride = outputs[0].shape().size() / outputs[0].shape()[0];

  int offset = 0;
  for (int subId = 0; subId < total; ++ subId) {
    vector<Bbox> boxes;
    if(type == "fire"){
      boxes = m_yolo_v_4->postProcess(prediction + offset, p->imageV_[startId + subId].width,
                                       p->imageV_[startId + subId].height, initParam_.thresh_);
    }
    else if(type == "smoke"){
      boxes = m_yolo_v_4->postProcess(prediction + offset, p->imageV_[startId + subId].width,
                                      p->imageV_[startId + subId].height, initParam_.thresh_);
    }
    else {
      boxes = m_yolo_v_4->postProcess2(prediction + offset, p->imageV_[startId + subId].width,
                                      p->imageV_[startId + subId].height, initParam_.thresh_);
    }

    for (auto &box : boxes) {
      FIRE_DETECT_TINY_INFO info;
      info.rect.x = box.x1;
      info.rect.y = box.y1;
      info.rect.width = box.x2 - box.x1;
      info.rect.height = box.y2 - box.y1;
      info.score = box.score;

      if(type == "smoke"){
        info.type = arctern::FireType(box.label + 1);
      }
      else{
        info.type = arctern::FireType(box.label);
      }

      r->detInfos_[startId + subId].push_back(info);
    }
    offset += stride;
  }

  return ErrorType::ERR_SUCCESS;
}

void FireDetImpl::InitPreprocessorParam() {
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

void FireDetImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
}

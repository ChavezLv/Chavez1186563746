//
// Created by Admin on 2019/6/11.
//


#include "TrtDogDetTiny.h"
namespace arctern {
TrtDogDetTinyImpl::TrtDogDetTinyImpl(){
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
}

TrtDogDetTinyImpl::~TrtDogDetTinyImpl(){

}

void TrtDogDetTinyImpl::InitPreprocessorParam(){
  m_pStream = m_vEngines[0]->getContext()->getCudaStream();
  m_pPreprocessor.reset(new NppPreprocessor(m_pStream));
  NppPreprocessorInitPt initPt;
  initPt.netH_ = m_modelsInputHeight;
  initPt.netW_ = m_modelsInputWidth;
  initPt.mean_ = 0.0;
  initPt.std_ = 255.0;
  initPt.alignMent_ = AlignMent::AlignCenter;
  initPt.detType_ = DetType::Face;
  initPt.borderValue_ = 128;
  assert(m_pPreprocessor);
  m_lastErrorCode = m_pPreprocessor->Init(&initPt);
  assert(m_lastErrorCode == ErrorType::ERR_SUCCESS);
}

void TrtDogDetTinyImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){

 runParm->sequence_.push_back(OperatorType::detFace);
 runParm->sequence_.push_back(OperatorType::normalized);
 runParm->sequence_.push_back(OperatorType::cvtColor);
 runParm->sequence_.push_back(OperatorType::extractChannel);
}

ErrorType TrtDogDetTinyImpl::GetModelsConfigInfo(const MxnetModels *models){

  m_yolo_v_3 = make_unique<YoloV3>();
  auto model =const_cast<MxnetModel*>( models->models()->Get(0));
  m_yolo_v_3->getConfigInfo(model);
  m_modelsInputWidth = m_yolo_v_3->getNetW();
  m_modelsInputHeight = m_yolo_v_3->getNetH();
  m_bSquareFace = false;
  return ERR_SUCCESS;
}

ErrorType TrtDogDetTinyImpl::LocalPostProcess(const RunParameter *p, int startId,
                                                  int endId, vector<arctern::OutputTensor<float>> &outputs) {
  auto r = dynamic_cast<dogDetTinyResult *>(m_pResult);
  int total = endId - startId;
  float *prediction = outputs[0].data();
  int stride = outputs[0].shape().size() / outputs[0].shape()[0];

  int offset = 0;
  for (int subId = 0; subId < total; subId++) {
    vector<Bbox> boxes;
    m_yolo_v_3->postProcess(prediction+offset,p->imageV_[startId + subId].width,
                            p->imageV_[startId+subId].height, m_initParam.thresh_,boxes);

    if (m_bSquareFace) {
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

}
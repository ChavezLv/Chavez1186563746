//
// Created by Admin on 2019/6/11.
//


#include "TrtFaceDetectTiny.h"
namespace arctern {
TrtFaceDetectTinyImpl::TrtFaceDetectTinyImpl(){
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
}

TrtFaceDetectTinyImpl::~TrtFaceDetectTinyImpl(){

}


ErrorType TrtFaceDetectTinyImpl::LocalPostProcess(const RunParameter *p, int startId,
                                                  int endId, vector<arctern::OutputTensor<float>> &outputs) {


  auto r = dynamic_cast<FaceDetectTinyResult *>(m_pResult);

  int total = endId - startId;

  float *prediction = outputs[0].data();
  int stride = outputs[0].shape().size() / outputs[0].shape()[0];

  float * prediction1 = nullptr; ///> for yolo v5
  float * prediction2 = nullptr;///> for yolo v5
  int stride1 = 0;///> for yolo v5
  int stride2 = 0;///> for yolo v5
  if(m_version == 5) {
    prediction1 = outputs[2].data();
    prediction2 = outputs[1].data();
    stride1 = outputs[2].shape().size() / outputs[2].shape()[0];
    stride2 = outputs[1].shape().size() / outputs[1].shape()[0];
  }

  int offset = 0;
  int offset1 = 0;///> for yolo v5
  int offset2 = 0;///> for yolo v5
  vector<Bbox> boxes;
  for (int subId = 0; subId < total; subId++) {
    if(m_version == 3){
      m_yolo_v_3->postProcess(prediction+offset,p->imageV_[startId + subId].width,
                              p->imageV_[startId+subId].height, m_initParam.thresh_,boxes);
    }
    else if(m_version == 5){
      boxes=m_yolo_v_5->postProcess(prediction + offset, prediction1 + offset1, prediction2 + offset2,
                              p->imageV_[startId + subId].width,
                              p->imageV_[startId+subId].height, m_initParam.thresh_);
    }


    if (m_bSquareFace) {
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

void TrtFaceDetectTinyImpl::InitPreprocessorParam(){
  m_pStream = m_vEngines[0]->getContext()->getCudaStream();
  m_pPreprocessor.reset(new NppPreprocessor(m_pStream));
  NppPreprocessorInitPt initPt;
  initPt.netH_ = m_modelsInputHeight;
  initPt.netW_ = m_modelsInputWidth;
  initPt.mean_ = 0.0;
  initPt.std_ = 255.0;
  initPt.alignMent_ = AlignMent::AlignCenter;
  initPt.detType_ = DetType::Face;
  if(m_version == 3){
    initPt.borderValue_ = 128;
  }
  else if(m_version == 5){
    initPt.borderValue_ = 114;
  }
  assert(m_pPreprocessor);
  m_lastErrorCode = m_pPreprocessor->Init(&initPt);
  assert(m_lastErrorCode == ErrorType::ERR_SUCCESS);
}

void TrtFaceDetectTinyImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  if(m_version == 5){
    runParm->sequence_.push_back(OperatorType::detFace2);
  }
  else{
    runParm->sequence_.push_back(OperatorType::detFace);
  }
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::extractChannel);
  runParm->sequence_.push_back(OperatorType::normalized);
}

ErrorType TrtFaceDetectTinyImpl::GetModelsConfigInfo(const MxnetModels *models){
  assert(models);
  auto model = models->models()->Get(0);
  std::string yolo;
  auto bRet = try_get_value(*model,"type",yolo);
  if(bRet && yolo == "yolov5") m_version = 5;
  else m_version = 3;

  switch (m_version) {
    case 3: {
      auto model =const_cast<MxnetModel*>( models->models()->Get(0));
      m_yolo_v_3 = make_unique<YoloV3>();

      m_yolo_v_3->getConfigInfo(model);
      m_modelsInputWidth = m_yolo_v_3->getNetW();
      m_modelsInputHeight = m_yolo_v_3->getNetH();
      m_bSquareFace = true;
    }
      break;
    case 5: {
      m_yolo_v_5 = std::make_unique<YoloV5>();
      m_yolo_v_5->getConfigInfo(nullptr);
      m_modelsInputHeight = m_yolo_v_5->getNetH();
      m_modelsInputWidth = m_yolo_v_5->getNetW();
      m_bSquareFace = false;
    }
      break;
  }

  return ERR_SUCCESS;
}
}
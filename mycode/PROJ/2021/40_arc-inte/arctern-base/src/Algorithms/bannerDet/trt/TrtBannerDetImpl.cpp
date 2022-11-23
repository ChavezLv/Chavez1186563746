//
// Created by lvchaolin on 2021/12/8.
//

#include "TrtBannerDetImpl.h"

namespace arctern {

TrtBannerDetectImpl::TrtBannerDetectImpl() {
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
}

TrtBannerDetectImpl::~TrtBannerDetectImpl() {

}

ErrorType TrtBannerDetectImpl::LocalPostProcess(const RunParameter *p, \
                            int startId, int endId,vector<arctern::OutputTensor<float>> &outputs){
    

  auto r = dynamic_cast<BannerDetResult*>(m_pResult);

  int total = endId - startId;
  float *prediction = outputs[0].data();
  int stride = outputs[0].shape().size() / outputs[0].shape()[0];
  int offset = 0;
  for(int subId = 0; subId < total; ++subId){
    vector<Bbox> boxes;    
    m_yolo_v_3->postProcess3(prediction + offset, p->imageV_[startId + subId].width,\
                p->imageV_[startId + subId].height, m_initParam.thresh_, boxes);
    if(m_bSquareFace){
        ConvertSquare(boxes, m_extHeightScaleFace, m_extWidthScaleFace);
    }
    for(auto &box : boxes){
        BANNER_DETECT_TINY_INFO info;
        info.rect.x = box.x1;
        info.rect.y = box.y1;
        info.rect.width = box.x2 - box.x1;
        info.rect.height = box.y2 - box.y1;
        info.score = box.score;
        info.type = arctern::BannerType(box.label);
        r->detInfos_[startId + subId].push_back(info);
    }
    offset += stride;

  }//end of for(int subId = 0; subId < total; ++subId)
  
  return ErrorType::ERR_SUCCESS;
}

void TrtBannerDetectImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
  runParm->sequence_.push_back(OperatorType::normalized);
}

ErrorType TrtBannerDetectImpl::GetModelsConfigInfo(const MxnetModels *models) {
  assert(models);

  auto model = const_cast<MxnetModel*>(models->models()->Get(0));

  m_yolo_v_3 = make_unique<YoloV3>();
  m_yolo_v_3->getConfigInfo(model);
  m_modelsInputWidth = m_yolo_v_3->getNetW();
  m_modelsInputHeight = m_yolo_v_3->getNetH();
  m_bSquareFace = false;
  return ERR_SUCCESS;

}


//void TrtBannerDetectImpl::InitPostProcessParam() {
//  if (m_modelsInputHeight == m_modelsInputWidth) {
//    auto padSize = img_width > img_height ? img_width : img_height;
//    v_scale_h.push_back(static_cast<float>(padSize) /
//        static_cast<float>(m_modelsInputHeight));
//    v_scale_w.push_back(static_cast<float>(padSize) /
//        static_cast<float>(m_modelsInputWidth));
//  } else {
//    v_scale_h.push_back(static_cast<float>(img_height) /
//        static_cast<float>(m_modelsInputHeight));
//    v_scale_w.push_back(static_cast<float>(img_width) /
//        static_cast<float>(m_modelsInputWidth));
//  }
//}

void TrtBannerDetectImpl::InitPreprocessorParam(){
  m_pStream = m_vEngines[0]->getContext()->getCudaStream(); 
  m_pPreprocessor.reset(new NppPreprocessor(m_pStream));
  NppPreprocessorInitPt initPt;
  initPt.netH_ = m_modelsInputWidth;
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


}

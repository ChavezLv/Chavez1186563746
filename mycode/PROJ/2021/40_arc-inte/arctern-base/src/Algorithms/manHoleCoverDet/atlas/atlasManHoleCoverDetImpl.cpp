//
// Created by lzm on 2020/12/17.
//

#include "atlasManHoleCoverDetImpl.h"
#include "src/common/atlas/atlasAclDataType.h"

#include "Engin/src/atlas/atlasengin.h"
#include "common/algUtil.h"

namespace arctern{

AtlasManHoleCoverDetImpl::AtlasManHoleCoverDetImpl(){

}

AtlasManHoleCoverDetImpl::~AtlasManHoleCoverDetImpl() {
  
}

ErrorType AtlasManHoleCoverDetImpl::GetModelsConfigInfo(const TrtModels* models){
  assert(models);
  auto model = models->models()->Get(0);
  m_version = 3;
  auto config = model->configs();
  if(config != nullptr){
    for(auto it = config->begin();it != config->end(); it++){
      if(it->key()->str() == "type") {
        if(it->value()->str() == "yolov4")  m_version = 4;
        if(it->value()->str() == "yolov5")  m_version = 5;
      }
    }
  }
  switch (m_version) {
    case 3: {
      auto model =const_cast<TrtModel*>( models->models()->Get(0));
      m_yolo_v_3 = make_unique<YoloV3>();

      m_yolo_v_3->getConfigInfo(model);
      m_modelsInputWidth = m_yolo_v_3->getNetW();
      m_modelsInputHeight = m_yolo_v_3->getNetH();
      m_bSquareFace = false;
    }
      break;
    case 4: {
      auto model =const_cast<TrtModel*>( models->models()->Get(0));
      m_yolo_v_4 = make_unique<YoloV4>();

      m_yolo_v_4->getConfigInfo(model);
      m_modelsInputWidth = m_yolo_v_4->getNetW();
      m_modelsInputHeight = m_yolo_v_4->getNetH();
      m_bSquareFace = false;
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

  return ErrorType::ERR_SUCCESS;
}




ErrorType AtlasManHoleCoverDetImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
std::vector<std::vector<float>> &outputs) {
  
  auto r = dynamic_cast<ManHoleCoverDetResult *>(m_pResult);

  int total = endId - startId;

  float *prediction = outputs[0].data();
  int stride = outputs[0].size() / total;

  float * prediction1 = nullptr; ///> for yolo v5
  float * prediction2 = nullptr;///> for yolo v5
  int stride1 = 0;///> for yolo v5
  int stride2 = 0;///> for yolo v5
  if(m_version == 5) {
    prediction1 = outputs[1].data();
    prediction2 = outputs[2].data();
    stride1 = outputs[1].size() / total;
    stride2 = outputs[2].size() / total;
  }

  int offset = 0;
  int offset1 = 0;///> for yolo v5
  int offset2 = 0;///> for yolo v5
  vector<Bbox> boxes;
  for (int subId = 0; subId < total; subId++) {
    if(m_version == 3){
      m_yolo_v_3->postProcess2(prediction+offset,p->imageV_[startId + subId].width,
                              p->imageV_[startId+subId].height, m_initParam.thresh_,boxes);
    }
    else if(m_version == 4){
      m_yolo_v_4->postProcess(prediction+offset,p->imageV_[startId + subId].width,
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
      MANHOLECOVER_DETECT_TINY_INFO info;
      info.rect.x = box.x1;
      info.rect.y = box.y1;
      info.rect.width = box.x2 - box.x1;
      info.rect.height = box.y2 - box.y1;
      info.score = box.score;
      info.type = (ManholeCoverAbnormalType) box.label;
      r->detInfos_[startId + subId].push_back(info);
    }
    offset += stride;
    offset1 += stride1;
    offset2 += stride2;
  }
  return m_lastErrorCode;

}



ErrorType AtlasManHoleCoverDetImpl::LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
std::vector<DvppDataInfo>& vOutputInfos,
std::vector<cv::Rect>& vRects) {
  
  int ret;
  if(m_version == 3){
    ret = m_pPreprocessor->CombineResizeBatchProcess(vInputInfo, vOutputInfos, true, VpcProcessType::VPC_PT_PADDING);
  }
  else{
    ret = m_pPreprocessor->CombineResizeBatchProcess(vInputInfo, vOutputInfos, true, VpcProcessType::VPC_PT_DEFAULT);
  } 

  if (ret != APP_ERR_OK) {
    LOGASDError << "failed to do altas preprocess";
    m_lastErrorCode = ErrorType::ERR_PRE_PROCESS;
  }
  return m_lastErrorCode;
}


}

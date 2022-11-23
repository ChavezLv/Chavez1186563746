//
// Created by Admin on 2020/6/2.
//

#include "ArcternAlgorithmEx.h"
#include "common/common.h"

#define CHECK_RESULT() \
        if (m_lastErrorCode != ErrorType::ERR_SUCCESS)  \
            return (ErrorType) m_lastErrorCode;

ArcternAlgorithmEx::ArcternAlgorithmEx() : m_pResult(nullptr) {

}

ArcternAlgorithmEx::~ArcternAlgorithmEx() noexcept {
  ArcternAlgorithmEx::Release();
}

ErrorType ArcternAlgorithmEx::Init(const InitParameter *p) {
  ///> check algorithm init parameter
  m_lastErrorCode = CheckInitParam(p);
  CHECK_RESULT()
  ///> save algorithm init parameter
  m_initParam = *p;
  m_pDecodeMgr = make_shared<DvppDecodeManager>(m_initParam.gpuId_);

  if(!InitPreprocessor()) {
    m_lastErrorCode = arctern::ErrorType::ERR_PRE_PROCESS;
  }

  return m_lastErrorCode;
}

void ArcternAlgorithmEx::Release() {

}

ErrorType ArcternAlgorithmEx::CommonPreProcess( const RunParameter *p,const int& idx,DvppDataInfo &decodeData){
  if(p->imageV_[idx].type == CPU){
  
    auto ret = m_pDecodeMgr->CombineJpegdProcess(p->imageV_[idx].imgdata.atlasData, PIXEL_FORMAT_YUV_SEMIPLANAR_420, true);
    if(!ret){
      std::cout << "dvpp for jpg decode failed!" << std::endl;
      m_lastErrorCode = ErrorType::ERR_PRE_PROCESS;
      CHECK_RESULT()
    }
    
    auto pDecodeData = m_pDecodeMgr->GetDecodedImage();

    decodeData.width = pDecodeData->width;
    decodeData.height = pDecodeData->height;
    decodeData.widthStride = pDecodeData->widthStride;
    decodeData.heightStride = pDecodeData->heightStride;
    decodeData.format = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    decodeData.dataSize = decodeData.widthStride * decodeData.heightStride * YUV_BGR_SIZE_CONVERT_3 / YUV_BGR_SIZE_CONVERT_2;
    decodeData.data = pDecodeData->data;

  }
  else{
    decodeData.width = p->imageV_[idx].width;
    decodeData.height = p->imageV_[idx].height;
    decodeData.widthStride = p->imageV_[idx].imgdata.atlasData.widthStride;
    decodeData.heightStride = p->imageV_[idx].imgdata.atlasData.heightStride;
    decodeData.format = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    decodeData.dataSize = decodeData.widthStride * decodeData.heightStride * YUV_BGR_SIZE_CONVERT_3 / YUV_BGR_SIZE_CONVERT_2;
    decodeData.data = static_cast<uint8*>(p->imageV_[idx].imgdata.atlasData.rawData);
  }
  return m_lastErrorCode;
}

cv::Rect ArcternAlgorithmEx::LocalGetROI(const RunParameter *p, int idx){
  if(!p->rectsV_.empty()){
    return cv::Rect(p->rectsV_[idx].x,p->rectsV_[idx].y,p->rectsV_[idx].width,p->rectsV_[idx].height);
  } 
  return cv::Rect(0,0,0,0);
}

ErrorType ArcternAlgorithmEx::Process(const RunParameter *p,Result *r) {
  m_lastErrorCode = CheckRunParam(p);
  CHECK_RESULT()

  if(r == nullptr) m_lastErrorCode = ERR_NULL_POINT;
  CHECK_RESULT()

  int imgNum = p->imageV_.size();
  m_pResult = r;
  m_pResult->Release();
  m_pResult->resize(imgNum);
  
  int startId = 0;
  int endId = startId + m_initParam.batchNum_;
  endId = endId >= imgNum ? imgNum : endId;

  while (startId < imgNum) {
    int num = endId - startId;

    std::vector<DvppDataInfo> vInputInfos(num);
    std::vector<DvppDataInfo> vOutputInfos(m_initParam.batchNum_);
    std::vector<cv::Rect> vRects(num);
    for (int i = 0; i < num; i++) {
      m_lastErrorCode = CommonPreProcess(p,i + startId,vInputInfos[i]);
      CHECK_RESULT()

      auto ptr = const_cast<RunParameter*>(p);
      ptr->imageV_[i].width = vInputInfos[i].width;
      ptr->imageV_[i].height = vInputInfos[i].height;
      vRects[i] = LocalGetROI(ptr,i + startId);

      vOutputInfos[i].width = m_modelsInputWidth;
      vOutputInfos[i].height = m_modelsInputHeight;
      vOutputInfos[i].format = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    }

    m_lastErrorCode = LocalPreProcess(vInputInfos,vOutputInfos,vRects);
    CHECK_RESULT()
    
    ///> do inference
    std::vector<std::pair<const void *, size_t> > inputData;
    std::vector<std::vector<float> > outputData;
    inputData.push_back(m_pPreprocessor->GetPreprocessedDvppPtr());

    forward(inputData,outputData,num);

    m_lastErrorCode = LocalPostProcess(p,startId,endId,outputData);
    startId = endId;
    endId += m_initParam.batchNum_;
    endId = endId > imgNum ? imgNum : endId;
  }

  return m_lastErrorCode;
};

ErrorType ArcternAlgorithmEx::CheckInitParam(const InitParameter *p) {
  if (p == nullptr || p->model_path.empty() || p->batchNum_ < 1) {
    ArcternAlgorithmEx::Release();
    return ErrorType::ERR_INIT_PARAM;
  }
  if (!isExist(p->model_path)) {
    return ErrorType::ERR_MODEL_NO_EXIST;
  }
  if(p->gpuId_ < 0){
    return ErrorType::ERR_GPU_NO_FIND;
  }
  return ErrorType::ERR_SUCCESS;
}

ErrorType ArcternAlgorithmEx::GetLastError() {
  return m_lastErrorCode;
}

void ArcternAlgorithmEx::SetEngines(std::vector<std::shared_ptr<AtlasEngine>> &engines) {
  m_vEngines = std::move(engines);
}


ErrorType ArcternAlgorithmEx::CheckRunParam(const RunParameter *p) {

  if (nullptr == p || p->imageV_.empty()) {
    SPDLOG_ERROR("algorithm run parameter is nullptr or images vector is empty!!!");
    m_lastErrorCode = ErrorType::ERR_RUN_PARM;
    return m_lastErrorCode;
  }

  m_lastErrorCode = p->CheckSize();
  if (m_lastErrorCode != ErrorType::ERR_SUCCESS) {
    SPDLOG_ERROR("algorithm run parameter check size failed!!!");
    return m_lastErrorCode;
  }

  for (const auto &i : p->imageV_) {

    if ( i.imgdata.atlasData.rawData == nullptr || i.imgdata.atlasData.rawDataLen <= 0) {
      SPDLOG_ERROR("some input images is nullptr at imageV_ !!! please check !!!");
      return ErrorType::ERR_NO_IMAGE_DATA;
    }

    if(i.type == GPU){
      if(i.imgdata.atlasData.heightStride == 0 || i.imgdata.atlasData.widthStride == 0
      || i.width == 0 || i.height == 0){
        SPDLOG_ERROR("image type=NPU, width and height and stride must not be zero !!! please check !!!");
      }
    }
  }

  return ErrorType::ERR_SUCCESS;
}

bool ArcternAlgorithmEx::InitPreprocessor(){
  ///> init atlas preprocess handles
  m_pPreprocessor = std::make_shared<AtlasAclPreprocess>(m_vEngines[0]->getAclContext(), m_vEngines[0]->getAclStream());
  auto ret = m_pPreprocessor->init(m_vEngines[0]->getMaxBatchSize(), m_modelsInputWidth, m_modelsInputHeight);
  if (ret != APP_ERR_OK) {
    LOGASDError << "failed to init atlas preprocess handle";
    return false;
  }
  return true;
}

int ArcternAlgorithmEx::forward(const std::vector<std::pair<const void *, size_t>> &input,
                                std::vector<std::vector<float>> &outputs,int batchSize) {
  return m_vEngines[0]->forward(input, outputs,batchSize);
}


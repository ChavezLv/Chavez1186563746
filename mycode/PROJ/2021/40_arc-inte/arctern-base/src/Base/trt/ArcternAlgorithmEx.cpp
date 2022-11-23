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

  ///> init preprocessor init parameter
  InitPreprocessorParam();

  return m_lastErrorCode;
}

void ArcternAlgorithmEx::Release() {

}

ErrorType ArcternAlgorithmEx::Process(const RunParameter *p,Result *r) {
  if(cudaSetDevice(m_initParam.gpuId_) != cudaSuccess){
    m_lastErrorCode = ErrorType::ERR_GPU_NO_FIND;
  }
  CHECK_RESULT()


  m_lastErrorCode = CheckRunParam(p);
  CHECK_RESULT()

  if(r == nullptr) m_lastErrorCode = ERR_NULL_POINT;
  CHECK_RESULT()

  m_pResult = r;
  m_pResult->Release();

  //m_lastErrorCode = CommonPreProcess(p);
  //CHECK_RESULT()

  m_lastErrorCode = CommonProProcess(p);
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

void ArcternAlgorithmEx::SetEngines(std::vector<std::shared_ptr<TrtEngin>> &engines) {
  m_vEngines = std::move(engines);
}

void ArcternAlgorithmEx::InitPreprocessorParam() {
  m_pStream = m_vEngines[0]->getContext()->getCudaStream();
  assert(m_pStream != nullptr);
  m_pPreprocessor.reset(new NppPreprocessor(m_pStream));

  NppPreprocessorInitPt pPreprocessorInitParam ;
  pPreprocessorInitParam.netH_ = m_modelsInputHeight;
  pPreprocessorInitParam.netW_ = m_modelsInputWidth;
  pPreprocessorInitParam.mean_ = 127.5;
  pPreprocessorInitParam.std_ = 128.0;
  pPreprocessorInitParam.borderValue_ = 0;
  assert(m_pPreprocessor);
  m_lastErrorCode = m_pPreprocessor->Init(&pPreprocessorInitParam);
  assert(m_lastErrorCode == ErrorType::ERR_SUCCESS);
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

    if (i.width <= 0 || i.height <= 0 || i.imgdata.gdata == nullptr) {
      SPDLOG_ERROR("some input images is nullptr at imageV_ !!! please check !!!");
      return ErrorType::ERR_NO_IMAGE_DATA;
    }

    if(i.step <= 0){
      SPDLOG_ERROR("step is zero !!! please set right step");
      m_lastErrorCode = ErrorType ::ERR_RUN_PARM;
      return m_lastErrorCode;
    }

    if(i.step < i.width * 3){
      SPDLOG_ERROR("step less than 3 time of width !!! please set right step");
      m_lastErrorCode = ErrorType ::ERR_RUN_PARM;
      return m_lastErrorCode;
    }
  }


  return ErrorType::ERR_SUCCESS;
}

ErrorType ArcternAlgorithmEx::CommonPreProcess(const RunParameter *p) {
  auto pRunParam = p;
  size_t imgNum = pRunParam->imageV_.size();

  m_vPreprocessRet.clear();
  m_vPreprocessRet.reserve(imgNum);

  for (size_t loopId = 0; loopId < imgNum; loopId++) {

    // runParameter_ will be used in LocalPreProcess
    m_lastErrorCode = LocalPreProcess(pRunParam, (int) loopId);
    CHECK_RESULT();
  }

  return m_lastErrorCode;
}
void ArcternAlgorithmEx::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  ///> do at child class
}
void ArcternAlgorithmEx::ArcternImage2DeviceBuffer(const ArcternImage& arctern_image,DeviceBuffer &device_buffer){
  /*assert(device_buffer.nbBytes() == arctern_image.width * arctern_image.height * 3);
  auto buffer_step = arctern_image.width * 3;
  if(arctern_image.step > buffer_step){
    auto image_ptr = (uchar*) arctern_image.imgdata.gdata;
    auto buffer_ptr = (uchar*) device_buffer.data();
    for(int i=0;i<arctern_image.height;i++){
      if (arctern_image.type == CPU) {
        cudaMemcpyAsync(buffer_ptr, image_ptr, buffer_step, cudaMemcpyDeviceToDevice,*m_pStream);
      } else {
        cudaMemcpyAsync(buffer_ptr, image_ptr, buffer_step, cudaMemcpyDeviceToDevice,*m_pStream);
      }
      image_ptr += arctern_image.step;
      buffer_ptr += buffer_step;
    }
  }
  else{*/
    if (arctern_image.type == CPU) {
      cudaMemcpyAsync(device_buffer.data(), arctern_image.imgdata.gdata,
                 device_buffer.nbBytes(), cudaMemcpyHostToDevice,*m_pStream);
    } else {
      cudaMemcpyAsync(device_buffer.data(), arctern_image.imgdata.gdata,
                 device_buffer.nbBytes(), cudaMemcpyDeviceToDevice,*m_pStream);
    }
  //}

}
ErrorType ArcternAlgorithmEx::LocalPreProcess(const RunParameter *p, const int &idx) {

  NppPreprocessorRunPt runParameter;
  runParameter.m_cvtColorCode = GetColorConversionCodes(p->imageV_[idx].format, m_dstImageFormatForForward);

  runParameter.m_roi = LocalGetROI(p, idx);
  runParameter.arcternImage->resize(p->imageV_[idx].height, p->imageV_[idx].width,
                                    GetArcternImageChannels(p->imageV_[idx].format),
                                    nvinfer1::DataType::kINT8,
                                    p->imageV_[idx].step);

  ArcternImage2DeviceBuffer( p->imageV_[idx], *(runParameter.arcternImage));

  InitPreprocessorRunParam(&runParameter);
  auto result = m_pPreprocessor->Process(&runParameter);
  if (m_pPreprocessor->GetLastError() != 0) {
    m_lastErrorCode = ErrorType::ERR_PRE_PROCESS;
  }
  CHECK_RESULT()
  m_vPreprocessRet.emplace_back(std::move(result));

  return m_lastErrorCode;
}

ErrorType ArcternAlgorithmEx::CommonProProcess(const RunParameter *p) {
  const int batchNum = m_initParam.batchNum_;
  int imgNum = p->imageV_.size();

  // inference result
  this->m_pResult->resize(imgNum);
  m_vPreprocessRet.clear();
  m_vPreprocessRet.reserve(imgNum);

  int startId = 0;
  int endId = startId + batchNum;
  endId = endId > imgNum ? imgNum : endId;

  while (startId < imgNum) {

    std::vector<std::pair<const void *, size_t>> input;
    for (int i = startId; i < endId; i++) {
      LocalPreProcess(p,i);
      input.emplace_back(make_pair(m_vPreprocessRet[i]->arcternImage->data(),
                                m_vPreprocessRet[i]->arcternImage->nbBytes()));
      //printDeviceBuffer<float>(*m_vPreprocessRet[i]->arcternImage,"trt_input_img.data");
    }

    std::vector<OutputTensor<float>> outputs;

    forward(input, outputs);

    m_lastErrorCode = LocalPostProcess(p, startId, endId, outputs);
    CHECK_RESULT();

    startId = endId;
    endId += batchNum;
    endId = endId > imgNum ? imgNum : endId;
  }

  return m_lastErrorCode;
}

int ArcternAlgorithmEx::forward(const std::vector<std::pair<const void *, size_t>> &input,
                                std::vector<arctern::OutputTensor<float>> &outputs) {
  return m_vEngines[0]->forward(input, outputs);
}

ArcternRect ArcternAlgorithmEx::LocalGetROI(const RunParameter *p, int idx) {
  float height = p->imageV_[idx].height;
  float width = p->imageV_[idx].width;
  return ArcternRect{0, 0, width, height};
}

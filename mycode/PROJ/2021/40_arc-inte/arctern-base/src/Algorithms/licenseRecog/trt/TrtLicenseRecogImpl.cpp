//
// Created by Admin on 2021/6/18.
//

#include "TrtLicenseRecogImpl.h"
#include "src/common/common.h"
#include "src/common/trt/TrtAlgorithmUtil.h"
namespace arctern {

TrtLicenseRecogImpl::TrtLicenseRecogImpl() {
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
}

TrtLicenseRecogImpl::~TrtLicenseRecogImpl() {

}

ErrorType
TrtLicenseRecogImpl::GetModelsConfigInfo(const MxnetModels *models) {
  std::string version = models->version()->str();
  m_version = version;
  std::cout << "version = " << version << std::endl;

  m_modelsInputHeight = 48;
  m_modelsInputWidth = 192;

  return ErrorType::ERR_SUCCESS;
}

ErrorType TrtLicenseRecogImpl::LocalPostProcess(const RunParameter *p,
                                                int startId,
                                                int endId,
                                                vector<arctern::OutputTensor<float>> &outputs) {
  auto r = dynamic_cast<LicenseRecogResult *>(m_pResult);

  int total = endId - startId;

  int stride0 = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < total; subId++) {
    const float *data0 = outputs[0].data() + subId * stride0;
    std::vector<float> res;
    if(m_version == "2.0.0"){
      res.resize(stride0);
      memcpy(res.data(),data0,sizeof (float )*stride0);
      r->score_[startId + subId].clear();
      r->LicenseRecogs_[startId + subId] = CalculateResult(res,r->score_[startId + subId]);
    }else{

      if (*(data0) != INDEXFORSPACE) {
        res.push_back(*(data0));
      }
      for (int i = 1; i < stride0; i++) {
        if(*(data0 + i) == *(data0 + i - 1) ||
            *(data0 + i) == INDEXFORSPACE) {
          continue;
        }
        res.push_back(*(data0 + i));
      }
      r->LicenseRecogs_[startId + subId] = CalculateResult(res);
    }
  }
  return ErrorType::ERR_SUCCESS;
}

ArcternRect TrtLicenseRecogImpl::LocalGetROI(const RunParameter *p, int idx) {
  if(m_version == "2.0.0"){
    if(!p->rectsV_.empty()) return p->rectsV_[idx];
    else return ArcternRect {0.f,0.f,0.f,0.f};
  }

  const auto pRunParam = dynamic_cast<const RunParameter *>(p);
  auto &faceRect = pRunParam->rectsV_[idx];
  auto rect = CheckShrinkBox(faceRect, p->imageV_[idx].width, p->imageV_[idx].height);
  return rect;
}

void TrtLicenseRecogImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  if(m_version == "2.0.0"){
    runParm->sequence_.push_back(OperatorType::cvtColor);
    runParm->sequence_.push_back(OperatorType::resized);
    runParm->sequence_.push_back(OperatorType::extractChannel);
    runParm->sequence_.push_back(OperatorType::normalized);
  }
  else{
    runParm->sequence_.push_back(OperatorType::crop);
    runParm->sequence_.push_back(OperatorType::cvtColor);
    runParm->sequence_.push_back(OperatorType::normalized);
    runParm->sequence_.push_back(OperatorType::resized);
    runParm->sequence_.push_back(OperatorType::extractChannel);
  }
}
std::string TrtLicenseRecogImpl::CalculateResult(const std::vector<float> &output,std::vector<float> &score){
  auto predict = output.data();

  std::vector<std::string> license;
  int pre_c = 0;
  int ctc_len = 16;
  int  ignore_code = 34;
  int max_len = CORPUS_LEN;
  for(int i = 0; i < ctc_len; i++)
  {
    int max_idx = 0;
    float max_score = -1;
    for(int j = 0; j < max_len; j++)
    {
      if (predict[i*max_len+j] > max_score)
      {
        max_idx = j;
        max_score = predict[i*max_len+j];
      }
    }
    if(i == 0)
    {
      pre_c = max_idx;
      if(pre_c != max_len - 1 && max_idx != ignore_code)
      {
        license.push_back(LicenseCharacters[max_idx]);
        score.push_back(max_score);
      }
    }
    else
    {
      if (max_idx != max_len-1 && max_idx != pre_c && max_idx != ignore_code)
      {
        license.push_back(LicenseCharacters[max_idx]);
        score.push_back(max_score);
      }
    }
    pre_c = max_idx;
  }
  string ret = "";
  for(const auto &item:license){
    ret += item;
  }
  return ret;
}
std::string TrtLicenseRecogImpl::CalculateResult(const std::vector<float> &output) {
  std::string license = "";
  for (size_t i = 0; i < output.size(); i++) {
    int index = static_cast<int>(output[i]) < 0 ? 0 : static_cast<int>(output[i]);
    index = index > INDEXFORSPACE ? INDEXFORSPACE : index;
    if (LicenseCharacters[index] == "^") {
      break;
    }
    license = license + LicenseCharacters[index];
  }
  return license;
}

}

//
// Created by chengaohua on 2021/7/6.
//

#include "trtPersonPoseImpl.h"
#include "../personPose.h"
#include "../utils/personPoseUtils.h"
#include "src/common/algUtil.h"
namespace arctern {
TrtPersonPoseImpl::TrtPersonPoseImpl() {
  m_modelsInputWidth = 192;
  m_modelsInputHeight = 256;
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
}

TrtPersonPoseImpl::~TrtPersonPoseImpl() throw() {

}

ErrorType TrtPersonPoseImpl::Process(const RunParameter *p,Result *r) {
  upBBoxes_.clear();
  return ArcternAlgorithmEx::Process(p,r);
}


void TrtPersonPoseImpl::InitPreprocessorParam() {
  m_pPreprocessor = std::make_unique<NppPreprocessor>(m_vEngines[0]->getContext()->getCudaStream());
  NppPreprocessorInitPt pPreprocessorInitParam;
  pPreprocessorInitParam.netH_ = m_modelsInputHeight;
  pPreprocessorInitParam.netW_ = m_modelsInputWidth;
  pPreprocessorInitParam.mean_ = 127.5;
  pPreprocessorInitParam.std_ = 127.5;

  assert(m_pPreprocessor);
  m_lastErrorCode = m_pPreprocessor->Init(&pPreprocessorInitParam);
}

ErrorType TrtPersonPoseImpl::GetModelsConfigInfo(const MxnetModels *models) {
  ErrorType result;
  auto model = models->models()->Get(0);
  bool ret = try_get_value(*model, "NET_INPUT_H", m_modelsInputHeight);
  ret &= try_get_value(*model, "NET_INPUT_W", m_modelsInputWidth);

  return result;
}

ErrorType TrtPersonPoseImpl::LocalPostProcess(const RunParameter *p,
                                              int startId,
                                              int endId,
                                              vector<arctern::OutputTensor<float>> &outputs) {
  auto *p_res = dynamic_cast<PersonPoseResult *>(m_pResult);

  int i_total = endId - startId;
  int i_stride = outputs[0].size() / outputs[0].shape()[0];

  for (int subId = 0; subId < i_total; subId++) {
    float *p_data = outputs[0].data() + subId * i_stride;
    auto type_res = personPose::heatmap_to_coord(p_data, upBBoxes_[startId + subId]);

    /*for(auto t : type_res) {
      std::cout<<t.first<<t.second<<std::endl;
    }*/

    PersonPosePoints kps;
    std::for_each(type_res.begin(), type_res.end(), [&kps](std::pair<cv::Point, float> &item) {
      std::pair<ArcternPoint, float> result;
      result.first.x = item.first.x;
      result.first.y = item.first.y;
      result.second = item.second;
      kps.push_back(result);
    });

    //std::cout << "kps = " << kps.size() << std::endl;

    p_res->results_[startId + subId] = kps;
  }

  return ErrorType::ERR_SUCCESS;
}

ArcternRect TrtPersonPoseImpl::LocalGetROI(const RunParameter *p, int idx) {
  const auto pRunParam = dynamic_cast<const PersonPoseRunParameter *>(p);
  auto &rect = pRunParam->rectsV_[idx];
  cv::Rect r(rect.x, rect.y, rect.width, rect.height);
  r = arctern::personPose::upscaleBbox(r, bboxExtScalTop_);
  auto roi= check_shrink_box(r,pRunParam->imageV_[idx].width,
                               pRunParam->imageV_[idx].height);

  upBBoxes_.push_back(roi);

  ArcternRect result;
  result.x = roi.x;
  result.y = roi.y;
  result.width = roi.width;
  result.height = roi.height;
  return result;
}

void TrtPersonPoseImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) {
  runParm->sequence_.push_back(OperatorType::crop);
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}

}

#include <utility>
#include <vector>
#include "licenseLandmarkImpl.h"
#include "common/file_buffer.h"
#include "common/imageConvert.h"
#include "common/algUtil.h"
#include "log/logger.h"

using arctern::LicenseLandmarkImpl;
using arctern::ErrorType;
using arctern::Result;

LicenseLandmarkImpl::LicenseLandmarkImpl() {
  input_h_ = 128;
  input_w_ = 256;
  dstImageFormatForForward_ = arctern::ArcternImageFormat::ARCTERN_IMAGE_FORMAT_RGB888;
}

LicenseLandmarkImpl::~LicenseLandmarkImpl() noexcept {
  LicenseLandmarkImpl::Release();
}

ErrorType LicenseLandmarkImpl::CheckRunParam(const RunParameter *p) {
  return ArcternAlgorithm::CheckRunParam(p);
}
void LicenseLandmarkImpl::InitPreprocessorParam() {
  preProcessor_.reset(new OpencvPreprocessor);
  OpencvPreprocessorInitPt init_pt;
  init_pt.net_h_ = input_h_;
  init_pt.net_w_ = input_w_;
  init_pt.meanV_ = {0.485, 0.456, 0.406};
  init_pt.stdV_ = {0.229, 0.224, 0.225};
  init_pt.border_value = 0;
  auto code = preProcessor_->Init(&init_pt);
  assert(code == ErrorType::ERR_SUCCESS);
}
ErrorType LicenseLandmarkImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
                                        std::vector<arctern::Tensor<float> > &outputs) {
  auto r = dynamic_cast<LicenseLandmarkResult *>(result_);
  int total = endId - startId;
  int stride = outputs[0].size() / outputs[0].shape()[0];
  for (int subId = 0; subId < total; subId++) {
    auto landmark = outputs[0].data() + subId * stride;
    float scale_x = float(p->imageV_[startId+subId].width) / float(input_w_);
    float scale_y = float(p->imageV_[startId+subId].height) / float(input_h_);
    cout << __func__ << ": " << __LINE__ << ": " << p->imageV_[startId+subId].width << ", and input_w_ = " << input_w_ << endl;
    cout << __func__ << ": " << __LINE__ << ": " << p->imageV_[startId+subId].height << ", and input_w_ = " << input_h_ << endl;
    std::vector<int> result;
    for(int i=0; i < 8; i=i+2)
    {
      arctern::ArcternPoint point;
      point.x = int(landmark[i] * scale_x + 0.5);
      point.y = int(landmark[i+1] * scale_y + 0.5);
      r->LicenseLandmarks_[startId + subId].emplace_back(point);
      cout << __func__ << ": " << __LINE__ << ": " << point.x << ", " << point.y << ")." << endl;
    }
  }
  return ErrorType::ERR_SUCCESS;
}

void LicenseLandmarkImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::resized);
  runParm->sequence_.push_back(OperatorType::splitNormalized);
}

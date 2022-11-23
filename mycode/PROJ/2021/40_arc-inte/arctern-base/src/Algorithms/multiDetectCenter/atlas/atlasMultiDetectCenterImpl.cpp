//
// Created by lzm on 2020/12/17.
//

#include "atlasMultiDetectCenterImpl.h"
#include "src/common/atlas/atlasAclDataType.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "Engin/src/atlas/atlasengin.h"
#include "common/algUtil.h"
#include "common/detectUtil.h"
namespace arctern{

AtlasMultiDetectCenterImpl::AtlasMultiDetectCenterImpl(){

}

AtlasMultiDetectCenterImpl::~AtlasMultiDetectCenterImpl() {
  
}

ErrorType AtlasMultiDetectCenterImpl::GetModelsConfigInfo(const TrtModels* models){
   auto model = models->models()->Get(0);
  bool return_res = try_get_value(*model, "INPUT_DIM_W", input_dim_w);
  return_res &= try_get_value(*model, "INPUT_DIM_H", input_dim_h);
  return_res &= try_get_value(*model, "OUTPUT_DIM_W", output_dim_w);
  return_res &= try_get_value(*model, "OUTPUT_DIM_H", output_dim_h);
  if (m_initParam.thresh_ <= 0) {
    return_res &= try_get_value(*model, "CONF_THRESH", m_initParam.thresh_);
  }
  if (!try_get_value(*model, "FLAG_QUALITY", flag_quality)) {
    flag_quality = false;
  }
  if (!try_get_value(*model, "FLAG_IND_WH", flag_ind_wh)) {
    flag_ind_wh = false;
  }

  if (!try_get_value(*model, "CLS_PROJECT", vec_cls_project)){
    vec_cls_project = std::vector<int>{0, 0, 1, 1, 2, 4, 3, 2, 4, 3}; /// default value, solve the compatibility of old model
  }
  if(flag_quality) {
    //cls project must be a multiple of 2;
    assert(!(vec_cls_project.size() & 1));
    for (size_t i = 0; i < vec_cls_project.size(); i+=2){
      cls_project[vec_cls_project[i]] = vec_cls_project[i+1];
    }
  }
  m_modelsInputWidth = input_dim_w;
  m_modelsInputHeight = input_dim_h;
  hm_ratio_h_ = input_dim_h * 1.0 / output_dim_h;
  hm_ratio_w_ = input_dim_w * 1.0 / output_dim_w;

  return ErrorType::ERR_SUCCESS;
}

void AtlasMultiDetectCenterImpl::initPostProcessParam(int img_width, int img_height, const cv::Size& osize,
                                     std::vector<int> &pad_w, std::vector<int> &pad_h, std::vector<float> &img_ratio) {
  const int w = osize.width;
  const int h = osize.height;
  int new_w = img_width;
  int new_h = img_height;

  if ((static_cast<float>(w) / img_width) <
      (static_cast<float>(h) / img_height)) {
    new_w = w;
    new_h = (img_height * w) / img_width;
  } else {
    new_h = h;
    new_w = (img_width * h) / img_height;
  }

  const int offset_w = (w - new_w) >> 1;
  const int offset_h = (h - new_h) >> 1;
  pad_h.push_back(offset_h);
  pad_w.push_back(offset_w);
  img_ratio.push_back(float(img_width) / float(new_w));
}



ErrorType AtlasMultiDetectCenterImpl::LocalPostProcess(const RunParameter *p, int startId, int endId,
std::vector<std::vector<float>> &outputs) {
  
  auto r = dynamic_cast<MultiDetectCenterResult *>(m_pResult);

  std::vector<int> pad_w;
  std::vector<int> pad_h;
  std::vector<float> img_ratio;
  int total = endId - startId;
  for (int subId = 0; subId < total; subId++) {
  
    initPostProcessParam(p->imageV_[startId + subId].width,
                         p->imageV_[startId + subId].height,
                         cv::Size(m_modelsInputWidth, m_modelsInputHeight),
                         pad_w, pad_h, img_ratio);
  }


  std::vector<Vbbox> results(total);
  std::vector<std::pair<float *, int>> vOutputs;
  for(int i = 0; i < outputs.size(); ++i){
    vOutputs.push_back(std::make_pair(outputs[i].data(), outputs[i].size()/total));
  }

  float* qm = nullptr;
  int stride_qm;
  int offset_qm;
  float* hm = vOutputs[0].first;
  float* hm_max = vOutputs[1].first;
  float* wh = vOutputs[2].first;
  float* offset = vOutputs[3].first;
  if(flag_quality){
    qm       = vOutputs[4].first;
    stride_qm = vOutputs[4].second;
    offset_qm = 0;
  }
  int stride_ind = vOutputs[0].second;
  int offset_ind = 0;
  int stride_wh  = vOutputs[3].second;
  int offset_wh  = 0;

  float score;
  float cur_w;
  float cur_h;
  int   ind;
  int   cls;
  int   x;
  int   y;
  for(int subId = 0; subId < total; subId++) {
    Vbbox result;
    for (int i = 0; i < stride_ind; i++) {
      cls = i / (output_dim_w * output_dim_h);
      if (hm[i + offset_ind] > m_initParam.thresh_ && hm[i + offset_ind] == hm_max[i + offset_ind]) {
        cv::Mat tl;
        cv::Mat br;

        score = hm[i + offset_ind];
        ind = i % (output_dim_w * output_dim_h);
        x = ind % output_dim_w;
        y = ind / output_dim_w; /// notice: y = ind / output_dim_w for 512x288 model, y = ind / output_dim_h for 512x512
        cur_w = wh[ind + offset_wh];
        cur_h = wh[ind + offset_wh + output_dim_w * output_dim_h];

        float x_rec = float(x) + offset[ind + offset_wh];
        float y_rec = float(y) + offset[ind + offset_wh + output_dim_w * output_dim_h];
        float x1 = x_rec - cur_w / 2.0f;
        float y1 = y_rec - cur_h / 2.0f;
        float x2 = x_rec + cur_w / 2.0f;
        float y2 = y_rec + cur_h / 2.0f;

        tl = cv::Mat(cv::Point3f(x1, y1, 1.0));
        br = cv::Mat(cv::Point3f(x2, y2, 1.0));

        int tl_x = tl.at<float>(0, 0) * hm_ratio_w_ < 0 ? 0 : int(
                (tl.at<float>(0, 0) * hm_ratio_w_) * img_ratio[subId]);
        int tl_y = tl.at<float>(0, 1) * hm_ratio_h_ < 0 ? 0 : int(
                (tl.at<float>(0, 1) * hm_ratio_h_) * img_ratio[subId]);
        int br_x = br.at<float>(0, 0) * hm_ratio_w_ > p->imageV_[startId + subId].width - 1 ? p->imageV_[startId + subId].width - 1 : int(
                (br.at<float>(0, 0) * hm_ratio_w_) * img_ratio[subId]);
        int br_y = br.at<float>(0, 1) * hm_ratio_h_ > p->imageV_[startId + subId].height - 1 ? p->imageV_[startId + subId].height - 1 : int(
                (br.at<float>(0, 1) * hm_ratio_h_) * img_ratio[subId]);

        Bbox b(tl_x, tl_y, br_x, br_y, float(score));
        if (flag_quality) {
          int quality_ind = int(cls) * output_dim_w * output_dim_h +
                            int(ind) + offset_qm;
          float quality = qm[quality_ind];
          b.quality = quality < 0 ? 0 : quality;
          b.label = cls_project[cls];
        } else {
          b.label = cls;
          b.quality = -1.0;
        }
        result.push_back(b);
      }
    }

    for (auto &box : result) {
      MULTI_DETECT_CENTER_INFO info;
      info.rect.x = box.x1;
      info.rect.y = box.y1;
      info.rect.width  = box.x2 - box.x1;
      info.rect.height = box.y2 - box.y1;
      info.score = box.score;
      info.quality = box.quality;
      info.type = static_cast<MultiCatType>(box.label);
      r->detInfos_[startId + subId].push_back(info);
    }

    offset_wh += stride_wh;
    offset_ind += stride_ind;
    if( flag_quality ){
      offset_qm += stride_qm;
    }
  }
  return ErrorType::ERR_SUCCESS;

}



ErrorType AtlasMultiDetectCenterImpl::LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
std::vector<DvppDataInfo>& vOutputInfos,
std::vector<cv::Rect>& vRects) {
  
  auto ret = m_pPreprocessor->CombineResizeBatchProcess(vInputInfo, vOutputInfos, true, VpcProcessType::VPC_PT_PADDING);

  if (ret != APP_ERR_OK) {
    LOGASDError << "failed to do altas preprocess";
    m_lastErrorCode = ErrorType::ERR_PRE_PROCESS;
  }
  return m_lastErrorCode;
}


}

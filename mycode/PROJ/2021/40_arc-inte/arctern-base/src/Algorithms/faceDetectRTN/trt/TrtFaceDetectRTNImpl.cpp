//
// Created by Admin on 2020/6/4.
//

#include "TrtFaceDetectRTNImpl.h"

namespace arctern {
TrtFaceDetectRTNImpl::TrtFaceDetectRTNImpl() {
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
  m_confThresh = m_initParam.thresh_;
}

TrtFaceDetectRTNImpl::~TrtFaceDetectRTNImpl() {

}

void TrtFaceDetectRTNImpl::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::detFace);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}


void TrtFaceDetectRTNImpl::InitPreprocessorParam() {
  m_pStream = m_vEngines[0]->getContext()->getCudaStream();
  m_pPreprocessor.reset(new NppPreprocessor(m_pStream));
  NppPreprocessorInitPt initPt;
  initPt.netH_ = m_modelsInputHeight;
  initPt.netW_ = m_modelsInputWidth;
  initPt.mean_ = 127.5;
  initPt.std_ = 128.0;
  initPt.borderValue_ = 0;
  initPt.alignMent_ = AlignMent::AlignLeft;
  assert(m_pPreprocessor);
  m_lastErrorCode = m_pPreprocessor->Init(&initPt);
  assert(m_lastErrorCode == ErrorType::ERR_SUCCESS);
}

ErrorType TrtFaceDetectRTNImpl::GetModelsConfigInfo(const MxnetModels *models) {
  ErrorType vResult;
  auto model = models->models()->Get(0);

  bool ret = try_get_value(*model, "INPUT_H", m_modelsInputHeight);
  assert(ret == true);

  ret= try_get_value(*model, "INPUT_W", m_modelsInputWidth);
  assert(ret == true);

  ret= try_get_value(*model, "USING_LANDMARK", m_bUsingLandmark);
  assert(ret == true);

  if (m_confThresh <= 0) {
    ret = try_get_value(*model, "CONF_THRESH", m_confThresh);
    assert(ret == true);
  }

  bool quality_flag = try_get_value(*model, "USING_QUALITY", m_bUsingQuality);
  if (!quality_flag) {
    m_bUsingQuality = false;
  }

  m_numOutput = model->num_output();

  m_vFeatureMaps.clear();
  for (auto step : m_vSteps) {
    int h = std::ceil(m_modelsInputHeight / step);
    int w = std::ceil(m_modelsInputWidth / step);
    m_vFeatureMaps.push_back({h, w});
  }
  m_vAnchors = GenerateBox();

  return vResult;

}

vector<arctern::prior_box> TrtFaceDetectRTNImpl::GenerateBox() {
  std::vector<prior_box> anchors;
  prior_box temp_anchor;
  for (size_t k = 0; k < m_vFeatureMaps.size(); k++) {
    std::vector<float> min_size = m_vminSizes[k];
    int f0 = m_vFeatureMaps[k][0];
    int f1 = m_vFeatureMaps[k][1];
    for (int i = 0; i < f0; i++) {
      for (int j = 0; j < f1; j++) {
        for (auto m : min_size) {
          temp_anchor.s_kx = m / m_modelsInputWidth;
          temp_anchor.s_ky = m / m_modelsInputHeight;
          temp_anchor.cx = (j + 0.5) * m_vSteps[k] / m_modelsInputWidth;
          temp_anchor.cy = (i + 0.5) * m_vSteps[k] / m_modelsInputHeight;
          anchors.push_back(temp_anchor);
        }
      }
    }
  }
  return anchors;
}

void TrtFaceDetectRTNImpl::InitPostProcessParam(int img_width,
                                                int img_height,
                                                std::vector<float> &v_scale_h,
                                                std::vector<float> &v_scale_w) {
  if (m_modelsInputHeight == m_modelsInputWidth) {
    auto padSize = img_width > img_height ? img_width : img_height;
    v_scale_h.push_back(static_cast<float>(padSize) /
        static_cast<float>(m_modelsInputHeight));
    v_scale_w.push_back(static_cast<float>(padSize) /
        static_cast<float>(m_modelsInputWidth));
  } else {
    v_scale_h.push_back(static_cast<float>(img_height) /
        static_cast<float>(m_modelsInputHeight));
    v_scale_w.push_back(static_cast<float>(img_width) /
        static_cast<float>(m_modelsInputWidth));
  }
}

arctern::Vbbox TrtFaceDetectRTNImpl::DoNmsSort(Vbbox &keep_bbox, float thresh) {
  arctern::Vbbox result;
  std::sort(keep_bbox.begin(), keep_bbox.end(), [](Bbox pa, Bbox pb) {
    return pa.score > pb.score;
  });
  for (auto i = keep_bbox.begin(); i != keep_bbox.end(); i++) {
    if (i->score <= 0.01) {
      continue;
    }
    for (auto j = i + 1; j != keep_bbox.end(); j++) {
      if (j->score <= 0.01) {
        continue;
      }
      ArcternRect a{i->x1, i->y1, i->x2 - i->x1, i->y2 - i->y1};
      ArcternRect b{j->x1, j->y1, j->x2 - j->x1, j->y2 - j->y1};
      float iou = box_iou(a, b);
      if (iou > thresh) {
        j->score = 0;
      }
    }
    result.emplace_back(*i);
  }
  return result;
}

arctern::Vbbox
TrtFaceDetectRTNImpl::DecodeBoxLandmark(float *locations,
                                       float *scores, float *landmarks, float *quality,
                                       float scale_h, float scale_w, int stride) {
  Vbbox boxes;
  for (int i = 0; i < stride; i++) {
    float current_score = scores[2 * i + 1];
    if (current_score < m_confThresh) {
      continue;
    }
    float center_x = m_vAnchors[i].cx + locations[4 * i] * m_vVariance[0] * m_vAnchors[i].s_kx;
    float center_y = m_vAnchors[i].cy + locations[4 * i + 1] * m_vVariance[0] * m_vAnchors[i].s_ky;
    float w = m_vAnchors[i].s_kx * exp(locations[4 * i + 2] * m_vVariance[1]);
    float h = m_vAnchors[i].s_ky * exp(locations[4 * i + 3] * m_vVariance[1]);

    float xmin = center_x - w / 2;
    float ymin = center_y - h / 2;
    float xmax = xmin + w;
    float ymax = ymin + h;

    float x1 = xmin * m_modelsInputWidth * scale_w;
    float y1 = ymin * m_modelsInputHeight * scale_h;
    float x2 = xmax * m_modelsInputWidth * scale_w;
    float y2 = ymax * m_modelsInputHeight * scale_h;
    float score_t = current_score;
    Bbox box(x1, y1, x2, y2, score_t);
    if (m_bUsingLandmark) {
      std::vector<cv::Point2f> temp_points;
      for (int j = 0; j < m_numLandmark; j++) {
        cv::Point2f temp_point;
        temp_point.x = (m_vAnchors[i].cx + landmarks[10 * i + 2 * j] * m_vVariance[0] * m_vAnchors[i].s_kx) *
            m_modelsInputWidth * scale_w;
        temp_point.y = (m_vAnchors[i].cy + landmarks[10 * i + 2 * j + 1] * m_vVariance[0] * m_vAnchors[i].s_ky) *
            m_modelsInputWidth * scale_h;
        temp_points.push_back(temp_point);
      }
      box.lmd.leye = temp_points[0];
      box.lmd.reye = temp_points[1];
      box.lmd.nose = temp_points[2];
      box.lmd.lmouth = temp_points[3];
      box.lmd.rmouth = temp_points[4];
    }

    if (m_bUsingQuality) {
      box.quality = quality[i];
    }
    box.label = 0;
    boxes.push_back(box);
  }
  if (m_bSquareFace) {
    ConvertSquare(boxes, m_extHeightScaleFace, m_extWidthScaleFace);
  }
  return boxes;
}

ErrorType TrtFaceDetectRTNImpl::LocalPostProcess(const RunParameter *p,
                                                 int startId,
                                                 int endId,
                                                 vector<arctern::OutputTensor<float>> &outputs) {
  assert(p != nullptr);
  auto r = dynamic_cast<FaceDetectRTNResult *>(m_pResult);
  std::vector<float> v_scale_h, v_scale_w;
  const int batchNum = outputs[0].shape()[0];//m_initParam.batchNum_;
  int total = endId - startId;
  int stride = outputs[0].size() / 4 / batchNum;
  for (int subId = 0; subId < total; subId++) {
    InitPostProcessParam(p->imageV_[startId + subId].width,
                         p->imageV_[startId + subId].height,
                         v_scale_h, v_scale_w);
    float *locations = outputs[0].data() + outputs[0].size() /
        batchNum * subId;
    float *scores = outputs[1].data() + outputs[1].size() /
        batchNum * subId;
    float *landmarks = outputs[2].data() + outputs[2].size() /
        batchNum * subId;
    float *quality = nullptr;
    if (m_bUsingQuality) {
      quality = outputs[3].data() + stride * subId;
    }

    auto vbox = DecodeBoxLandmark(locations, scores,
                                  landmarks, quality, v_scale_h[subId],
                                  v_scale_w[subId], stride);
    Vbbox result = DoNmsSort(vbox, m_nmsThresh);
    for (auto &box : result) {
      FACE_DETECT_RTN_INFO info;
      info.rect.x = box.x1;
      info.rect.y = box.y1;
      info.rect.width = box.x2 - box.x1;
      info.rect.height = box.y2 - box.y1;
      info.quality = box.quality;
      info.score = box.score;
      info.landmarks[0].x = box.lmd.leye.x;
      info.landmarks[0].y = box.lmd.leye.y;
      info.landmarks[1].x = box.lmd.reye.x;
      info.landmarks[1].y = box.lmd.reye.y;
      info.landmarks[2].x = box.lmd.nose.x;
      info.landmarks[2].y = box.lmd.nose.y;
      info.landmarks[3].x = box.lmd.lmouth.x;
      info.landmarks[3].y = box.lmd.lmouth.y;
      info.landmarks[4].x = box.lmd.rmouth.x;
      info.landmarks[4].y = box.lmd.rmouth.y;
      r->detInfos_[startId + subId].push_back(info);
    }
  }
  return ErrorType::ERR_SUCCESS;

}

}
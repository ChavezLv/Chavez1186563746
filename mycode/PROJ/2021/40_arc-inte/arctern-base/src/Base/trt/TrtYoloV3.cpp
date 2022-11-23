//
// Created by Admin on 2019/6/11.
//


#include "TrtYoloV3.h"
namespace arctern {
TrtYoloV3::TrtYoloV3(){
  m_dstImageFormatForForward = ARCTERN_IMAGE_FORMAT_RGB888;
  m_configThresh = m_initParam.thresh_;
}

TrtYoloV3::~TrtYoloV3(){

}


ErrorType TrtYoloV3::GetModelsConfigInfo(const MxnetModels *models) {

  auto model = models->models()->Get(0);

  bool ret = try_get_value(*model, "CLASS_NUM", m_classNum);
  assert(ret == true);

  ret = try_get_value(*model, "INPUT_DIM", m_inputDim);
  assert(ret == true);

  m_inputDim2 = 0;
  try_get_value(*model, "INPUT_DIM2", m_inputDim2);
  assert(ret == true);

  ret = try_get_value(*model, "NMS_THRESH", m_nmsThresh);
  assert(ret == true);

  if (m_configThresh <= 0) {
    ret = try_get_value(*model, "CONF_THRESH", m_configThresh);
    assert(ret == true);
  }

  ret = try_get_value(*model, "DET_PERSON_FLAG", m_bDetPersonFlag);
  assert(ret == true);
  ret = try_get_value(*model, "TINY_YOLO_FLAG", m_bTinyYolov3Flag);
  assert(ret == true);

  std::vector<int> anchors;
  ret = try_get_value(*model, "ANCHORS", anchors);
  assert(ret == true);
  for (size_t i = 0; i < anchors.size(); i += 2) {
    m_vAnchors.push_back({anchors[i], anchors[i + 1]});
  }

  m_bSquareFace = true;
  m_strides = m_bTinyYolov3Flag ? 2 : 3;
  m_boxs = m_vAnchors.size() / m_strides;
  m_vStep.resize(m_strides);
  m_inputDim2 = (0 == m_inputDim2) ? m_inputDim : m_inputDim2;

  int pre_step = 0;
  int stride1, stride2;
  for (int i = 0; i < m_strides; ++i) {
    stride1 = m_inputDim * static_cast<int>(pow(2, i)) / 32;
    stride2 = m_inputDim2 * static_cast<int>(pow(2, i)) / 32;
    m_vStrides.push_back({stride1, stride2});
    m_vStep[i].push_back(pre_step);
    pre_step += stride1 * stride2 * m_boxs;
    m_vStep[i].push_back(pre_step);
  }

  m_modelsInputWidth = m_inputDim;
  m_modelsInputHeight = m_inputDim2;

  return ERR_SUCCESS;
}

void TrtYoloV3::InitPreprocessorParam(){
  m_pStream = m_vEngines[0]->getContext()->getCudaStream();
  m_pPreprocessor.reset(new NppPreprocessor(m_pStream));
  NppPreprocessorInitPt initPt;
  initPt.netH_ = m_modelsInputHeight;
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

void TrtYoloV3::InitPreprocessorRunParam(NppPreprocessorRunPt *runParm){
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::detFace);
  runParm->sequence_.push_back(OperatorType::normalized);
  runParm->sequence_.push_back(OperatorType::extractChannel);
}

ErrorType TrtYoloV3::LocalPostProcess(const RunParameter *p, int startId,
                                      int endId, vector<arctern::OutputTensor<float>> &outputs) {

}

void TrtYoloV3::TransformGrid(float *prediction) {
  std::vector<std::vector<int>> anchors_masks;
  const std::vector<std::vector<int>> ANCHORS_MASKS6 = {{3, 4, 5},
                                                        {0, 1, 2}};

  const std::vector<std::vector<int>> ANCHORS_MASKS8 = {{4, 5, 6, 7},
                                                        {0, 1, 2, 3}};

  const std::vector<std::vector<int>> ANCHORS_MASKS9 = {{6, 7, 8},
                                                        {3, 4, 5},
                                                        {0, 1, 2}};

  const std::vector<std::vector<int>> ANCHORS_MASKS12 = {{8, 9, 10, 11},
                                                         {4, 5, 6, 7},
                                                         {0, 1, 2, 3}};

  if (m_vAnchors.size() == 6) {
    anchors_masks = ANCHORS_MASKS6;
  } else if (m_vAnchors.size() == 8) {
    anchors_masks = ANCHORS_MASKS8;
  } else if (m_vAnchors.size() == 9) {
    anchors_masks = ANCHORS_MASKS9;
  } else if (m_vAnchors.size() == 12) {
    anchors_masks = ANCHORS_MASKS12;
  }

  int len = 5 + m_classNum;
  if (m_bDetPersonFlag) {
    len += 2;
  }

  for (int i = 0; i < m_strides; ++i) {
    auto stride = m_vStrides[i];
    float input_dim_div_stride = static_cast<float>(m_inputDim) / stride[0];
    int to_offset = m_vStep[i][0] * len;
    int tmp_idx_1 = 0;
    for (int x_offset = 0; x_offset < stride[1]; ++x_offset) {
      int tmp_idx_2 = 0;
      for (int y_offset = 0; y_offset < stride[0]; ++y_offset) {
        int tmp_idx_3 = 0;
        for (int kk = 0; kk < m_boxs; ++kk) {
          int tmp_idx = to_offset + tmp_idx_1 + tmp_idx_2 + tmp_idx_3;
          if (prediction[tmp_idx + 4] >= m_configThresh) {
            prediction[tmp_idx + 0] += y_offset;
            prediction[tmp_idx + 0] *= input_dim_div_stride;
            prediction[tmp_idx + 1] += x_offset;
            prediction[tmp_idx + 1] *= input_dim_div_stride;
            prediction[tmp_idx + 2] = (std::exp(prediction[tmp_idx + 2]) *
                m_vAnchors[anchors_masks[i][kk]][0]);
            prediction[tmp_idx + 3] = (std::exp(prediction[tmp_idx + 3]) *
                m_vAnchors[anchors_masks[i][kk]][1]);
          }
          tmp_idx_3 += len;
        }
        tmp_idx_2 += (m_boxs * len);
      }
      tmp_idx_1 += (stride[0] * m_boxs * len);
    }
  }
}

vector<detection> TrtYoloV3::PackValidDetections(float *prediction) {
  std::vector<int> valid_detections;
  int len = 5 + m_classNum;
  if (m_bDetPersonFlag) {
    len += 2;
  }
  ///> find out the number of valid detections
  for (int i = 0; i < m_vStep[m_strides - 1][1]; ++i) {
    float score = prediction[i * len + 4];
    if (score > m_configThresh) {
      valid_detections.push_back(i);
    }
  }

  float top_occluded_prob = 0.0f;
  float bottom_occluded_prob = 0.0f;

  // convert to box_corner
  std::vector<detection> detections;

  for (size_t i = 0; i < valid_detections.size(); ++i) {
    detection det;
    int iPrediction = valid_detections[i];
    if (m_bDetPersonFlag) {
      top_occluded_prob = prediction[iPrediction * len + 5 + m_classNum + 0];
      bottom_occluded_prob =
          prediction[iPrediction * len + 5 + m_classNum + 1];
    }
    float score = prediction[iPrediction * len + 4];
    float x = prediction[iPrediction * len + 0];
    float y = prediction[iPrediction * len + 1];
    float w = prediction[iPrediction * len + 2];
    float h = prediction[iPrediction * len + 3];

    det.bbox.x = x;
    det.bbox.y = y;
    det.bbox.width = w;
    det.bbox.height = h;
    det.objectness = score;
    det.prob = prediction + iPrediction * len + 5;
    if (m_bDetPersonFlag) {
      det.top_occluded_prob = top_occluded_prob;
      det.bottom_occluded_prob = bottom_occluded_prob;
    }

    for (int j = 0; j < m_classNum; ++j) {
      float prob = score * det.prob[j];
      det.prob[j] = (prob > m_configThresh) ? prob : 0;
    }
    detections.push_back(det);
  }
  return detections;
}

void TrtYoloV3::DoNmsSort(detection *dets, int total) {
  int i, j, k;
  k = total - 1;
  for (i = 0; i <= k; ++i) {
    if (dets[i].objectness == 0) {
      detection swap = dets[i];
      dets[i] = dets[k];
      dets[k] = swap;
      --k;
      --i;
    }
  }
  total = k + 1;

  for (k = 0; k < m_classNum; ++k) {
    for (i = 0; i < total; ++i) {
      dets[i].sort_class = k;
    }
    qsort(dets, total, sizeof(detection), NmsComparator);
    for (i = 0; i < total; ++i) {
      if (dets[i].prob[k] == 0) continue;
      ArcternRect a = dets[i].bbox;
      for (j = i + 1; j < total; ++j) {
        ArcternRect b = dets[j].bbox;
        if (box_iou(a, b) > m_nmsThresh) {
          dets[j].prob[k] = 0;
        }
      }
    }
  }
}

int TrtYoloV3::NmsComparator(const void *pa, const void *pb) {
  detection a = *reinterpret_cast<const detection *>(pa);
  detection b = *reinterpret_cast<const detection *>(pb);
  float diff = 0;
  if (b.sort_class >= 0) {
    diff = a.prob[b.sort_class] - b.prob[b.sort_class];
  } else {
    diff = a.objectness - b.objectness;
  }
  if (diff < 0)
    return 1;
  else if (diff > 0)
    return -1;
  return 0;
}

Vbbox TrtYoloV3::PackResults(int oriWidth, int oriHeight, const std::vector<detection> &detections) const {
  Vbbox boxes;
  int tmp_dim;
  float scaling_factor1, scaling_factor2;
  int len = 5 + m_classNum;
  if (m_bDetPersonFlag) {
    len += 2;
  }

  if (m_inputDim == m_inputDim2) {
    tmp_dim = m_inputDim;
    int max_dim = std::max(oriHeight, oriWidth);
    scaling_factor1 = m_inputDim * 1.0f / max_dim;
    scaling_factor2 = scaling_factor1;
  } else {
    tmp_dim = m_inputDim2;
    scaling_factor1 = m_inputDim * 1.0f / oriWidth;
    scaling_factor2 = tmp_dim * 1.0f / oriHeight;
  }
  float top_occluded_prob = 0.0f;
  float bottom_occluded_prob = 0.0f;

  for (size_t i = 0; i < detections.size(); ++i) {
    float x = detections[i].bbox.x;
    float y = detections[i].bbox.y;
    float w = detections[i].bbox.width;
    float h = detections[i].bbox.height;
    if (m_bDetPersonFlag) {
      top_occluded_prob = detections[i].top_occluded_prob;
      bottom_occluded_prob = detections[i].bottom_occluded_prob;
    }

    float x1 = x - w / 2;
    float y1 = y - h / 2;
    float x2 = x + w / 2;
    float y2 = y + h / 2;

    x1 -= (m_inputDim - scaling_factor1 * oriWidth) / 2.0f;
    x2 -= (m_inputDim - scaling_factor1 * oriWidth) / 2.0f;
    y1 -= (tmp_dim - scaling_factor2 * oriHeight) / 2.0f;
    y2 -= (tmp_dim - scaling_factor2 * oriHeight) / 2.0f;
    x1 /= scaling_factor1;
    x2 /= scaling_factor1;
    y1 /= scaling_factor2;
    y2 /= scaling_factor2;

    x1 = std::min(oriWidth, std::max(static_cast<int>(x1), 0));
    x2 = std::min(oriWidth, std::max(static_cast<int>(x2), 0));
    y1 = std::min(oriHeight, std::max(static_cast<int>(y1), 0));
    y2 = std::min(oriHeight, std::max(static_cast<int>(y2), 0));

    float score = 0.0f;
    for (int j = 0; j < m_classNum; ++j) {
      if (detections[i].prob[j] > 0) {
        score = detections[i].prob[j];
        Bbox b(x1, y1, x2, y2, score);
        b.label = j;
        if (m_bDetPersonFlag) {
          b.dy1 = top_occluded_prob;
          b.dy2 = bottom_occluded_prob;
        }
        boxes.push_back(b);
      }
    }
  }
  return boxes;
}

}
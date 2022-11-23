/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.14
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include "YoloV3.h"
#include "common/imageConvert.h"
#include <algorithm>
#include <vector>


namespace arctern {

YoloV3::YoloV3()
    :  num_classes_{}, input_dim_{},
      input_dim2_(0), nstrides_{},
      config_thresh(0.6f), nboxes_{}, nms_thresh_{}, det_person_flag_{},
      tiny_yolov3_flag_{}, anchors_{}, step_{}, strides_{} {}

YoloV3::~YoloV3() {  }

int YoloV3::getConfigInfo(arctern::MxnetModel *model) {
  std::vector<int> anchors;
  input_dim2_ = 0;

  bool return_res = try_get_value(*model, "CLASS_NUM", num_classes_);
  return_res &= try_get_value(*model, "INPUT_DIM", input_dim_);
  return_res &= try_get_value(*model, "INPUT_DIM2", input_dim2_);
  return_res &= try_get_value(*model, "NMS_THRESH", nms_thresh_);

  if (config_thresh <= 0) {
    return_res &= try_get_value(*model, "CONF_THRESH", config_thresh);
  }
  return_res &= try_get_value(*model, "DET_PERSON_FLAG", det_person_flag_);
  return_res &= try_get_value(*model, "TINY_YOLO_FLAG", tiny_yolov3_flag_);
  return_res &= try_get_value(*model, "ANCHORS", anchors);

  for (size_t i = 0; i < anchors.size(); i += 2) {
    anchors_.push_back({anchors[i], anchors[i + 1]});
  }

  initMemberVariable();
  return 0;
}
int YoloV3::getConfigInfo(arctern::TrtModel *model) {
  std::vector<int> anchors;
  input_dim2_ = 0;
  bool return_res = try_get_value(*model, "CLASS_NUM", num_classes_);
  return_res &= try_get_value(*model, "INPUT_DIM", input_dim_);
  return_res &= try_get_value(*model, "INPUT_DIM2", input_dim2_);
  return_res &= try_get_value(*model, "NMS_THRESH", nms_thresh_);

  if (config_thresh <= 0) {
    return_res &= try_get_value(*model, "CONF_THRESH", config_thresh);
  }

  return_res &= try_get_value(*model, "DET_PERSON_FLAG", det_person_flag_);
  return_res &= try_get_value(*model, "TINY_YOLO_FLAG", tiny_yolov3_flag_);
  return_res &= try_get_value(*model, "ANCHORS", anchors);

  for (size_t i = 0; i < anchors.size(); i += 2) {
    anchors_.push_back({anchors[i], anchors[i + 1]});
  }

  initMemberVariable();
  return 0;
}

int YoloV3::getNetH() {
  return input_dim2_;
}

int YoloV3::getNetW() {
  return input_dim_;
}


bool YoloV3::initMemberVariable() {
  nstrides_ = tiny_yolov3_flag_ ? 2 : 3;

  nboxes_ = anchors_.size() / nstrides_;

  step_.resize(nstrides_);
  int pre_step = 0;
  int stride1, stride2;

  input_dim2_ = (0 >= input_dim2_) ? input_dim_ : input_dim2_;
  auto tmp_dim = input_dim2_;
  for (int i = 0; i < nstrides_; ++i) {
    stride1 = input_dim_ * static_cast<int>(pow(2, i)) / 32;
    stride2 = tmp_dim * static_cast<int>(pow(2, i)) / 32;
    strides_.push_back({stride1, stride2});
    step_[i].push_back(pre_step);
    pre_step += stride1 * stride2 * nboxes_;
    step_[i].push_back(pre_step);
  }

  return true;
}

void YoloV3::pack_results(int oriWidth, int oriHeight,
                          const std::vector<detection> &detections,
                          Vbbox &boxes ) const{
  float scaling_factor1, scaling_factor2;
  int len = 5 + num_classes_;
  if (det_person_flag_) {len += 2;}
  if (input_dim_ == input_dim2_) {
    int max_dim = std::max(oriHeight, oriWidth);
    scaling_factor1 = input_dim_ * 1.0f / max_dim;
    scaling_factor2 = scaling_factor1;
  } else {
    scaling_factor1 = input_dim_ * 1.0f / oriWidth;
    scaling_factor2 = input_dim2_ * 1.0f / oriHeight;}
  float top_occluded_prob = 0.0f;
  float bottom_occluded_prob = 0.0f;
  for (size_t i = 0; i < detections.size(); ++i) {
    float x = detections[i].bbox.x;
    float y = detections[i].bbox.y;
    float w = detections[i].bbox.width;
    float h = detections[i].bbox.height;
    if (det_person_flag_) {
      top_occluded_prob = detections[i].top_occluded_prob;
      bottom_occluded_prob = detections[i].bottom_occluded_prob;
    }
    float x1 = x - w / 2;float y1 = y - h / 2;float x2 = x + w / 2;
    float y2 = y + h / 2;
    x1 -= (input_dim_ - scaling_factor1 * oriWidth) / 2.0f;
    x2 -= (input_dim_ - scaling_factor1 * oriWidth) / 2.0f;
    y1 -= (input_dim2_ - scaling_factor2 * oriHeight) / 2.0f;
    y2 -= (input_dim2_ - scaling_factor2 * oriHeight) / 2.0f;
    x1 /= scaling_factor1;x2 /= scaling_factor1;
    y1 /= scaling_factor2;y2 /= scaling_factor2;
    x1 = std::min(oriWidth, std::max(static_cast<int>(x1), 0));
    x2 = std::min(oriWidth, std::max(static_cast<int>(x2), 0));
    y1 = std::min(oriHeight, std::max(static_cast<int>(y1), 0));
    y2 = std::min(oriHeight, std::max(static_cast<int>(y2), 0));
    float score = 0.0f;
    for (int j = 0; j < num_classes_; ++j) {
      if (detections[i].prob[j] > 0) {
        score = detections[i].prob[j];
        Bbox b(x1, y1, x2, y2, score);
        b.label = j;
        if (det_person_flag_) {
          b.dy1 = top_occluded_prob;
          b.dy2 = bottom_occluded_prob;}
        boxes.push_back(b);}}}}

Vbbox YoloV3::pack_results2(int oriWidth, int oriHeight, const std::vector<detection> &detections) {
  Vbbox boxes;
  float scaling_factor1, scaling_factor2;
  int len = 5 + num_classes_;
  if (det_person_flag_) {
    len += 2;
  }

  if (input_dim_ == input_dim2_) {
    int max_dim = std::max(oriHeight, oriWidth);
    scaling_factor1 = input_dim_ * 1.0f / max_dim;
    scaling_factor2 = scaling_factor1;
  } else {
    scaling_factor1 = input_dim_ * 1.0f / oriWidth;
    scaling_factor2 = input_dim2_ * 1.0f / oriHeight;
  }
  float top_occluded_prob = 0.0f;
  float bottom_occluded_prob = 0.0f;

  for (size_t i = 0; i < detections.size(); ++i) {
    float x = detections[i].bbox.x;
    float y = detections[i].bbox.y;
    float w = detections[i].bbox.width;
    float h = detections[i].bbox.height;
    if (det_person_flag_) {
      top_occluded_prob = detections[i].top_occluded_prob;
      bottom_occluded_prob = detections[i].bottom_occluded_prob;
    }

    float x1 = x - w / 2;
    float y1 = y - h / 2;
    float x2 = x + w / 2;
    float y2 = y + h / 2;

    x1 /= scaling_factor1;
    x2 /= scaling_factor1;
    y1 /= scaling_factor2;
    y2 /= scaling_factor2;

    x1 = std::min(oriWidth, std::max(static_cast<int>(x1), 0));
    x2 = std::min(oriWidth, std::max(static_cast<int>(x2), 0));
    y1 = std::min(oriHeight, std::max(static_cast<int>(y1), 0));
    y2 = std::min(oriHeight, std::max(static_cast<int>(y2), 0));

    float score = 0.0f;
    for (int j = 0; j < num_classes_; ++j) {
      if (detections[i].prob[j] > 0) {
        score = detections[i].prob[j];
        Bbox b(x1, y1, x2, y2, score);
        b.label = j;
        if (det_person_flag_) {
          b.dy1 = top_occluded_prob;
          b.dy2 = bottom_occluded_prob;
        }
        boxes.push_back(b);
      }
    }
  }
  return boxes;
}

Vbbox YoloV3::pack_results(int oriWidth, int oriHeight,
                           const std::vector<detection> &detections) const {
  Vbbox boxes;

  float scaling_factor1, scaling_factor2;
  int len = 5 + num_classes_;
  if (det_person_flag_) {
    len += 2;
  }

  if (input_dim_ == input_dim2_) {

    int max_dim = std::max(oriHeight, oriWidth);
    scaling_factor1 = input_dim_ * 1.0f / max_dim;
    scaling_factor2 = scaling_factor1;
  } else {

    scaling_factor1 = input_dim_ * 1.0f / oriWidth;
    scaling_factor2 = input_dim2_ * 1.0f / oriHeight;
  }
  float top_occluded_prob = 0.0f;
  float bottom_occluded_prob = 0.0f;

  for (size_t i = 0; i < detections.size(); ++i) {
    float x = detections[i].bbox.x;
    float y = detections[i].bbox.y;
    float w = detections[i].bbox.width;
    float h = detections[i].bbox.height;

    if (det_person_flag_) {
      top_occluded_prob = detections[i].top_occluded_prob;
      bottom_occluded_prob = detections[i].bottom_occluded_prob;
    }

    float x1 = x - w / 2;
    float y1 = y - h / 2;
    float x2 = x + w / 2;
    float y2 = y + h / 2;

    x1 -= (input_dim_ - scaling_factor1 * oriWidth) / 2.0f;
    x2 -= (input_dim_ - scaling_factor1 * oriWidth) / 2.0f;
    y1 -= (input_dim2_ - scaling_factor2 * oriHeight) / 2.0f;
    y2 -= (input_dim2_ - scaling_factor2 * oriHeight) / 2.0f;
    x1 /= scaling_factor1;
    x2 /= scaling_factor1;
    y1 /= scaling_factor2;
    y2 /= scaling_factor2;

    x1 = std::min(oriWidth, std::max(static_cast<int>(x1), 0));
    x2 = std::min(oriWidth, std::max(static_cast<int>(x2), 0));
    y1 = std::min(oriHeight, std::max(static_cast<int>(y1), 0));
    y2 = std::min(oriHeight, std::max(static_cast<int>(y2), 0));

    float score = 0.0f;
    for (int j = 0; j < num_classes_; ++j) {
      if (detections[i].prob[j] > 0) {
        score = detections[i].prob[j];
        Bbox b(x1, y1, x2, y2, score);
        b.label = j;

        if (det_person_flag_) {
          b.dy1 = top_occluded_prob;
          b.dy2 = bottom_occluded_prob;
        }

        boxes.push_back(b);
      }
    }
  }

  return boxes;
}

Vbbox YoloV3::pack_results3(int oriWidth, int oriHeight,const std::vector<detection>& detections) {
  Vbbox boxes;
    float scaling_factor1,scaling_factor2;
    int len = 5 + num_classes_;
    if (det_person_flag_) { len += 2; }

    // no padding used in preprocess
    scaling_factor1 = input_dim_ * 1.0f / oriWidth;
    scaling_factor2 = input_dim2_ * 1.0f / oriHeight;

    float top_occluded_prob = 0.0f;
    float bottom_occluded_prob = 0.0f;

    for (int i = 0; i < detections.size(); ++i) {
      float x = detections[i].bbox.x;
      float y = detections[i].bbox.y;
      float w = detections[i].bbox.width;
      float h = detections[i].bbox.height;
      if (det_person_flag_){
        top_occluded_prob = detections[i].top_occluded_prob;
        bottom_occluded_prob = detections[i].bottom_occluded_prob;
      }

      float x1 = x - w / 2;
      float y1 = y - h / 2;
      float x2 = x + w / 2;
      float y2 = y + h / 2;
      x1 /= scaling_factor1;
      x2 /= scaling_factor1;
      y1 /= scaling_factor2;
      y2 /= scaling_factor2;

      x1 = std::min(oriWidth, std::max((int)x1, 0));
      x2 = std::min(oriWidth, std::max((int)x2, 0));
      y1 = std::min(oriHeight, std::max((int)y1, 0));
      y2 = std::min(oriHeight, std::max((int)y2, 0));

      float score = 0.0f;
      for (int j = 0; j < num_classes_; ++j)
      {
        if (detections[i].prob[j] > 0)
        {
          score = detections[i].prob[j];
          Bbox b(x1, y1, x2, y2, score);
          b.label = j;
          if (det_person_flag_){
            b.dy1 = top_occluded_prob;
            b.dy2 = bottom_occluded_prob;
          }
          boxes.push_back(b);
        }
      }
    }

    return boxes;
}

void YoloV3::transform_grid(float *prediction) {
  std::vector<std::vector<int>> anchors_masks;
  const std::vector<std::vector<int>> ANCHORS_MASKS6 = {{3, 4, 5}, {0, 1, 2}};
  const std::vector<std::vector<int>> ANCHORS_MASKS8 = {{4, 5, 6, 7},
                                                        {0, 1, 2, 3}};
  const std::vector<std::vector<int>> ANCHORS_MASKS9 = {
      {6, 7, 8}, {3, 4, 5}, {0, 1, 2}};
  const std::vector<std::vector<int>> ANCHORS_MASKS12 = {
      {8, 9, 10, 11}, {4, 5, 6, 7}, {0, 1, 2, 3}};
  if (anchors_.size() == 6) {
    anchors_masks = ANCHORS_MASKS6;
  } else if (anchors_.size() == 8) {
    anchors_masks = ANCHORS_MASKS8;
  } else if (anchors_.size() == 9) {
    anchors_masks = ANCHORS_MASKS9;
  } else if (anchors_.size() == 12) {
    anchors_masks = ANCHORS_MASKS12;
  }int len = 5 + num_classes_;
  if (det_person_flag_) {len += 2;}
  for (int i = 0; i < nstrides_; ++i) {
    auto stride = strides_[i];
    float input_dim_div_stride = static_cast<float>(input_dim_) / stride[0];
    int to_offset = step_[i][0] * len;
    int tmp_idx_1 = 0;
    for (int x_offset = 0; x_offset < stride[1]; ++x_offset) {
      int tmp_idx_2 = 0;
      for (int y_offset = 0; y_offset < stride[0]; ++y_offset) {
        int tmp_idx_3 = 0;
        for (int kk = 0; kk < nboxes_; ++kk) {
          int tmp_idx = to_offset + tmp_idx_1 + tmp_idx_2 + tmp_idx_3;
          if (prediction[tmp_idx + 4] >= config_thresh) {
            prediction[tmp_idx + 0] += y_offset;
            prediction[tmp_idx + 0] *= input_dim_div_stride;
            prediction[tmp_idx + 1] += x_offset;
            prediction[tmp_idx + 1] *= input_dim_div_stride;
            prediction[tmp_idx + 2] = (std::exp(prediction[tmp_idx + 2]) *
                anchors_[anchors_masks[i][kk]][0]);
            prediction[tmp_idx + 3] = (std::exp(prediction[tmp_idx + 3]) *
                anchors_[anchors_masks[i][kk]][1]);
          }
          tmp_idx_3 += len;}
        tmp_idx_2 += (nboxes_ * len);}
      tmp_idx_1 += (stride[0] * nboxes_ * len);
  }}}
void YoloV3::pack_valid_detections(float *prediction,std::vector<detection> &detections){
  int len = 5 + num_classes_;
  if (det_person_flag_) {
    len += 2;
  }
  ///> find out the number of valid detections
  std::vector<int> valid_detections;
  for (int i = 0; i < step_[nstrides_ - 1][1]; ++i) {
    float score = prediction[i * len + 4];
    if (score > config_thresh) {
      valid_detections.push_back(i);
    }
  }
  float top_occluded_prob = 0.0f;
  float bottom_occluded_prob = 0.0f;
  detections.reserve(valid_detections.size());
  for (size_t i = 0; i < valid_detections.size(); ++i) {
    detection det;int iprediction = valid_detections[i];
    if (det_person_flag_) {
      top_occluded_prob = prediction[iprediction * len + 5 + num_classes_ + 0];
      bottom_occluded_prob =
          prediction[iprediction * len + 5 + num_classes_ + 1];}
    float score = prediction[iprediction * len + 4];
    float x = prediction[iprediction * len + 0];
    float y = prediction[iprediction * len + 1];
    float w = prediction[iprediction * len + 2];
    float h = prediction[iprediction * len + 3];
    det.bbox.x = x;
    det.bbox.y = y;
    det.bbox.width = w;
    det.bbox.height = h;
    det.objectness = score;
    det.prob = prediction + iprediction * len + 5;
    if (det_person_flag_) {
      det.top_occluded_prob = top_occluded_prob;
      det.bottom_occluded_prob = bottom_occluded_prob;
    }
    for (int j = 0; j < num_classes_; ++j) {
      float prob = score * det.prob[j];
      det.prob[j] = (prob > config_thresh) ? prob : 0;
    }
    detections.push_back(det);
  }
}
std::vector<detection> YoloV3::pack_valid_detections(float *prediction) {
  std::vector<int> valid_detections;
  int len = 5 + num_classes_;
  if (det_person_flag_) {
    len += 2;
  }
  // find out the number of valid detections
  for (int i = 0; i < step_[nstrides_ - 1][1]; ++i) {
    float score = prediction[i * len + 4];
    if (score > config_thresh) {
      valid_detections.push_back(i);
    }
  }
  float top_occluded_prob = 0.0f;
  float bottom_occluded_prob = 0.0f;
  // convert to box_corner
  std::vector<detection> detections;
  for (size_t i = 0; i < valid_detections.size(); ++i) {
    detection det;
    int iprediction = valid_detections[i];
    if (det_person_flag_) {
      top_occluded_prob = prediction[iprediction * len + 5 + num_classes_ + 0];
      bottom_occluded_prob =
          prediction[iprediction * len + 5 + num_classes_ + 1];
    }
    float score = prediction[iprediction * len + 4];
    float x = prediction[iprediction * len + 0];
    float y = prediction[iprediction * len + 1];
    float w = prediction[iprediction * len + 2];
    float h = prediction[iprediction * len + 3];
    det.bbox.x = x;det.bbox.y = y;
    det.bbox.width = w;det.bbox.height = h;
    det.objectness = score;
    det.prob = prediction + iprediction * len + 5;
    if (det_person_flag_) {
      det.top_occluded_prob = top_occluded_prob;
      det.bottom_occluded_prob = bottom_occluded_prob;}
    for (int j = 0; j < num_classes_; ++j) {
      float prob = score * det.prob[j];
      det.prob[j] = (prob > config_thresh) ? prob : 0;}
    detections.push_back(det);}
  return detections;
}



void YoloV3::do_nms_sort(detection *dets, int total) {
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

  for (k = 0; k < num_classes_; ++k) {
    for (i = 0; i < total; ++i) {
      dets[i].sort_class = k;
    }

    qsort(dets, total, sizeof(detection), nms_comparator);
    for (i = 0; i < total; ++i) {
      if (dets[i].prob[k] == 0)
        continue;
      ArcternRect a = dets[i].bbox;
      for (j = i + 1; j < total; ++j) {
        ArcternRect b = dets[j].bbox;
        if (box_iou(a, b) > nms_thresh_) {
          dets[j].prob[k] = 0;
        }
      }
    }
  }
}

void YoloV3::postProcess(float *data, int width, int height, float thresh, Vbbox &vbbox) {
  if(thresh > 0 && thresh < 1) {
    config_thresh = thresh;
  }
  transform_grid(data);
  std::vector<detection> detections;
  pack_valid_detections(data,detections);
  do_nms_sort(&detections[0], detections.size());
  //pack_results(width, height, detections, vbbox);
  vbbox = pack_results(width, height, detections);
}

void YoloV3::postProcess2(float *data, int width, int height, float thresh, Vbbox &vbbox) {
  if(thresh > 0 && thresh < 1) {
    config_thresh = thresh;
  }
  transform_grid(data);
  std::vector<detection> detections;
  pack_valid_detections(data,detections);
  do_nms_sort(&detections[0], detections.size());
  vbbox = pack_results2(width, height, detections);
}

void YoloV3::postProcess3(float *data, int width, int height, float thresh, Vbbox &vbbox) {
  if(thresh > 0 && thresh < 1) {
    config_thresh = thresh;
  }
  transform_grid(data);
  std::vector<detection> detections;
  pack_valid_detections(data,detections);
  do_nms_sort(&detections[0], detections.size());
  vbbox = pack_results3(width, height, detections);
}

} // namespace arctern

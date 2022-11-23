/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.07
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include <vector>
#include <algorithm>
#include "src/Algorithms/faceDetectRTN/faceDetectRTNImpl.h"
#include "common/imageConvert.h"

using arctern::FaceDetectRTNImpl;
using arctern::ErrorType;
using arctern::Result;


FaceDetectRTNImpl::FaceDetectRTNImpl() {
  return;
}

FaceDetectRTNImpl::~FaceDetectRTNImpl() noexcept {
  FaceDetectRTNImpl::Release();
  return;
}

void FaceDetectRTNImpl::initPostProcessParam(int img_width,
                                             int img_height, std::vector<float> &v_scale_h,
                                             std::vector<float> &v_scale_w) {
  if (input_w_ == input_h_) {
    auto padSize = img_width > img_height ? img_width : img_height;
    v_scale_h.push_back(static_cast<float>(padSize) /
        static_cast<float>(input_h_));
    v_scale_w.push_back(static_cast<float>(padSize) /
        static_cast<float>(input_w_));
  } else {
    v_scale_h.push_back(static_cast<float>(img_height) /
        static_cast<float>(input_h_));
    v_scale_w.push_back(static_cast<float>(img_width) /
        static_cast<float>(input_w_));
  }
}

int FaceDetectRTNImpl::GetConfigInfo(arctern::MxnetModel *model) {
  bool return_res = try_get_value(*model, "INPUT_H", input_h_);
  return_res &= try_get_value(*model, "INPUT_W", input_w_);
  assert(input_w_>0 && input_w_ %4 ==0);
  return_res &= try_get_value(*model, "USING_LANDMARK", using_landmark_);
  if (conf_thresh_ < 0) {
    return_res &= try_get_value(*model, "CONF_THRESH", conf_thresh_);
  }
  bool quality_flag = try_get_value(*model, "USING_QUALITY", using_quality_);
  if (!quality_flag) {
    using_quality_ = false;
  }

  num_output_ = model->num_output();

  feature_maps_.clear();
  for (auto step : steps_) {
    int h = std::ceil(input_h_ / step);
    int w = std::ceil(input_w_ / step);
    feature_maps_.push_back({h, w});
  }
  anchors_ = generate_box();
  return 0;
}

void FaceDetectRTNImpl::InitPreprocessorParam() {
  preProcessor_.reset(new OpencvPreprocessor);
  OpencvPreprocessorInitPt initPt;
  initPt.net_h_ = input_h_;
  initPt.net_w_ = input_w_;
  initPt.mean_ = 127.5;
  initPt.std_ = 128.0;
  initPt.border_value = 0;
  initPt.alignMent_ = AlignMent::AlignLeft;

  auto code = preProcessor_->Init(&initPt);
  assert(code == ErrorType::ERR_SUCCESS);
}

void FaceDetectRTNImpl::InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) {
  runParm->sequence_.push_back(OperatorType::cvtColor);
  runParm->sequence_.push_back(OperatorType::detFace);
  runParm->sequence_.push_back(OperatorType::normalized);
}

std::vector<arctern::prior_box> FaceDetectRTNImpl::generate_box() {
  std::vector<prior_box> anchors;
  prior_box temp_anchor;
  for (size_t k = 0; k < feature_maps_.size(); k++) {
    std::vector<float> min_size = min_sizes_[k];
    int f0 = feature_maps_[k][0];
    int f1 = feature_maps_[k][1];
    for (int i = 0; i < f0; i++) {
      for (int j = 0; j < f1; j++) {
        for (auto m : min_size) {
          temp_anchor.s_kx = m / input_w_;
          temp_anchor.s_ky = m / input_h_;
          temp_anchor.cx = (j + 0.5) * steps_[k] / input_w_;
          temp_anchor.cy = (i + 0.5) * steps_[k] / input_h_;
          anchors.push_back(temp_anchor);
        }
      }
    }
  }
  return anchors;
}

arctern::Vbbox FaceDetectRTNImpl::nms(Vbbox &keep_bbox, float thresh) {
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
FaceDetectRTNImpl::decode_box_landmark(float *locations,
                                       float *scores, float *landmarks, float *quality,
                                       float scale_h, float scale_w, int stride) {
  Vbbox boxes;
  for (int i = 0; i < stride; i++) {
    float current_score = scores[2 * i + 1];
    if (current_score < initParam_.thresh_) {
      continue;
    }
    float center_x = anchors_[i].cx +
        locations[4 * i] * variance_[0] * anchors_[i].s_kx;
    float center_y = anchors_[i].cy +
        locations[4 * i + 1] * variance_[0] * anchors_[i].s_ky;
    float w = anchors_[i].s_kx * exp(locations[4 * i + 2] * variance_[1]);
    float h = anchors_[i].s_ky * exp(locations[4 * i + 3] * variance_[1]);

    float xmin = center_x - w / 2;
    float ymin = center_y - h / 2;
    float xmax = xmin + w;
    float ymax = ymin + h;

    float x1 = xmin * input_w_ * scale_w;
    float y1 = ymin * input_h_ * scale_h;
    float x2 = xmax * input_w_ * scale_w;
    float y2 = ymax * input_h_ * scale_h;
    float score_t = current_score;
    Bbox box(x1, y1, x2, y2, score_t);
    if (using_landmark_) {
      std::vector<cv::Point2f> temp_points;
      for (int j = 0; j < num_landmark_; j++) {
        cv::Point2f temp_point;
        temp_point.x =
            (anchors_[i].cx + landmarks[10 * i + 2 * j] *
                variance_[0] * anchors_[i].s_kx) * input_w_ *
                scale_w;
        temp_point.y =
            (anchors_[i].cy + landmarks[10 * i + 2 * j + 1] *
                variance_[0] * anchors_[i].s_ky) * input_h_ *
                scale_h;
        temp_points.push_back(temp_point);
      }
      box.lmd.leye = temp_points[0];
      box.lmd.reye = temp_points[1];
      box.lmd.nose = temp_points[2];
      box.lmd.lmouth = temp_points[3];
      box.lmd.rmouth = temp_points[4];
    }

    if (using_quality_) {
      box.quality = quality[i];
    }
    box.label = 0;
    boxes.push_back(box);
  }
  if (square_face_) {
    ConvertSquare(boxes, exth_scale_face_, extw_scale_face_);
  }
  return boxes;
}

ErrorType FaceDetectRTNImpl::LocalPostProcess(
    const RunParameter *p, int startId, int endId,
    std::vector<arctern::Tensor<float> > &outputs) {
  assert(p != nullptr);
  auto r = dynamic_cast<FaceDetectRTNResult *>(result_);
  std::vector<float> v_scale_h, v_scale_w;
  const int batchNum = initParam_.batchNum_;
  int total = endId - startId;
  int stride = outputs[0].size() / 4 / batchNum;
  for (int subId = 0; subId < total; subId++) {
    // cv::Mat &image = preprocessed_imgs[subId];
    // note!! if not gray rgb bgr, please compute size caffelly
    // initPostProcessParam(p->imageV_[loopId * batchNum + subId].width,
    // p->imageV_[loopId * batchNum + subId].height, v_scale_h, v_scale_w);
    initPostProcessParam(p->imageV_[startId + subId].width,
                         p->imageV_[startId + subId].height,
                         v_scale_h, v_scale_w);
    float *locations = outputs[0].data() + outputs[0].size() /
        batchNum * subId;
    float *scores = outputs[1].data() + outputs[1].size() /
        batchNum * subId;
    float *landmarks = outputs[2].data() + outputs[2].size() /
        batchNum * subId;
    float *quality = nullptr;
    if (using_quality_) {
      quality = outputs[3].data() + stride * subId;
    }

    auto vbox = decode_box_landmark(locations, scores,
                                    landmarks, quality, v_scale_h[subId],
                                    v_scale_w[subId], stride);
    Vbbox result = nms(vbox, nms_thresh_);
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
      // r->detInfos_[loopId * batchNum + subId].push_back(info);
      r->detInfos_[startId + subId].push_back(info);
    }
  }
  return ErrorType::ERR_SUCCESS;
}


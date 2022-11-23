//
// Created by gh on 20-11-4.
//

#include <stdio.h>
#include "src/Algorithms/faceDetectTiny/faceDetectTiny.h"
#include "src/Algorithms/hisiFaceTracker/IVETracker.h"

using namespace arctern;
std::string enum2string(TrackerState state);

#include "opencv2/opencv.hpp"
#include <cassert>
int main(int argc, char **argv) {

  arctern::FaceDetectTiny det;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.model_path =
      "/root/data4/chengaohua/arctern-models/develop/face/nnie3516/face-det-video-nnie3516-i8-0.0.3-q1.bin";
  auto ret = det.Init(&initParam);
  std::cout << ret.desc << std::endl;
  arctern::FaceDetectTinyConfigParameter configParam;
  configParam.confThresh_ = 0.6;
  det.SetParameter(&configParam);

  arctern::ObjectTrackerParameter tracker_parameter;
  tracker_parameter.single_tracker_type = "KCF";
  tracker_parameter.ive_input_img_width = 1280;
  tracker_parameter.ive_input_img_height = 720;
  tracker_parameter.min_obj_size = 31;
  tracker_parameter.min_iou_threshold = 0.65;
  tracker_parameter.max_time_lost = 5;

  IVETracker tracker(tracker_parameter);

  //setup_IVE_tracker_params(tracker_parameter);

  std::string dir = "/root/data4/chengaohua/arctern1.0/Data/IMG_8389/";
  for (int idx = 158; idx < 1000; idx++) {
    std::string path = dir + std::to_string(idx) + ".jpg";
    cv::Mat frame = cv::imread(path);
    cv::resize(frame,frame,cv::Size(1280,720));
    cv::Mat frameclone = frame.clone();

    if (idx % 5 == 0) {
      ::arctern::ArcternImage img;
      img.width = frame.cols;
      img.height = frame.rows;
      img.imgdata.gdata = frame.data;
      img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;

      arctern::RunParameter runParameter;
      runParameter.imageV_.push_back(img);

      std::cout << "Process start" << std::endl;
      auto result = det.Process(&runParameter);
      std::cout << "Process end" << std::endl;

      auto *detResult = dynamic_cast<const arctern::FaceDetectTinyResult *>(result);

      std::vector<cv::Rect> rects;
      if (detResult->errorType_ == arctern::ErrorType::ERR_SUCCESS) {
        int num = detResult->detInfos_[0].size();
        std::cout << "face num = " << num << std::endl;
        for (int i = 0; i < num; i++) {
          cv::Rect rect(detResult->detInfos_[0][i].rect.x, detResult->detInfos_[0][i].rect.y,
                        detResult->detInfos_[0][i].rect.width, detResult->detInfos_[0][i].rect.height);
          rects.push_back(rect);
        }
      }
      std::cout << "rect size = " << rects.size() << std::endl;

      tracker.IVE_update(frame, rects);
    }

    auto track_info = tracker.IVE_track(frame);

    for (auto ele : track_info) {
      auto bgr_face = ele.rect;
      cv::rectangle(frame, bgr_face, cv::Scalar(0, 255, 0), 1);
      std::string text = enum2string(ele.state) + std::to_string(ele.id);
      cv::putText(frame, text, bgr_face.tl(), cv::FONT_HERSHEY_COMPLEX, 1,
                  cv::Scalar(255, 0, 0));
    }

    std::string oPath = dir + std::to_string(idx) + "_output.jpg";
    cv::imwrite(oPath, frame);
  }

  return 0;
}

std::string enum2string(TrackerState state) {
  switch (state) {
    case NEW:return "new";
    case TRACKED:return "tracked";
    case LOST:return "lost";
    case REMOVED:return "removed";
    case REPLACED:return "replaced";
  }
  return "";
}

//
// Created by gh on 2020/9/30.
//


#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "detectRecogSink.h"
#include "src/Algorithms/faceTracker/faceTracker.h"

using namespace arctern;
std::string enum2string(TrackerState state);

int main(int argc, char *argv[]) {
  const std::string face_model_root = argv[1];
  std::string detModel = face_model_root + "face-det-tiny-f32-d-2.0.0.bin";
  std::string recogModel = face_model_root + "face-gluon-2.4.0.bin";

  const ObjectTrackerParameter param = ObjectTrackerParameter();

  std::cout << " tracker init begin" << std::endl;
  FaceTracker tracker(param);

  DetectRecogSink sink(detModel, recogModel);
  tracker.initDetectRecogSink(&sink);

  std::cout << " tracker init end" << std::endl;

  cv::VideoCapture capture;
  capture.open(argv[2]);
  if (!capture.isOpened()) {
    std::cout << " exit" << std::endl;
    exit(-1);
  }

  while (true) {
    printf("***demo track_face.\n");
    cv::Mat frame;
    capture >> frame;
    if (frame.empty()) {
      break;
    }

    cv::Mat frameclone = frame.clone();

    std::cout << " tracker begin " << std::endl;

    double start = cv::getTickCount();

    ::arctern::ArcternImage img;
    img.width = frame.cols;
    img.height = frame.rows;
    img.imgdata.gdata = frame.data;
    img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;


    auto retval = tracker.trackFace(img);
    double end = cv::getTickCount();
    std::cout << " track face = " << (end - start) / cv::getTickFrequency() * 1000 << " ms" << std::endl;
    auto track_info = retval.tracking_infos;

    for (auto ele : track_info) {
      auto bgr_face = ele.rect;
      cv::rectangle(frame, bgr_face, cv::Scalar(0, 255, 0), 1);
      std::string text = enum2string(ele.state) + std::to_string(ele.id);
      cv::putText(frame, text, bgr_face.tl(), cv::FONT_HERSHEY_COMPLEX, 1,
                  cv::Scalar(255, 0, 0));
    }

    cv::imshow("test", frame);
    cv::waitKey(1);
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

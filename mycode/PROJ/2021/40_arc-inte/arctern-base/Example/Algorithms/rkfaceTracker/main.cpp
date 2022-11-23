#include "src/Algorithms/faceDetectTiny/faceDetectTiny.h"
#include "src/Algorithms/rkfaceTracker/rkfaceTracker.h"

using namespace arctern;
std::string enum2string(TrackerState state);

void save(cv::Mat &mat, std::vector<TrackerInfo> &infos, std::string &opath) {
  for (auto & ele : infos) {
    auto bgr_face = ele.rect;
    cv::rectangle(mat, bgr_face, cv::Scalar(0, 255, 0), 1);
    std::string text = enum2string(ele.state) + std::to_string(ele.id);
    cv::putText(mat, text, bgr_face.tl(), cv::FONT_HERSHEY_COMPLEX, 1,
                cv::Scalar(255, 0, 0));
  }


   cv::imwrite(opath, mat);
}

#include "opencv2/opencv.hpp"
#include <cassert>
int main(int argc, char **argv) {

  arctern::FaceDetectTiny det;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.model_path =
      "/userdata/rknn/models/face-det-video-rv1109N-i8-q1-d-0.0.3.bin";
  initParam.thresh_ = 0.6;
  auto ret = det.Init(&initParam);

  arctern::ObjectTrackerParameter tracker_parameter;
  tracker_parameter.min_obj_size = 31;
  tracker_parameter.max_time_lost = 5;

  RKTracker tracker(tracker_parameter);

  //setup_IVE_tracker_params(tracker_parameter);

  std::string dir = "/userdata/IMG_8389/";
  for (int idx = 158; idx < 1000; idx++) {
    std::string path = dir + std::to_string(idx) + ".jpg";
    cv::Mat frame = cv::imread(path);
    // cv::resize(frame,frame,cv::Size(1280,720));
    cv::Mat frameclone = frame.clone();
    cv::Mat grayMat;
    cv::cvtColor(frame, grayMat, CV_BGR2GRAY);

    ::arctern::ArcternImage bgrImg;
    bgrImg.width = frame.cols;
    bgrImg.height = frame.rows;
    bgrImg.imgdata.gdata = frame.data;
    bgrImg.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;

    ::arctern::ArcternImage img;
    img.width = grayMat.cols;
    img.height = grayMat.rows;
    img.imgdata.gdata = grayMat.data;
    img.format = arctern::ARCTERN_IMAGE_FORMAT_GRAY;

    std::string oPath = dir + std::to_string(idx) + "_output.jpg";

    if (idx % 5 == 0) {
      arctern::RunParameter runParameter;
      runParameter.imageV_.push_back(img);

      arctern::FaceDetectTinyResult result;
      auto code = det.Process(&runParameter, &result);

      std::vector<cv::Rect> rects;
      if (code == arctern::ErrorType::ERR_SUCCESS) {
        int num = result.detInfos_[0].size();
        for (int i = 0; i < num; i++) {
          cv::Rect rect(result.detInfos_[0][i].rect.x, result.detInfos_[0][i].rect.y,
                        result.detInfos_[0][i].rect.width, result.detInfos_[0][i].rect.height);
          rects.push_back(rect);
        }
      }
      std::cout << "rect size = " << rects.size() << std::endl;

      auto track_info = tracker.RK_update(bgrImg, rects);
      save(frame, track_info.tracking_infos, oPath);
    } else {
      double start = cv::getTickCount();
      auto track_info = tracker.RK_track(bgrImg);
      double end = cv::getTickCount();
      std::cout << " tracker run = " << (end - start) / cv::getTickFrequency() * 1000 << " ms" << std::endl;
      save(frame, track_info.tracking_infos, oPath);

    }

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

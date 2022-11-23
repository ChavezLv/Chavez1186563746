/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.11
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#include <sstream>
#include <chrono>
#include "src/Algorithms/cameraOcclusionDetect/cameraOcclusionDetect.h"
#include "opencv2/opencv.hpp"
#include <cassert>
std::string getfilename(cv::String &fpathcv) {
    std::string fpath = fpathcv;
    std::stringstream ss;
    ss<<fpath;
    std::vector<std::string> vstr;
    std::string str1;
    while(getline(ss, str1,'/')) {
      vstr.push_back(str1);
    }
    return vstr.back();
}

std::string datapath = "./Data/background_estimation_data";

int main(int argc, char **argv) {
  arctern::CameraOcclusionDetect recog;
  arctern::CameraOcclusionDetectInitParameter initParam;
  initParam.ssim_thr = 0.75;
  auto ret = recog.Init(&initParam);


  assert(ret == arctern::ErrorType::ERR_SUCCESS);
  
  arctern::CameraOcclusionDetectRunParameter runParameter;

  int start_frame_idx = 0;
  int frame_distance = 1;
  int frame_counts = 220;
  std::vector<cv::String> file_names;
  std::string BACKGROUND_ESTIMATION_PATH = datapath;
  cv::glob(BACKGROUND_ESTIMATION_PATH, file_names);
  std::cout << "file num:"<< file_names.size() << std::endl;
  int frame_ind = 0;
  int frame_input_ind = 0;
  int frame_cover_count = 0;
  if (file_names.size() <= 1 ) {
    std::cout<<"Warning:not enough frames to estimate background images"<<std::endl;
    return 0;
  }
  std::vector<cv::Mat> frames;
  frames.resize(frame_counts + 1);
  for(size_t i = 0, j = 0; j < file_names.size(); j ++){
    frame_ind += 1;
    if(frame_ind > start_frame_idx)
      frame_cover_count += 1;
    if(frame_cover_count == frame_distance){
      frame_cover_count = 0;
      frame_input_ind += 1;
      frames[i] = cv::imread(file_names[j]);
      std::cout<<file_names[j]<<std::endl;
      //cv::imwrite(datapath + getfilename(file_names[j]), frames[i]);
      auto &frame = frames[i];
      i++;
      ::arctern::ArcternImage img;
      img.width = frame.cols;
      img.height = frame.rows;
      img.imgdata.gdata = frame.data;
      img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;
      runParameter.imageV_.push_back(img);
      if(frame_input_ind > frame_counts){
        break;
      }
    }
  }   
  auto start_t = std::chrono::high_resolution_clock::now();
  auto backgroundreal = recog.GetRealBackground(&runParameter);
  auto end_t = std::chrono::high_resolution_clock::now();
  auto elapsed_t = end_t - start_t;
  std::cout << "GetRealBackground time cost : "
      << std::chrono::duration<double, std::milli>(elapsed_t).count() 
      << " ms" << std::endl;
  auto implR = dynamic_cast<const arctern::CameraOcclusionDetectResult *>(backgroundreal);
  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    cv::Mat bgreal = cv::imdecode(implR->backgroundImageReal, CV_LOAD_IMAGE_COLOR);
    cv::imwrite("./bgreal.jpg", bgreal);
  }
 
  arctern::CameraOcclusionDetectRunParameter runParameter0;
  cv::Mat background = cv::imread(BACKGROUND_ESTIMATION_PATH + "/background_gt.png");
  std::vector<arctern::ArcternImage> background_vec;
  ::arctern::ArcternImage bg;
  bg.width = background.cols;
  bg.height = background.rows;
  bg.imgdata.gdata = background.data;
  bg.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;
  background_vec.push_back(bg);
  runParameter0.imageV_.assign(runParameter.imageV_.begin(), runParameter.imageV_.end());
  runParameter0.backgroundRefsV_.assign(background_vec.begin(), background_vec.end());

  start_t = std::chrono::high_resolution_clock::now();
  arctern::CameraOcclusionDetectResult result0;
  ret = recog.Process(&runParameter0,&result0);
  end_t = std::chrono::high_resolution_clock::now();
  elapsed_t = end_t - start_t;
  std::cout << "Process  time cost: "
      << std::chrono::duration<double, std::milli>(elapsed_t).count() 
      << " ms" << std::endl;

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    cv::Mat bgpred = cv::imdecode(result0.backgroundImagePred, CV_LOAD_IMAGE_COLOR);
    cv::imwrite("./bgpred0.jpg", bgpred);
    std::cout<<"0. isOcclusion :"<< result0.isOcclusion <<std::endl;
    std::cout<<"0. similarity  :"<< result0.similarity <<std::endl;
  }
  
  arctern::CameraOcclusionDetectRunParameter runParameter1;
  cv::Mat background_bad = cv::imread(BACKGROUND_ESTIMATION_PATH + "/background_bad.png");
  std::vector<arctern::ArcternImage> background_bad_vec;
  ::arctern::ArcternImage bgbad;
  bgbad.width = background_bad.cols;
  bgbad.height = background_bad.rows;
  bgbad.imgdata.gdata = background_bad.data;
  bgbad.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;
  background_bad_vec.push_back(bgbad);
  runParameter1.imageV_.assign(runParameter.imageV_.begin(), runParameter.imageV_.end());
  runParameter1.backgroundRefsV_.assign(background_bad_vec.begin(), background_bad_vec.end());

  arctern::CameraOcclusionDetectResult result1;
  ret = recog.Process(&runParameter1,&result1);

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    cv::Mat bgpred = cv::imdecode(result1.backgroundImagePred, CV_LOAD_IMAGE_COLOR);
    cv::imwrite("./bgpred1.jpg", bgpred);
    std::cout<<"1. isOcclusion :"<< result1.isOcclusion <<std::endl;
    std::cout<<"1. similarity  :"<< result1.similarity <<std::endl;
  }
  return 0;
}

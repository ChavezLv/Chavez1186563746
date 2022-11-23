/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.18
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "src/Algorithms/crowdHeatmap/crowdHeatmap.h"
#include <chrono>

#include "opencv2/opencv.hpp"
#include <cassert>
int main(int argc, char **argv) {
  arctern::CrowdHeatmap recog;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.model_path = argv[1];
  auto ret = recog.Init(&initParam);


  assert(ret == arctern::ErrorType::ERR_SUCCESS);

  cv::Mat image = cv::imread("/data/leizhiming/arctern-metadata/ModelsVerifyData/data/crownHeatmap/1.jpg");

  ::arctern::ArcternImage img;
  img.width = image.cols;
  img.height = image.rows;
  img.imgdata.gdata = image.data;
  img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;
  img.step = image.step;

  arctern::RunParameter runParameter;
  runParameter.imageV_.push_back(img);
  // runParameter.imageV_.push_back(img);

  arctern::CrowdHeatmapResult result;
  ret = recog.Process(&runParameter,&result);
  auto start_t = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < 10; ++i) {
    ret = recog.Process(&runParameter,&result);
  }
  auto end_t = std::chrono::high_resolution_clock::now();
  auto elapsed_t = end_t - start_t;
  std::cout << "crowdHeatmap time cost : "
      << std::chrono::duration<double, std::milli>(elapsed_t).count() 
      << " ms" << std::endl;

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    auto res0 = result.crowdHeatmaps_[0];
    cv::Mat hmap = cv::imdecode(res0.heatmap, CV_LOAD_IMAGE_COLOR);
    cv::imwrite("./Data/crowd_heatmap_imgs/heatmap_result.jpg", hmap);
    std::cout << "head count : " << res0.headCount << std::endl;
  }
  return 0;
}

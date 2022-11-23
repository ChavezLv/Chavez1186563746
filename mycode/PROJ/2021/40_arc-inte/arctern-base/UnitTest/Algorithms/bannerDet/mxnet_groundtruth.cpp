/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.09
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "./groundtruth.h"
#include "gtest/gtest.h"
#include "../../config.h"

#ifdef MXNET_ALLOWED
TEST(BannerDet, MXNET) {
  const std::string dir = ARCTERN_PROJECT_PATH + \
            std::string("/../arctern-metadata/ModelsVerifyData/");
  std::string ymlGroundTruthPath = dir + \
        "results_arcterncpp/LINUX_MXNET_CPU/banner-det-d-0.1.0-rc.yml";
  std::string ymlInputPath = dir + \
                    "input_params/banner-det-0.1.0-rc.yml";
  std::string imgDir = dir + "/data/detect_banner/";
  std::string modelPath = ARCTERN_PROJECT_PATH +std::string(\
      "/../arctern-models/develop/unclassified/banner-det-f32-d-0.1.0.bin");
  
  arctern::BannerDetResult result;
  //unique_ptr<arctern::Result> result(new arctern::BannerDetResult);
  BannerDetUnittest ground_truth(ymlGroundTruthPath, ymlInputPath,\
                    imgDir, modelPath, result);

  ground_truth.test();
}

#endif

/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         lvchaolin
 *  Last modified:  2021.12.20
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
        "results_arcterncpp/LINUX_MXNET_CPU/expose_rubbish-det-f32-d-0.1.0-rc.yml";
  std::string ymlInputPath = dir + \
                    "input_params/expose_rubbish-det-f32-d-0.1.0-rc-i.yml";
  std::string imgDir = dir + "data/detect_rubbish_tiny/";
  std::string modelPath = ARCTERN_PROJECT_PATH +std::string(\
      "/../arctern-models/develop/unclassified/expose_rubbish-det-f32-d-0.1.0.bin");
  

  arctern::RubbishDetectTinyResult result;
  RubbishDetectTinyUnittest ground_truth(ymlGroundTruthPath,imgDir, modelPath, &result);

  ground_truth.test();
}



#endif

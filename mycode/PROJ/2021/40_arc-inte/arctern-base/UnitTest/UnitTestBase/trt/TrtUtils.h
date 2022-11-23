//
// Created by Admin on 2021/7/19.
//

#ifndef ARCTERN_BASE_UNITTEST_UNITTESTBASE_TRT_TRTUTILS_H_
#define ARCTERN_BASE_UNITTEST_UNITTESTBASE_TRT_TRTUTILS_H_
#include "cuda_runtime.h"
#include "include/autoTime.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"
#include "gtest/gtest.h"
#include "opencv2/opencv.hpp"
#include "opencv2/cudacodec.hpp"
#include "opencv2/cudaimgproc.hpp"

typedef struct {
  string ymlGroundTruth;
  string ymlInput;
  string imgPath;
  string modelPath;
  string irModelPath;
  int gpuIndex;
  arctern::Result *result;
} TrtTestParam;


using BuildFunc = std::function<std::shared_ptr<arctern::RunParameter>()>;
using CheckFunc = std::function<void(const arctern::Result*)>;
#define GPUDATA 1
inline void CheckPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                           std::shared_ptr<BaseGroundTruthData> gtData,
                           int startId,
                           int len,
                           std::string imgDir,
                           const BuildFunc& build,
                           const CheckFunc& check,
                           arctern::Result *p) {
  auto runParameter = build();
  const std::string dir = imgDir;

  vector<cv::Mat> vMat;
  vector<cv::cuda::GpuMat> vGpuMat;
  vGpuMat.resize(len);
  vMat.resize(len);
  for (int i = 0; i < len; i++) {
    std::string fullpath = dir + gtData->files_[i + startId];
    vMat[i] = cv::imread(fullpath);

    EXPECT_FALSE(vMat[i].empty());

    vGpuMat[i].upload(vMat[i]);

    ::arctern::ArcternImage img;
#ifdef GPUDATA
    img.type = arctern::GPU;
    img.imgdata.gdata = vGpuMat[i].data;
    img.step = vGpuMat[i].step;
    img.width = vGpuMat[i].cols;
    img.height = vGpuMat[i].rows;
#else
    img.type = arctern::CPU;
    img.imgdata.gdata = vMat[i].data;
    img.step = vMat[i].step;
    img.width = vMat[i].cols;
    img.height = vMat[i].rows;
#endif


    img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;
    runParameter->imageV_.push_back(img);

  }

  arctern::ErrorType ret;
  {
    AUTOTIME;
    ret = alg->Process(runParameter.get(),p);
  }

  EXPECT_EQ(ret, arctern::ERR_SUCCESS);

  check(p);

}
#endif //ARCTERN_BASE_UNITTEST_UNITTESTBASE_TRT_TRTUTILS_H_

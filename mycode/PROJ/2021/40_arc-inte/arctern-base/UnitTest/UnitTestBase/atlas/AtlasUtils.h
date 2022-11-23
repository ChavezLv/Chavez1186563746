//
// Created by Admin on 2021/7/19.
//

#ifndef ARCTERN_BASE_UNITTEST_UNITTESTBASE_TRT_TRTUTILS_H_
#define ARCTERN_BASE_UNITTEST_UNITTESTBASE_TRT_TRTUTILS_H_
#include "include/autoTime.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"
#include "gtest/gtest.h"
#include "opencv2/opencv.hpp"
#include "src/common/file_buffer.h"
#include "src/Processors/atlasAclPreprocess.h"
#define USE_GPU_DATA 1
typedef struct {
  string ymlGroundTruth;
  string ymlInput;
  string imgPath;
  string modelPath;
  string modelPath8;
  int gpuIndex;
  arctern::Result *result;
} AtlasTestParam;


using BuildFunc = std::function<std::shared_ptr<arctern::RunParameter>()>;
using CheckFunc = std::function<void(const arctern::Result*)>;

inline void CheckPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                           std::shared_ptr<BaseGroundTruthData> gtData,
                           int startId,
                           int len,
                           std::string imgDir,
                           const BuildFunc& build,
                           const CheckFunc& check,
                           arctern::Result *p) {
#ifdef USE_GPU_DATA
   arctern::DvppDecodeManager decodeManage(0);
#endif
  auto runParameter = build();
  const std::string dir = imgDir;

  vector<arctern::FileBuffer> vFileBuffer;
  vFileBuffer.resize(len);

  for (int i = 0; i < len; i++) {
    std::string fullpath = dir + gtData->files_[i + startId];
    vFileBuffer[i].SetBufferFromFile(fullpath);

    arctern::ArcternImage img;

#ifdef USE_GPU_DATA
      
      arctern::ArcternAtlasImgData atlasData;
      atlasData.rawData = vFileBuffer[i].buffer_;
      atlasData.rawDataLen = vFileBuffer[i].length_;
      auto ret = decodeManage.CombineJpegdProcess(atlasData, PIXEL_FORMAT_YUV_SEMIPLANAR_420, true);
      EXPECT_EQ(ret, true);
      auto pDecodeData = decodeManage.GetDecodedImage();

      img.type = arctern::GPU;
      img.width = pDecodeData->width;
      img.height = pDecodeData->height;
      img.imgdata.atlasData.widthStride = pDecodeData->widthStride;
      img.imgdata.atlasData.heightStride = pDecodeData->heightStride;
      img.imgdata.atlasData.rawData = pDecodeData->data;
      img.imgdata.atlasData.rawDataLen = pDecodeData->dataSize;
#else
      img.type = arctern::CPU;
      img.imgdata.atlasData.rawData = vFileBuffer[i].buffer_;
      img.imgdata.atlasData.rawDataLen = vFileBuffer[i].length_;
#endif
    
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

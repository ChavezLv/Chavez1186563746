/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020/12/17
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/
#include "gtest/gtest.h"
#include "src/common/Tensor5.h"
#include "UnitTestForTensor.h"
using arctern::TensorShape5;
using arctern::Tensor5;
using arctern::UnitTestForTensor;
UnitTestForTensor::UnitTestForTensor() {
  int totalNum = batchNum_ * sequenceNum_;
  oriValue = new float[batchNum_ * sequenceNum_ * h_ * w_ * channel_];
  oriValueSize = batchNum_ * sequenceNum_ * h_ * w_ * channel_;
  gtValue = new float[batchNum_ * sequenceNum_ * h_ * w_ * channel_];
  gtValueSize = batchNum_ * sequenceNum_ * h_ * w_ * channel_;
  images.resize(totalNum);
  int imageIdx = 0;
  float value = 0;
  int oriValueIdx = 0;
  for (int i = 0; i < batchNum_; ++i) {
    for (int j = 0; j < sequenceNum_; ++j) {
      images[imageIdx] = cv::Mat(h_, w_, CV_32FC3);
      float *data = (float *) images[imageIdx].data;
      int dataIdx = 0;
      for (int ih = 0; ih < h_; ++ih) {
        for (int iw = 0; iw < w_; ++iw) {
          for (int ic = 0; ic < channel_; ++ic) {
            data[dataIdx] = value;
            oriValue[oriValueIdx] =    value;
            gtValue[i * channel_ * sequenceNum_ * h_ * w_ +
                 ic * sequenceNum_ * h_ * w_ + j * h_ * w_ +
                 ih * w_ + iw] =  value;
            value += 1;
            dataIdx++;
            oriValueIdx++;
          }
        }
      }
      ++imageIdx;
    }
  }
}

UnitTestForTensor::~UnitTestForTensor() {
   if(nullptr != oriValue) {
     delete[] oriValue;
     oriValue = nullptr;
   }
  if(nullptr != gtValue) {
    delete[] gtValue;
    gtValue = nullptr;
  }
}

TEST(UnitTestForTensor, Test5DimConversion)
{
  UnitTestForTensor testClass;
  int test[5] = {testClass.batchNum_, testClass.sequenceNum_, testClass.h_, testClass.w_, testClass.channel_};
  arctern::TensorShape5 tshape5(5, test);
  arctern::Tensor5<float> tsr5;
  tsr5.reshape(tshape5);

  tsr5.from_cvmat(testClass.images, tshape5, arctern::DataFormat::NTHWC, false);
  int totalNum = testClass.gtValueSize;
  EXPECT_EQ((int)tsr5.size(), (int)testClass.gtValueSize);
  EXPECT_EQ((int)tsr5.size(), (int)testClass.oriValueSize);
  printf("\n001 ---  -------------not swap ---------------- \n");
  float* data01 = tsr5.data();
  for (int i = 0; i < totalNum; ++i) {
    // printf("001 ---i=%d, gt %d  vs %d\n", i,(int) testClass.oriValue[i],(int) data01[i]);
    EXPECT_NEAR(testClass.oriValue[i], data01[i], 0.00001);
  }

  //std::cout << (int) tsr5.data_.get();
  printf("\n002 ---  -------------swap ---------------- \n");
  tsr5.from_cvmat(testClass.images, tshape5, arctern::DataFormat::NCTHW, true);
  float* data02 = tsr5.data();
  for (int i = 0; i < totalNum; ++i) {
    //printf("002 ---i=%d, gt %d  vs %d\n", i,(int) testClass.gtValue[i],(int) data02[i]);
    EXPECT_NEAR(testClass.gtValue[i], data02[i], 0.00001);
  }
}

TEST(UnitTestForTensor, Test5DimConversionPlus01)
{
  UnitTestForTensor testClass;
  int test[5] = {testClass.batchNum_, testClass.sequenceNum_, testClass.h_, testClass.w_, testClass.channel_};
  arctern::TensorShape5 tshape5(5, test);
  arctern::Tensor5<float> tsr5;
  tsr5.reshape(tshape5);

  int totalNum = testClass.gtValueSize;
  EXPECT_EQ((int)tsr5.size(), (int)testClass.gtValueSize);
  EXPECT_EQ((int)tsr5.size(), (int)testClass.oriValueSize);

  //std::cout << (int) tsr5.data_.get();
  printf("\n0012 ---  -------------swap ---------------- \n");
  tsr5.from_cvmat(testClass.images, tshape5, arctern::DataFormat::NCTHW, true);
  float* data02 = tsr5.data();
  for (int i = 0; i < totalNum; ++i) {
    //printf("002 ---i=%d, gt %d  vs %d\n", i,(int) testClass.gtValue[i],(int) data02[i]);
    EXPECT_NEAR(testClass.gtValue[i], data02[i], 0.00001);
  }

  arctern::Tensor5<float> tsr501;
  printf("\n0013 ---  -------------swap ---------------- \n");
  tsr501.reshape(tshape5);
  tsr501.from_cvmat(testClass.images, tshape5, arctern::DataFormat::NCTHW,
      1);
  float* data03 = tsr501.data();

  for (int i = 0; i < totalNum; ++i) {
    //printf("002 ---i=%d, gt %d  vs %d\n", i,(int) testClass.gtValue[i],(int) data02[i]);
    EXPECT_NEAR(testClass.gtValue[i], data03[i], 0.00001);
  }
}
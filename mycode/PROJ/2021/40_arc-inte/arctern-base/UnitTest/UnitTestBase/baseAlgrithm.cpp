/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.13
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "baseAlgrithm.h"
#include "gtest/gtest.h"
#include "include/autoTime.h"
#include "opencv2/opencv.hpp"
BaseAlgrithmUnittest::BaseAlgrithmUnittest(std::string ymlGroundTruth,
                                           std::string ymlInputPath,
                                           std::string imgDir,
                                           std::string modelPath,
                                           arctern::Result *result)
    : ymlGroundTruthPath_(ymlGroundTruth), ymlInputPath_(ymlInputPath), imgDir_(imgDir), modelPath_(modelPath) {
  result_ = result;
  fout_.open(ymlGroundTruthPath_ + ".txt",std::ios::binary | std::ios::out );
  if(!fout_.is_open()){
    std::cout << "create arctern-base result file failed." << std::endl;
  }
}

BaseAlgrithmUnittest::~BaseAlgrithmUnittest(){
  fout_.close();
}

void BaseAlgrithmUnittest::batchTest(const std::shared_ptr<BaseGroundTruthData> groundtruths) {
  unsigned int seed = time(nullptr);

  int batch = 1;

  // 只有trt 和atlas 支持batch
#if  defined(TRT_ALLOWED) || defined(ATLAS_ALLOWED)
  batch = 8;   /// 1 - 3
#endif

  auto alg = createAlgHandle(batch);

  int picturesnum = groundtruths->size();
  for (int idx = 0; idx < picturesnum; ) {
    int randnum = rand_r(&seed) % 7 + 1;   /// 1 - 8
    int left = picturesnum - idx;
    int minNum = std::min(randnum, left);
    checkPictures(alg, groundtruths, idx, minNum);
    idx += minNum;
  }
  alg->Release();
}

void BaseAlgrithmUnittest::singleTest(const std::shared_ptr<BaseGroundTruthData> groundtruths) {
  int batch = 1;
  auto alg = createAlgHandle(batch);

  int picturesnum = groundtruths->size();
  for (int idx = 0; idx < picturesnum; ) {
    int minNum = 1;
    checkPictures(alg, groundtruths, idx, minNum);
    idx += minNum;
  }
  alg->Release();
}

void BaseAlgrithmUnittest::checkPictures(std::shared_ptr<arctern::AlgorithmInterface> alg,
                                         std::shared_ptr<BaseGroundTruthData> gtData,
                                         int startId,
                                         int len) {
  auto runParameter = buildRunParameter(gtData, startId, len);
  vector<cv::Mat> mat;
  mat.resize(len);
  const std::string dir = imgDir_;
  for (int i = 0; i < len; i++) {
      std::string fullpath = dir + gtData->files_[i + startId];
      if(grayMat_) {
          mat[i] = cv::imread(fullpath,  cv::IMREAD_GRAYSCALE);
      } else {
          mat[i] = cv::imread(fullpath);
      }

      EXPECT_FALSE(mat.empty());

      //for CPU
      //使用本作用域内的mat仅仅初始化imageV_将不能达到预期效果
      //gdata只是指向mat.data并没有拷贝，在离开作用域将被Mat的析构函数释放，因此需要保证存储图片的对象在process执行完成之前不被释放
      //ArcternImage内存管理不科学，建议逐渐被舍弃
      ::arctern::ArcternImage img;
      img.width = mat[i].cols;
      img.height = mat[i].rows;

      img.imgdata.gdata = mat[i].data;
      if(grayMat_) {
          img.format = arctern::ARCTERN_IMAGE_FORMAT_GRAY;
      } else {
          img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;
      }
      runParameter->imageV_.push_back(img);

  }
  arctern::ErrorType ret;
  {
    AUTOTIME;
    ret = alg->Process(runParameter.get(),result_);
  }

  EXPECT_EQ(ret, arctern::ERR_SUCCESS);

  checkResult(gtData, startId, len, result_);
}

void BaseAlgrithmUnittest::test() {
  auto data = generateGroundTruth();
  cout <<endl<<endl<<"=========================Single Test=========================="<<endl<<endl<<endl;
  singleTest(data);
  cout <<endl<<endl<<"=========================Batch Test=========================="<<endl<<endl<<endl;
  /* batchTest(data); */
}

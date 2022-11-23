/****************************************************************************
 *  Filename:       aes_cryptor.cpp
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.11
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_COMMON_IMAGECONVERT_H_
#define SRC_COMMON_IMAGECONVERT_H_

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "include/arctern.h"
using namespace cv;
namespace arctern {

extern int ArcternImage2BgrMat(const ArcternImage &image, cv::Mat &bgrMat);
extern int ArcternImage2Mat(const ArcternImage &image, cv::Mat &bgrMat);
extern int Mat2RgbMat(const cv::Mat &image, ArcternImageFormat format , cv::Mat &rgbMat);
extern int Mat2BgrMat(const cv::Mat &image, ArcternImageFormat format , cv::Mat &bgrMat);
int Mat2GrayMat(const cv::Mat &image, ArcternImageFormat format , cv::Mat &grayMat);
extern int ArcternImage2RgbMat(const ArcternImage &image, cv::Mat &rgbMat);
extern int GetColorConversionCodes(const ArcternImageFormat &src,const ArcternImageFormat &dst);
extern int GetImageType(const int& channel,const int& depth);
extern int GetArcternImageChannels(const ArcternImageFormat &format);

}

#endif  // SRC_COMMON_IMAGECONVERT_H_

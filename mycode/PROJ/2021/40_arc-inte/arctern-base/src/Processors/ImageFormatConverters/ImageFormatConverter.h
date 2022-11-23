/****************************************************************************
 *  Filename:       ImageFormatConverter.h
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.09.16
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/
#ifndef SRC_PROCESSORS_IMAGEFORMATCONVERTERS_IMAGEFORMATCONVERTER_H_
#define SRC_PROCESSORS_IMAGEFORMATCONVERTERS_IMAGEFORMATCONVERTER_H_

#include <opencv2/opencv.hpp>
#include "include/arctern.h"

namespace arctern {
class ImageFormatConverter {
 public:
    ImageFormatConverter() {}
    virtual ~ImageFormatConverter() {}
    virtual arctern::ErrorType ConvertBeforeResize(const cv::Mat &image,
        arctern::ArcternImageFormat oriFormat,
        arctern::ArcternImageFormat dstFormat,
        arctern::ArcternImageFormat &outFormat, cv::Mat &dstImage) = 0;
    virtual arctern::ErrorType Convert(const cv::Mat &image,
        arctern::ArcternImageFormat oriFormat,
        arctern::ArcternImageFormat dstFormat,
        arctern::ArcternImageFormat &outFormat, cv::Mat &dstImage) = 0;
};
}  // namespace arctern
#endif  // SRC_PROCESSORS_IMAGEFORMATCONVERTERS_IMAGEFORMATCONVERTER_H_


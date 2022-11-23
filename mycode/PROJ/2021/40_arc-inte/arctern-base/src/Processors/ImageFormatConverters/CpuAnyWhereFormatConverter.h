/****************************************************************************
 *  Filename:       CpuAnyWhereFormatConverter.h
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.09.16
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/
#ifndef SRC_PROCESSORS_IMAGEFORMATCONVERTERS_CPUANYWHEREFORMATCONVERTER_H_
#define SRC_PROCESSORS_IMAGEFORMATCONVERTERS_CPUANYWHEREFORMATCONVERTER_H_

#include <opencv2/opencv.hpp>
#include "./src/Processors/ImageFormatConverters/ImageFormatConverter.h"
namespace arctern {

class CpuAnyWhereFormatConverter : public ImageFormatConverter {
 public:
    CpuAnyWhereFormatConverter() {}
    ~CpuAnyWhereFormatConverter() override;
    arctern::ErrorType ConvertBeforeResize(const cv::Mat &image,
        arctern::ArcternImageFormat oriFormat,
        arctern::ArcternImageFormat dstFormat,
        arctern::ArcternImageFormat &outFormat, cv::Mat &dstImage) override;
    arctern::ErrorType Convert(const cv::Mat &image,
        arctern::ArcternImageFormat oriFormat,
        arctern::ArcternImageFormat dstFormat,
        arctern::ArcternImageFormat &outFormat, cv::Mat &dstImage) override;
 private:
};
}  // namespace arctern
#endif  // SRC_PROCESSORS_IMAGEFORMATCONVERTERS_CPUANYWHEREFORMATCONVERTER_H_


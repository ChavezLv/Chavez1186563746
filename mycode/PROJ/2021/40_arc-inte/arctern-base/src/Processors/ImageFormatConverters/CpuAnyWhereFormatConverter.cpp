/****************************************************************************
 *  Filename:       CpuAnyWhereFormatConverter.cpp
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.09.16
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/
#include "log/logger.h"

#include "include/arctern.h"
#include "src/common/imageConvert.h"
#include "src/Processors/ImageFormatConverters/CpuAnyWhereFormatConverter.h"

using arctern::CpuAnyWhereFormatConverter;
using arctern::ErrorType;
using arctern::ArcternImageFormat;

CpuAnyWhereFormatConverter::~CpuAnyWhereFormatConverter() {
}

ErrorType CpuAnyWhereFormatConverter::ConvertBeforeResize(
    const cv::Mat &image, ArcternImageFormat oriFormat,
    ArcternImageFormat dstFormat, ArcternImageFormat &outFormat,
    cv::Mat &dstImage) {
  return Convert(image, oriFormat, dstFormat, outFormat, dstImage);
}

ErrorType CpuAnyWhereFormatConverter::Convert(
    const cv::Mat &image, ArcternImageFormat oriFormat,
    ArcternImageFormat dstFormat, ArcternImageFormat &outFormat,
    cv::Mat &dstImage) {
  outFormat = oriFormat;

  int ret = -1;
  switch (dstFormat) {
    case ARCTERN_IMAGE_FORMAT_BGR888: {
      ret = arctern::Mat2BgrMat(image, oriFormat, dstImage);
      break;
    }
    case ARCTERN_IMAGE_FORMAT_RGB888: {
      ret = arctern::Mat2RgbMat(image, oriFormat, dstImage);
      break;
    }
    case ARCTERN_IMAGE_FORMAT_GRAY: {
      ret = arctern::Mat2GrayMat(image, oriFormat, dstImage);
      break;
    }
    default: {
      SPDLOG_ERROR("error!!! no support format for dstFormat");
    }
  }

  if (ret != 0) {
    return ErrorType::ERR_ARCTERN_IMAGE_MAT;
  }
  outFormat = dstFormat;
  return ErrorType::ERR_SUCCESS;
}



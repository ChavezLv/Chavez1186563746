/****************************************************************************
 *  Filename:       aes_cryptor.cpp
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.11
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "imageConvert.h"
#include "log/logger.h"
#include "Base/base.h"
namespace arctern {

int ArcternImage2BgrMat(const ArcternImage &image, cv::Mat &bgrMat) {
  int ret = 0;
  if (nullptr == image.imgdata.gdata || 1 > image.height || 1 > image.width) {
    ret = -1;
    return ret;
  }

  unsigned char *ptr = (unsigned char *) image.imgdata.gdata;
  switch (image.format) {
    case ARCTERN_IMAGE_FORMAT_BGR888: {
      cv::Mat tmp(image.height, image.width, CV_8UC3, ptr);
      bgrMat = tmp;
      break;
    }
    case ARCTERN_IMAGE_FORMAT_RGB888: {
      cv::Mat tmp(image.height, image.width, CV_8UC3, ptr);
      cv::cvtColor(tmp, bgrMat, CV_RGB2BGR);
      break;
    }
    case ARCTERN_IMAGE_FORMAT_GRAY : {
      cv::Mat tmp(image.height, image.width, CV_8UC1, ptr);
      cv::cvtColor(tmp, bgrMat, CV_GRAY2BGR);
      break;
    }
    case ARCTERN_IMAGE_FORMAT_NV21: {
      cv::Mat tmp(image.height * 3 / 2, image.width, CV_8UC1, ptr);
      cv::cvtColor(tmp, bgrMat, CV_YUV2BGR_NV21);
      break;
    }
    default: {
      SPDLOG_ERROR("error!!! no support format");
      return -1;
    }
  }
  return ret;
}

int Mat2RgbMat(const cv::Mat &image, ArcternImageFormat format, cv::Mat &rgbMat) {
  int ret = 0;
  switch (format) {
    case ARCTERN_IMAGE_FORMAT_RGB888: {
      rgbMat = image;
      break;
    }
    case ARCTERN_IMAGE_FORMAT_BGR888: {
      cv::cvtColor(image, rgbMat, CV_BGR2RGB);
      break;
    }
    case ARCTERN_IMAGE_FORMAT_GRAY : {
      cv::cvtColor(image, rgbMat, CV_GRAY2RGB);
      break;
    }
    case ARCTERN_IMAGE_FORMAT_NV21: {
      cv::cvtColor(image, rgbMat, CV_YUV2RGB_NV21);
      break;
    }
    default: {
      spdlog::error("no support format");
    }
  }
  ret = 0;
  return ret;
}

int GetImageType(const int &channel, const int &depth) {
  if (channel == 1 && depth == 1) return U8C1;
  if (channel == 1 && depth == 4) return F32C1;
  if (channel == 3 && depth == 1) return U8C3;
  if (channel == 3 && depth == 4) return F32C3;
  return -1;
}

int GetColorConversionCodes(const ArcternImageFormat &src, const ArcternImageFormat &dst) {
  switch (dst) {
    case ARCTERN_IMAGE_FORMAT_BGR888: {
      switch (src) {
        case ARCTERN_IMAGE_FORMAT_RGB888:return ColorCvtType::RGB2BGR;
        case ARCTERN_IMAGE_FORMAT_BGR888:return -1;
        case ARCTERN_IMAGE_FORMAT_GRAY:return ColorCvtType::GRAY2BGR;
        case ARCTERN_IMAGE_FORMAT_NV21:return ColorCvtType::YUV2BGR_NV21;

        default:return -1;
      }
    }
    case ARCTERN_IMAGE_FORMAT_RGB888: {
      switch (src) {
        case ARCTERN_IMAGE_FORMAT_RGB888:return -1;
        case ARCTERN_IMAGE_FORMAT_BGR888:return ColorCvtType::BGR2RGB;
        case ARCTERN_IMAGE_FORMAT_GRAY:return ColorCvtType::GRAY2RGB;
        case ARCTERN_IMAGE_FORMAT_NV21:return ColorCvtType::YUV2RGB_NV21;

        default:return -1;
      }
    }
    case ARCTERN_IMAGE_FORMAT_GRAY : {
      switch (src) {
        case ARCTERN_IMAGE_FORMAT_RGB888:return ColorCvtType::RGB2GRAY;
        case ARCTERN_IMAGE_FORMAT_BGR888:return ColorCvtType::BGR2GRAY;
        case ARCTERN_IMAGE_FORMAT_GRAY:return -1;
        case ARCTERN_IMAGE_FORMAT_NV21:return ColorCvtType::YUV2GRAY_NV21;

        default:return -1;
      }
    }
    case ARCTERN_IMAGE_FORMAT_NV21: {
      switch (src) {
        case ARCTERN_IMAGE_FORMAT_RGB888:return ColorCvtType::RGB2YUV_I420;
        case ARCTERN_IMAGE_FORMAT_BGR888:return ColorCvtType::BGR2YUV_I420;
        case ARCTERN_IMAGE_FORMAT_GRAY:return -1;
        case ARCTERN_IMAGE_FORMAT_NV21:return -1;
        default:return -1;
      }
    }
    default: {
      return -1;
    }
  }
}

int GetArcternImageChannels(const ArcternImageFormat &format){
  int channels = 0;
  switch (format) {
    case arctern::ARCTERN_IMAGE_FORMAT_BGR888:
    case arctern::ARCTERN_IMAGE_FORMAT_RGB888:
    case arctern::ARCTERN_IMAGE_FORMAT_NV21:
      channels = 3;
      break;
    case arctern::ARCTERN_IMAGE_FORMAT_GRAY:
      channels = 1;
      break;
    default:
      std::cout <<"error!!! not support arctern image format."<<std::endl;
  }
  return channels;
}

int Mat2BgrMat(const cv::Mat &image, ArcternImageFormat format, cv::Mat &bgrMat) {
  int ret = 0;
  switch (format) {
    case ARCTERN_IMAGE_FORMAT_BGR888: {
      bgrMat = image;
      break;
    }
    case ARCTERN_IMAGE_FORMAT_RGB888: {
      cv::cvtColor(image, bgrMat, CV_RGB2BGR);
      break;
    }
    case ARCTERN_IMAGE_FORMAT_GRAY : {
      cv::cvtColor(image, bgrMat, CV_GRAY2BGR);
      break;
    }
    case ARCTERN_IMAGE_FORMAT_NV21: {
      cv::cvtColor(image, bgrMat, CV_YUV2BGR_NV21);
      break;
    }
    default: {
      spdlog::error("no support format");
    }
  }
  ret = 0;
  return ret;
}

int Mat2GrayMat(const cv::Mat &image, ArcternImageFormat format, cv::Mat &grayMat) {
  int ret = 0;
  switch (format) {
    case ARCTERN_IMAGE_FORMAT_GRAY: {
      grayMat = image;
      break;
    }
    case ARCTERN_IMAGE_FORMAT_RGB888: {
      cv::cvtColor(image, grayMat, CV_RGB2GRAY);
      break;
    }
    case ARCTERN_IMAGE_FORMAT_BGR888 : {
      cv::cvtColor(image, grayMat, CV_BGR2GRAY);
      break;
    }
    case ARCTERN_IMAGE_FORMAT_NV21: {
      cv::cvtColor(image, grayMat, CV_YUV2GRAY_NV21);
      break;
    }
    default: {
      spdlog::error("no support format");
    }
  }
  ret = 0;
  return ret;
}

int ArcternImage2Mat(const ArcternImage &image, cv::Mat &bgrMat) {
  int ret = 0;
  if (nullptr == image.imgdata.gdata || 1 > image.height || 1 > image.width) {
    ret = -1;
    return ret;
  }
  unsigned char *ptr = (unsigned char *) image.imgdata.gdata;
  switch (image.format) {
    case ARCTERN_IMAGE_FORMAT_BGR888: {
      bgrMat = cv::Mat(image.height, image.width, CV_8UC3, ptr);
      break;
    }
    case ARCTERN_IMAGE_FORMAT_RGB888: {
      bgrMat = cv::Mat(image.height, image.width, CV_8UC3, ptr);
      break;
    }
    case ARCTERN_IMAGE_FORMAT_GRAY : {
      bgrMat = cv::Mat(image.height, image.width, CV_8UC1, ptr);
      break;
    }
    case ARCTERN_IMAGE_FORMAT_NV21: {
      bgrMat = cv::Mat(image.height * 3 / 2, image.width, CV_8UC1, ptr);
      break;
    }
    default: {
      SPDLOG_ERROR("error!!! no support format");
      return -1;
    }
  }
  return ret;
}

int ArcternImage2Mat01(const ArcternImage &image, cv::Mat &rgbMat) {
  int ret = 0;
  do {
    if (nullptr == image.imgdata.gdata || 1 > image.height || 1 > image.width) {
      ret = -1;
      break;
    }
    unsigned char *ptr = (unsigned char *) image.imgdata.gdata;
    switch (image.format) {
      case ARCTERN_IMAGE_FORMAT_RGB888: {
        cv::Mat tmp(image.height, image.width, CV_8UC3, ptr);
        rgbMat = tmp;
        break;
      }
      case ARCTERN_IMAGE_FORMAT_BGR888: {
        cv::Mat tmp(image.height, image.width, CV_8UC3, ptr);
        cv::cvtColor(tmp, rgbMat, CV_BGR2RGB);
        break;
      }
      case ARCTERN_IMAGE_FORMAT_GRAY : {
        cv::Mat tmp(image.height, image.width, CV_8UC1, ptr);
        cv::cvtColor(tmp, rgbMat, CV_GRAY2RGB);
        break;
      }
      case ARCTERN_IMAGE_FORMAT_NV21: {
        cv::Mat tmp(image.height * 3 / 2, image.width, CV_8UC1, ptr);
        cv::cvtColor(tmp, rgbMat, CV_YUV2RGB_NV21);
        break;
      }
      default: {
        spdlog::error("no support format");
      }
    }
    ret = 0;
  } while (0);
  return ret;
}

int ArcternImage2RgbMat(const ArcternImage &image, cv::Mat &rgbMat) {
  int ret = 0;
  do {
    if (nullptr == image.imgdata.gdata || 1 > image.height || 1 > image.width) {
      ret = -1;
      break;
    }
    unsigned char *ptr = (unsigned char *) image.imgdata.gdata;
    switch (image.format) {
      case ARCTERN_IMAGE_FORMAT_RGB888: {
        cv::Mat tmp(image.height, image.width, CV_8UC3, ptr);
        rgbMat = tmp;
        break;
      }
      case ARCTERN_IMAGE_FORMAT_BGR888: {
        cv::Mat tmp(image.height, image.width, CV_8UC3, ptr);
        cv::cvtColor(tmp, rgbMat, CV_BGR2RGB);
        break;
      }
      case ARCTERN_IMAGE_FORMAT_GRAY : {
        cv::Mat tmp(image.height, image.width, CV_8UC1, ptr);
        cv::cvtColor(tmp, rgbMat, CV_GRAY2RGB);
        break;
      }
      case ARCTERN_IMAGE_FORMAT_NV21: {
        cv::Mat tmp(image.height * 3 / 2, image.width, CV_8UC1, ptr);
        cv::cvtColor(tmp, rgbMat, CV_YUV2RGB_NV21);
        break;
      }
      default: {
        spdlog::error("no support format");
      }
    }
    ret = 0;
  } while (0);
  return ret;
}
}  // namespace arctern

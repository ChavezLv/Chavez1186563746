/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file     if_common.h
/// @brief    公用定义C头文件
/// @author   caoyan
/// @date     2019/7/1
/// @version  1.0.1
/// @par      Copyright(C) 2018, Shenzhen Intellifusion Technologies Co.Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>

#ifndef IF_COMMON_H
#define IF_COMMON_H

#define MIN(a,b) ((b < a) ? b : a)
#define MAX(a,b) ((b > a) ? b : a)

/// @brief SDK状态
typedef enum {
  IF_STATE_UNINIT       = 0,    ///< 未初始化状态
  IF_STATE_INITED       = 1,    ///< 已初始化状态
  IF_STATE_DISCONNECT   = 2,    ///< 连接断开状态
  IF_STATE_CONNECTED    = 3,    ///< 已连接状态
  IF_STATE_WORKING      = 4,    ///< 工作状态
} if_state_t;

/// @brief 图像格式定义
typedef enum {
  //单通道图像
  IF_IMG_GRAY8        = 0, ///< 每个像素点灰度像素
  IF_IMG_RGB888       = 1, ///< 每个像素点按照RGB像素排列，每种8bit，共24bit
  IF_IMG_BGR888       = 2, ///< 每个像素点按照BGR像素排列，每种8bit，共24bit
  IF_IMG_YUV          = 3, ///< 每个像素点按照YUV像素排列，每种8bit，共24bit
  
  //YUV 4:2:0格式集
  IF_IMG_NV12         = 10, ///< 2通道, 一个通道是连续的亮度通道, 另一通道为UV分量交错
  IF_IMG_NV21         = 11, ///< 2通道, 一个通道是连续的亮度通道, 另一通道为VU分量交错
  IF_IMG_YU12         = 12, ///< 3通道，按顺序分别为Y/U/V
  IF_IMG_YV12         = 13, ///< 3通道，按顺序分别为Y/V/U
  IF_IMG_YUV420SP     = IF_IMG_NV12,
  IF_IMG_YUV420P      = IF_IMG_YU12,
  IF_IMG_I420         = IF_IMG_YU12,
  
  //YUV 4:2:2格式集
  IF_IMG_NV16         = 20, ///< YUV  4:2:2   16bpp (2通道, 一个通道是连续的亮度通道, 另一通道为UV分量交错)
  IF_IMG_NV61         = 21, ///< YUV  4:2:2   16bpp (2通道, 一个通道是连续的亮度通道, 另一通道为VU分量交错)
  IF_IMG_RGBA8888     = 22, ///< RGBA 8:8:8:8 32bpp (4通道32bit RGBA 像素)
  IF_IMG_ARGB8888     = 23, ///< ARGB 8:8:8:8 32bpp (4通道32bit ARGB 像素)
  IF_IMG_UYVY         = 24, ///< YUV  4:2:2   bt1120格式之一
  
  //编码格式
  IF_IMG_JPG          = 100, ///< JPEG压缩格式
  IF_IMG_PNG          = 101,///< PNG压缩格式

  IF_IMG_UNKNOWN      = -1,///< 未知格式，一般用于初始化
} if_image_format_t;

/// @brief 矩形框定义
typedef struct {
  int left; ///< 矩形最左边的坐标
  int top;  ///< 矩形最上边的坐标
  int right;  ///< 矩形最右边的坐标
  int bottom; ///< 矩形最下边的坐标
} if_rect_t;

/// @brief 图像类型定义
typedef struct {
  uint32_t frame_id; ///< 图像帧号，视频流帧号有效时必填，图片恒为0
  uint64_t time_stamp; ///< 时间戳，视频流时间戳有效时必填，单位ms，图片恒为0
  // if_image_format_t format; ///< 图像格式
  int format;
  int width; ///< 图像的像素宽
  int height; ///< 图像的像素高
  int stride; ///< 图像的行对齐字节长度，由SDK内部进行管理，用户不能指定这个值
  int size; ///< 图像数据字节大小
  uint8_t *data; ///< 图像数据
  uint8_t *extra_data[2]; ///< 仅用于传递其他格式中多通道指针，如AVFrame，extra_data[0] = avframe.data[1]
} if_image_t;

/// @brief 整型点定义
typedef struct {
  int x; ///< 点的水平方向坐标
  int y; ///< 点的竖直方向坐标
} if_point_t;

#endif //IF_COMMON_H

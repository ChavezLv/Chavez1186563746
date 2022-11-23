/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file     media_common.h
/// @brief    媒体服务公用头文件
/// @author   caoyan
/// @date     2019/8/21
/// @version  0.0.1
/// @par      Copyright(C) 2018, Shenzhen Intellifusion Technologies Co.Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MEDIA_COMMON_H
#define MEDIA_COMMON_H

namespace intellif {

typedef struct {
  if_rect_t rect; ///< 目标框位置
  std::string top_text; ///< 顶部文字
  std::string side_text; ///< 侧方文字(一般在右边)
  std::string bottom_text; ///< 底部文字
  int color;///< 目标颜色
} ObjectDisplayInfo;

typedef struct {
  uint64_t time_stamp = 0; ///< 时间戳
  uint64_t frame_id = 0;
  uint32_t det_delay = 0;
  uint32_t trk_delay = 0;
  std::vector<ObjectDisplayInfo> obj_infos; ///< 目标信息
} DisplayInfo;

} //namespace intellif

#endif //MEDIA_COMMON_H


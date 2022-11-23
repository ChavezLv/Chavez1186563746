/**
 * @file de_errorcode.h
 * @brief desdk错误码定义
 * @author DeSdk Team
 * @version v1.0.0
 * @date 2020/05/16
 * @copyright Intellif
 */

#pragma once

namespace de {

typedef enum
{
  DE_OK              = 0x00000000, ///< 执行成功
  DE_UNINIT          = 0x00001000, ///< 未初始化
  DE_PARAM_INVALID   = 0x00001001, ///< 参数错误
  DE_PART_SUCCESS    = 0x00001002, ///< 部分成功
  DE_NO_DEVICE_FOUND = 0x00001003, ///< 未探测到设备
  DE_LOAD_DLL_FAIL   = 0x00001004, ///< 加载动态库失败
  DE_TIMEOUT   	     = 0x00001005, ///< 超时
  DE_UNKNOWN         = 0xFFFFFFFF  ///< 未知错误
} ErrorCode;

}

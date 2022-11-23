/**
 * @file de_base_api.h
 * @brief 基本类型和接口定义
 * @author DeSdk Team
 * @date 2020/5/20
 * @copyright Intellif
 */

#ifndef DE_BASE_API_H_
#define DE_BASE_API_H_

#include "de_errorcode.h"
#include "core/de_c_type.h"
#include <vector>
#include <string>

namespace de {

/**
 * @brief DESDK初始化
 * @return 参考@enum ErrorCode
 * @attention 使用SDK前必须调用
 */
DE_API ErrorCode Init();

/**
 * @brief DESDK去初始化
 * @return 参考@enum ErrorCode
 * @attention 退出SDK前必须调用
 */
DE_API ErrorCode DeInit();

/**
 * @brief 获取SDK版本信息
 * @return SDK版本信息字符串
 */
DE_API std::string Version();

/**
 * @brief 获取SDK版本
 * @return SDK版本数字信息
 * @note 尚未实现
 */
DE_API uint32_t VersionNum();

/**
 * @brief DeSdk system init function
 * @param[in] dev_id device id
 * @note If call this function on host side, must provide a valid dev_id.
         If call this function on device side, param dev_id will be ignored.
 */
DE_API void DeSystemInit(int dev_id = 0);

/**
 * @brief DeSdk system de-init function
 * @param[in] dev_id device id
 * @note If call this function on host side, must provide a valid dev_id.
         If call this function on device side, param dev_id will be ignored.
 */
DE_DLL void DeSystemDeinit(int dev_id = 0);

/**
 * @brief 读取desdk配置文件
 * @param[in] cfg_file desdk配置文件
 * @note 若没有指定配置文件或者配置文件不存在，使用默认配置。否则，从文件中读取desdk配置
 */
DE_API void DeSDKReadConfig(const std::string& cfg_file = "/root/cfg/desdk.cfg");

/**
 * @brief 判断SDK是否仍在工作中
 * @return 返回TRUE表示SDK仍在工作中
 * @note 该函数绝大部分情况下返回true，只在一种情况下会返回false: 
         在Device端，sdkmd收到了切换至desdk_rpc_server的命令，等待当前RPCSession结束。此后该函数返回false。
 */
DE_API bool DeSDKAlive(void);

}

#endif


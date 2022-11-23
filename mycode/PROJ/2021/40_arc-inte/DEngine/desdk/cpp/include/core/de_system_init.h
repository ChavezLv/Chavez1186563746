/**
 * @file de_system_init.h
 * @brief Declare system init and system deinit in DeSdk
 * @author DeSdk Team
 * @date 2019/12/15
 * @copyright Intellif
 * @note Open a P2P channel to send log in device to host when RPC connected, \
 *       and close this channel when RPC connection shut down.
 */

#ifndef DE_SYSTEM_INIT_H_H_
#define DE_SYSTEM_INIT_H_H_

#include <core/de_c_type.h>
#include <string>

namespace de {

constexpr int kMaxDevNum = 32;
constexpr int kDev2HostAgentMaxSize = 16 << 10;
constexpr int kHost2DevAgentMaxSize = 1 << 20;

/**
 * @brief Desdk功能初始化
 * @param[in] dev_id 设备ID，仅在Host端有效
 * @note 在Host端调用该函数时，必须指定一个有效的设备ID
         在Device端调用该函数时，该参数被忽略
 */
DE_DLL void DeSDKInit(int dev_id = kMaxDevNum);


/**
 * @brief Desdk功能去初始化
 * @param[in] dev_id 设备ID，仅在Host端有效
 * @note 在Host端调用该函数时，必须指定一个有效的设备ID
         在Device端调用该函数时，该参数被忽略
 */
DE_DLL void DeSDKDeInit(int dev_id = kMaxDevNum);

/**
 * @brief Get dev to host agent channel
 * @param[in] dev_id device id
 * @return	  P2p channel name
 * @note This function is only available on host side. Must provide a valid dev_id.
 */
DE_DLL std::string GetDev2HostChannelName(int dev_id = kMaxDevNum);

/**
 * @brief Get host to dev agent channel
 * @param[in] dev_id device id
 * @return	  P2p channel 
 * @note This function is only available on host side. Must provide a valid dev_id.
 */
DE_DLL void* GetHost2DevChannel(int dev_id) ;

/**
* @brief Config Aie Mode of device(dp1000)
* @param[in]			mode0			core0's aie mode,  value 0~3  means [0-NN Normal,1-NN Enhance,2-CV,3-NN Test]
* @param[in]			mode1			core1's aie mode,  value 0~3  means [0-NN Normal,1-NN Enhance,2-CV,3-NN Test]
* @param[in]			dev_id			must provide a valid dev_id on host side; on device side , this parameter is ignored
* @Note This function should be called before DeSystemInit to take effect!
*/
DE_DLL void DevAieModeConfig(int32_t mode0,int32_t mode1,int dev_id = kMaxDevNum);

}

#endif

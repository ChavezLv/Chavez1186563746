/**
 * @file de_dcmi_api.h
 * @brief desdk设备管理接口
 * @author DeSdk Team
 * @version v1.0.0
 * @date 2020/05/16
 * @copyright Intellif
 */

#pragma once
#include <functional>
#include <string>
#include <vector>

#include <core/de_c_type.h>
#include <de_errorcode.h>

namespace de {
namespace dcmi {

typedef uint32_t DevId;         ///< 设备ID
typedef uint32_t CardId;        ///< 板卡ID
typedef uint32_t CardDevIndex;  ///< 单个板卡上的设备索引

typedef enum {
  BOOTMODE_USB      = 0,
  BOOTMODE_EMMC     = 1,
  BOOTMODE_NAND     = 2,
  BOOTMODE_BOTTOM   = BOOTMODE_NAND,
} BootMode;

typedef enum {
  AMT_OK              = 0x0,        ///< 生产测试成功
  AMT_NOT_START       = 0x80000000, ///< 生产测试未开始
  AMT_DDR_FAIL        = 0x80000001, ///< 生产测试DDR失败
  AMT_NNP_FAIL        = 0x80000002, ///< 生产测试NNP失败
  AMT_CV_FAIL         = 0x80000004, ///< 生产测试CV失败
  AMT_FILE_FAIL       = 0x80000008, ///< 生产测试FILE失败
  AMT_INTERRUPTED     = 0x80000010, ///< 生产测试被打断
  AMT_FUNC_NOT_REG    = 0x80000020, ///< 设备端生产测试函数未注册
  AMT_DEVICE_NOEXIST  = 0x80000040, ///< 生产测试指定的设备不存在
} AmtErrorCode;

typedef enum {
  UPGRADE_START           = 0x00002000, ///< 升级开始
  UPGRADE_FINISH          = 0x00002001, ///< 升级完成
  UPGRADE_FAIL            = 0x00002002, ///< 升级失败
  UPGRADE_PART_SUCCESS    = 0x00002003, ///< 升级部分成功
  UPGRADE_NO_DEVICE_FOUND = 0x00002004, ///< 未探测到设备
  UPGRADE_FILE_NO_EXIST   = 0x00002005, ///< 未找到文件
  UPGRADE_UNKNOWN         = 0xFFFFFFFF  ///< 未知错误
} UpgradeStatus;

typedef enum {
  DEVICE_OFFLINE    = 0x1100, ///< 设备离线
  DEVICE_ONLINE     = 0x1101, ///< 设备在线
  DEVICE_UNKNOWN    = 0x1102, ///< 设备未知(配置文件无效/ID无效/未使能/USB不识别)
} DevStatus;

typedef enum {
  //event

  //exception
  //Kernel: 0-99
  EXCEPTION_COMMON        = 0, ///< String to be print.
  EXCEPTION_COREDUMP,          ///< Coredump file path and its size.
  EXCEPTION_KERNEL_PANIC,      ///< Kernel panic, unrecoverable.
  EXCEPTION_KERNEL_LOCKUP,     ///< Found process lockup, need to check root cause.
  EXCEPTION_KERNEL_OOM,        ///< Found out of memory, maybe memory leakage.
  EXCEPTION_KERNEL_BOOTMODE,   ///< Show bootmode, NORM(Normal)/WDTx(Watchdog/Sw reset)/HW(Hw reset)/TSEN(Thermal).
  EXCEPTION_KERNEL_TEMP,
  //System common: 100-199
  EXCEPTION_SYSTEM_COMMON = 100,
  //Heartbeat
  EXCEPTION_HEARTBEAT     = 200,
  //DP1000 Subsystem
  //Multimedia
  EXCEPTION_MM_DROPPACKET = 210,
  EXCEPTION_MM_G1,
  EXCEPTION_MM_G2,
  //VP(todo:补充完全)
  EXCEPTION_VP_XX         = 220,
  //Connection(todo:补充完全)
  EXCEPTION_CONN_XX       = 230,
  EXCEPTION_USB_XX        = 240,
  EXCEPTION_MEMOEY_XX     = 250,
  EXCEPTION_AIDRV_XX      = 260,
  EXCEPTION_RPC_XX        = 270,
  EXCEPTION_P2P_XX        = 280,
  EXCEPTION_VIDEOIN_XX    = 290,
  EXCEPTION_AIE_XX        = 300,
  EXCEPTION_CV_XX         = 310,
  EXCEPTION_CORE_XX       = 320,
} EventType;

typedef enum
{
  MODE_P2P          	  = 1, 
  MODE_P2P_SERIAL         = 2, 
  MODE_P2P_UVC            = 3, 
  MODE_P2P_UVC_SERIAL     = 4, 
  MODE_P2P_HID            = 5, 
  MODE_P2P_HID_SERIAL     = 6, 
  //MODE_HID          	  = 7, 
  //MODE_HID_SERIAL       = 8, 
} UsbMode;

typedef struct
{
  DevId dev_id; ///< 设备ID
  UsbMode mode; ///< 设备ID对应的usb模式
  int timeout;  /// 超时，单位ms
} UsbModeCfg;

typedef struct {
  DevId dev_id;           ///< 设备ID
  std::string version;    ///< 版本信息字符串
  uint32_t version_num;   ///< 版本号 0a0b0c代表a.b.c版本
} VersionInfo;

typedef struct {
  DevId dev_id;           ///< 设备ID
  std::string file_path;  ///< 版本配置文件路径
  int timeout;            ///< 超时，单位ms
  bool flash_flag;        ///< flag 0 表示不刷flash, 1 表示刷flash
} VersionCfg;

typedef struct {
  DevId dev_id;           ///< 设备ID
  std::string cmd;        ///< 要执行的命令
  int timeout;            ///< 超时，单位ms
} XferCmdCfg;

typedef struct {
  DevId dev_id;           ///< 设备ID
  std::string src_path;   ///< 源文件路径
  std::string dst_path;   ///< 目的文件路径
  int timeout;            ///< 超时，单位ms
} XferFileCfg;

typedef struct {
  CardId card_id;         ///< 板卡ID
  CardDevIndex dev_index; ///< 单个板卡上的设备索引
} CardDevId;

typedef struct {
  CardId card_id;         ///< 板卡ID
  CardDevIndex dev_index; ///< 单个板卡上的设备索引
  std::string file_path;  ///< 版本配置文件路径
  int timeout;            ///< 超时，单位ms
  bool flash_flag;        ///< flag 0 表示不刷flash, 1 表示刷flash
} CardVersionCfg;

typedef struct {
  int32_t DevIdEnable;
  int32_t DevId;
  uint64_t DevKey;
  int32_t CardIdEnable;
  int32_t CardId;
  int32_t CardDevIndex;
} DevCfgInfo;

/** @struct HeartbeatConfigInfo
 * @brief Configure heartbeat.
 */
typedef struct {
	uint32_t heartbeat_cycle;	///< hearbeat cycle in seconds
	uint32_t loss_cnt;			///< lost cnt to report failure
}HeartbeatConfigInfo;

typedef struct {
	uint32_t feed_interval; 	///< feed time in seconds
	uint32_t reset_timeout; 	///< timeout to reboot in seconds
}WatchdogConfigInfo;

/**
 * @brief dcmi模块初始化
 * @param[in] nums DeepEye芯片个数,如果指定了USB_CONFIG_INI，该参数将忽略
 * @return 参考@enum ErrorCode
 * @attention 使用DCMI模块的设置事件回调/升级/传输接口前必须调用
 */
DE_API ErrorCode Init(unsigned int nums);

/**
 * @brief dcmi模块去初始化
 * @return 参考@enum ErrorCode
 * @attention 退出dcmi模块前必须调用
 */
DE_API ErrorCode DeInit();

/**
 * @brief 获取支持的最大DeepEye芯片个数
 * @return 芯片个数
 */
DE_API int MaxDevNum();

/**
 * @brief 获取设备状态
 * @param[in] dev_id 设备ID
 * @param[out] status 设备状态
 * @return 总体执行结果，参考@enum ErrorCode
 */
DE_API ErrorCode GetDevStatus(DevId dev_id, DevStatus &status);

/**
 * @brief 探测当前已连接设备,生成USB配置文件(usbprop.ini)
 * @param[in] dev_nums 设备个数
 * @param[in] switch_mode 是否切换模式:
 * - true  依次切换到usb和work两种模式下探测,用于有硬件控制DeepEye芯片启动模式和复位引脚的平台且能启动固件
 * - false 直接探测,用于无硬件控制DeepEye芯片启动模式或复位引脚的平台,或不能启动固件
 * @param[out] ini_path 配置文件路径
 * @return 参考@enum ErrorCode
 */
DE_API ErrorCode GenUsbCfgIni(int dev_nums, bool switch_mode, const std::string &ini_path);

/**
 * @brief 事件回调函数定义
 * @param[in] dev_id 设备ID
 * @param[in] message_code 消息码，请参数@enum EventType定义
 * @param[in] message 事件消息字符串
 */
using EventCallback = std::function<void(DevId dev_id, EventType message_code, const std::string &message)>;

/**
 * @brief 设置事件回调函数
 * @param[in] dev_id 设备ID
 * @param[in] cb 事件回调函数指针
 * @return 参考@enum ErrorCode
 * @attention 使用前必须调用一次de::dcmi::Init()
 */
DE_API ErrorCode SetEventCallback(DevId dev_id, EventCallback cb);

DE_API ErrorCode StartHostHeartBeat(DevId dev_id,HeartbeatConfigInfo* cfg,bool device_reboot = true);
DE_API ErrorCode StopHostHeartBeat(DevId dev_id,bool device_ok = true);


DE_API ErrorCode StartDeviceWdt(DevId dev_id,WatchdogConfigInfo* cfg);
DE_API ErrorCode StopDeviceWdt(DevId dev_id,bool device_ok = true);


/**
 * @brief 设置host事件回调函数
 * @param[in] dev_id 设备ID
 * @param[in] cb 事件回调函数指针
 * @return 参考@enum ErrorCode
 */
DE_API ErrorCode SetEventHostCallback(DevId dev_id, EventCallback cb);

/**
 * @brief 执行生产测试
 * @param[in] dev_ids 设备ID
 * @param[in] time_out 超时时间，单位ms
 * @param[out] result 设备结果，直接使用数组下标访问对应dev_id的结果
 * @return 参考@enum ErrorCode
 * @note 支持设置多芯片，不支持多线程重入
 */
DE_API ErrorCode AmtTest(const std::vector<DevId> &dev_ids, int time_out, std::vector<AmtErrorCode> &result);

/**
 * @brief DeepEye芯片版本升级(补丁包升级/用户自定义脚本)
 * @param[in] configs 升级版本配置
 * @param[out] result 每个设备的执行结果，直接使用数组下标访问对应dev_id的结果
 * @return 参考@enum ErrorCode
 * @note 支持设置多芯片，不支持多线程重入
 * @attention 使用前必须调用一次de::dcmi::Init()
 */
DE_API ErrorCode Upgrade(const std::vector<VersionCfg> &configs, std::vector<ErrorCode> &result);

/**
 * @brief DeepEye芯片全版本下载
 * @param[in] configs 下载版本配置
 * @param[out] result 每个设备的执行结果，直接使用数组下标访问对应dev_id的结果
 * @return 参考@enum ErrorCode
 * @note 支持设置多芯片，不支持多线程重入
 * @attention 仅限USB连接方式使用
 */
DE_API ErrorCode Download(const std::vector<VersionCfg> &configs, std::vector<ErrorCode> &result);

/**
 * @brief 发送命令到device上执行
 * @param[in] configs 命令配置
 * @param[out] results 执行结果，直接使用数组下标访问对应dev_id设备的结果
 * @return 参考@enum ErrorCode
 * @attention 使用前必须调用一次de::dcmi::Init()
 */
DE_API ErrorCode SendCommand(const std::vector<XferCmdCfg> &configs, std::vector<ErrorCode> &results);

/**
 * @brief 发送文件到device
 * @param[in] configs 文件配置
 * @param[out] results 执行结果，直接使用数组下标访问对应dev_id设备的结果
 * @return 参考@enum ErrorCode
 * @note XferFileCfg中src_path是host侧文件路径，dst_path是device侧文件路径
 * @attention 使用前必须调用一次de::dcmi::Init()
 */
DE_API ErrorCode SendFile(const std::vector<XferFileCfg> &configs, std::vector<ErrorCode> &results);

/**
 * @brief 从device接收文件
 * @param[in] configs 文件配置
 * @param[out] results 执行结果，直接使用数组下标访问对应dev_id设备的结果
 * @return 参考@enum ErrorCode
 * @note XferFileCfg中src_path是device侧文件路径，dst_path是host侧文件路径
 * @attention 使用前必须调用一次de::dcmi::Init()
 */
DE_API ErrorCode ReceiveFile(const std::vector<XferFileCfg> &configs, std::vector<ErrorCode> &results);

/**
 * @brief 设置device的usb模式
 * @param[in] config usb模式的配置信息
 * @param[out] status 每个设备的执行结果，直接使用数组下标访问对应dev_id的结果
 * @return 总体执行结果，参考@enum ErrorCode
 */
DE_API ErrorCode SetUsbMode(const std::vector<UsbModeCfg> &config, std::vector<ErrorCode> &result);

/*-----------------only for pcie card interface (card id, device index) -------------------------*/
/**
 * @brief 事件回调函数定义
 * @param[in] dev_id 设备ID
 * @param[in] message_code 消息码，请参考@enum EventType定义
 * @param[in] message 事件消息字符串
 */
//using EventCardCallback = std::function<void(CardId card_id, CardDevIndex dev_index, EventType message_code, const std::string &message)>;

/**
 * @brief 获取设备配置信息
 * @param[in] card_id
 * @param[in] dev_index
 * @param[out] devcfg config info
 * @return 总体执行结果，参考@enum ErrorCode
 */
DE_API ErrorCode GetDevConfigInfo(CardId card_id, CardDevIndex dev_index, DevCfgInfo &devcfg);

/**
 * @brief 获取设备状态
 * @param[in] card_id
 * @param[in] dev_index
 * @param[out] dev_id 设备ID
 * @param[out] status 设备状态
 * @return 总体执行结果，参考@enum ErrorCode
 */
DE_API ErrorCode GetDevStatus(CardId card_id, CardDevIndex dev_index, DevId &dev_id, DevStatus &status);

/**
 * @brief 设置事件回调函数
 * @param[in] card id
 * @param[in] dev index
 * @param[in] cb 事件回调函数指针
 * @return 参考@enum ErrorCode
 */
DE_API ErrorCode SetEventCallback(CardId card_id, CardDevIndex dev_index, EventCallback cb);

/**
 * @brief 设置host事件回调函数
 * @param[in] card id
 * @param[in] dev index
 * @param[in] cb 事件回调函数指针
 * @return 参考@enum ErrorCode
 */
DE_API ErrorCode SetEventHostCallback(CardId card_id, CardDevIndex dev_index, EventCallback cb);

/**
 * @brief 执行生产测试
 * @param[in] card_dev_ids
 * @param[in] time_out 超时时间，单位ms
 * @param[out] result 设备结果，使用dev_id作为数组下标访问对应的结果
 * @return 参考@enum ErrorCode
 * @note 支持设置多芯片，不支持多线程重入
 */
DE_API ErrorCode AmtTest(const std::vector<CardDevId> &card_dev_ids, int time_out, std::vector<AmtErrorCode> &result);

/**
 * @brief DeepEye芯片版本升级(补丁包升级/用户自定义脚本)
 * @param[in] configs 升级版本配置
 * @param[out] result 每个设备的执行结果，使用dev_id作为数组下标访问对应的结果
 * @return 参考@enum ErrorCode
 * @note 支持设置多芯片，不支持多线程重入
 */
DE_API ErrorCode Upgrade(const std::vector<CardVersionCfg> &configs, std::vector<ErrorCode> &result);

/**
 * @brief DeepEye芯片全版本下载
 * @param[in] configs 下载版本配置
 * @param[out] result 每个设备的执行结果，使用dev_id作为数组下标访问对应的结果
 * @return 参考@enum ErrorCode
 * @note 支持设置多芯片，不支持多线程重入
 * @attention 仅限USB连接方式使用
 */
DE_API ErrorCode Download(const std::vector<CardVersionCfg> &configs, std::vector<ErrorCode> &result);

}
}

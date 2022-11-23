/**
 * @file de_config.h
 * @brief 读取配置文件(json)格式，提供系统参数配置
 * @author Desdk Group
 * @version V1.0
 * @date 2020-05-28
 */

#ifndef DE_CONFIG_H_
#define DE_CONFIG_H_

#include <core/de_prop.h>

#define LOG_CFG (de::SDKCfg::Global()->log)
#define MODE_CFG (de::SDKCfg::Global()->mode)
#define MM_CFG (de::SDKCfg::Global()->mulMedia)
#define CONN_CFG (de::SDKCfg::Global()->connect)
#define MEM_CFG (de::SDKCfg::Global()->memory)
#define AIE_CFG (de::SDKCfg::Global()->aiEng)
#define CV_CFG (de::SDKCfg::Global()->cvEng)


namespace de
{


class DE_DLL SDKCfg
{
public:
    using CfgCBFunc = std::function<void(const Prop&)>;

    /**
     * @brief Log 等级设置
     * 0 - DEBUG
     * 1 - INFO
     * 2 - IMPORTANT
     * 3 - WARNING
     * 4 - ERROR
     * 5 - FATAL
     */
    struct LogLevel
    {
        // DESDK系统log等级
#ifdef DEBUG
        int32_t ck810 = 0;
#else
        int32_t ck810 = 2;
#endif
        int32_t dsp = 33; // 传递给DSP的log等级设置
        int32_t klog = 0;

        // 子模块
        // 注意：只有子模块的log等级和系统CK810的log等级全部满足要求，才输出
        int32_t core = 2;  // SDK Core 相关的log等级设置
        // fml
        int32_t cv_eng = 2;
        int32_t aie_eng = 2;
        int32_t video_in = 2;
        int32_t video_dec = 2;
        int32_t video_enc = 2;
        int32_t pic_dec = 2;
        int32_t pic_enc = 2;
        int32_t p2p_chan = 2;
        int32_t rpc_chan = 2;

        // hal
        int32_t usb_hal = 2;
        int32_t mem_hal = 2;
        int32_t dsp_hal = 2; // dsp hal driver
        int32_t aidrv_hal = 2;

    };

    /**
     * @brief 通用的模式切换
     */
    struct Mode
    {
        std::string usb_net_conn = "usb"; // 连接方式，usb/net
        int32_t dsp = 32;  //dsp work mode:    3  ==> test mode,  2==> cv mode;
        std::string dsp_bin_dir = "/root";  // dsp bin directory of DP1000
        int32_t log = 0; // 0 : dev->host, 1 : std::cout/print, 2 : file,  3 : syslog
        int32_t profile_enable = 0;  //是否默认启动device侧的resource profiler功能
    };


    /**
     * @brief 媒体相关的参数
     */
    struct MultiMedia
    {
        int32_t reserved;
    };

    /**
     * @brief 连接相关的参数
     */
    struct Connect
    {
        std::string ip = "127.0.0.1";
        std::string netmask = "";
        std::string gateway = "";

        //获取ip地址的方式。
        //auto: 自动获取本机ip，并忽略config文件中的connect.ip字段中的值
        //manual: 使用config文件中connect.ip字段中的值，并设置为本机ip
        std::string ip_mode = "auto";

        int32_t port = 9200;
    };

    struct DspLogMode
    {
        int32_t save_mode = 0;
        int32_t size = 1024*1024;
    };


    /**
     * @brief 存储相关的参数
     */
    struct Memory
    {
    };

    /**
     * @brief Aie相关的参数
     */
    struct AiEng
    {
        // 正常版本默认为 1
        int32_t enable=1;    //AIE使能，1：打开；0：关闭,  
        int32_t sc_dsp0_thresh = 32; //AIE dsp0 流控门限
        int32_t sc_dsp1_thresh = 32; //AIE dsp1 流控门限
        int32_t aidrv_mode = 1;//Aidrv收消息模式 0:poll,1:query
    };


    /**
     * @brief CV配置相关的参数
     */
    struct CVEng
    {
        int32_t enable=1;    //CV使能，1：打开；0：关闭
        int32_t aidrv_mode = 0;//Aidrv收消息模式 0:poll,1:query
        int32_t reserved;
    };


    SDKCfg();
    ~SDKCfg();
    static SDKCfg* Global();

    /**
     * @brief 从文件读取配置文件，更新对应的参数
     * @param json_file
     * @note
     * 1. 如果配置文件无对应的配置参数，那么使用缺省值；
     * 2. 如果配置参数携带的值超出范围，那么打印Warning，并使用缺省值；
     * 3. 如果配置参数携带的key未找到，那么异常退出；
     */
    void ReadFile(const std::string& json_file);

    void WriteFile(const std::string& json_file);


    /**
     * @brief 命令行解析出来的参数配置
     *
     * @param pattern
     * @note 命令行中携带 -D key:val,
     *
     */
    void ReadCmdLine(const std::string& pattern);
    void ReadCmdLine(const std::vector<std::string>& patterns);

    /**
     * @brief 从prop中更新配置参数
     */
    void UpdateConfig(void);

    /**
     * @brief 注册读取配置文件后的回调
     */
    void SetCfgCallback(CfgCBFunc func);

    LogLevel log;
    Mode mode;
    MultiMedia mulMedia;
    Connect connect;
    Memory memory;
    DspLogMode dspLogMode;
    AiEng aiEng;
    CVEng cvEng;
    Prop prop;

private:
    std::vector<CfgCBFunc> cb_funcs_;
};
}


#endif

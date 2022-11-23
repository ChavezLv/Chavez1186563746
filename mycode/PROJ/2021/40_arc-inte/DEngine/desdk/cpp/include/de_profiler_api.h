/**
 * @file de_profiler_api.h
 * @brief 获取芯片侧资源使用情况数据结构及api声明
 * @author ma.dengyun@intellif.com
 * @version 1.0.0
 * @date 2020/09/17
 * @copyright Intellif
 */

#ifndef DE_PROFILER_API_H_H_
#define DE_PROFILER_API_H_H_

#include <core/de_c_type.h>

namespace de {

// gcc + 32位系统、gcc + 64位系统、vs的结构体对齐策略都不一样，所以统一每个结构体的大小为8字节的整数倍，简化代码
// 注意：结构体/类的成员赋初始值之后，就不再是POD类型的了

///< NNP调度率
struct NNPSchedRate
{
    //单位：百分比
    double period_sched_rate;   //最近一个统计周期内的调度率
    double tot_sched_rate;      //总调度率
};

///< DDR带宽
struct BandwidthItem
{
    //单位：GB/s
    float r_bw;
    float w_bw;
    float rw_bw;
    
    //对齐到8字节整数倍
    float reserved;
};
struct DDRBandwidth
{
    struct BandwidthItem total;
    struct BandwidthItem port[5];
};

///< FastMallo内存使用
struct FastMallocMem
{
    //单位：MB
    float total_size;   //总大小
    float used_size;    //已使用大小
    float free_size;    //空闲大小

    //对齐到8字节整数倍
    float reserved;
};

///< CPU使用率
struct CPUUsage
{
    //单位：百分比，例如12.3表示CPU使用率为12.3%
    double cpu_percent;     //最近一个统计周期内的cpu使用率
};

struct ResourceUsageInfo
{
    static const int kNnpNumber = 4;

    NNPSchedRate nnp_shced_rate[kNnpNumber];
    DDRBandwidth ddr_bandwidth;
    FastMallocMem fast_malloc_mem;
    CPUUsage cpu_usage;
};

/**
 * @brief 通知device开始统计资源使用情况
 * @param dev_id device id
 */
DE_DLL void StartDevResProfile(int dev_id);

/**
 * @brief 获取芯片侧CPU使用率
 * @param dev_id device id
 * @return 第dev_id个设备上的资源使用情况
 */
DE_DLL ResourceUsageInfo GetDevResUseInfo(int dev_id);

/**
 * @brief 通知device停止统计资源使用情况
 * @param dev_id device id
 */
DE_DLL void StopDevResProfile(int dev_id);

}   //end of namespace de

#endif

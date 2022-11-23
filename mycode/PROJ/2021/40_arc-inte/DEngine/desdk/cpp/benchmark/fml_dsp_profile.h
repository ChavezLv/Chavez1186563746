/**
 * @file 
 * @brief 
 * @author 
 * @version 1.0.0
 * @date 2019/05/28
 * @copyright Intellifusion Corp.
 */

#ifndef FML_DSP_PROFILE_H
#define FML_DSP_PROFILE_H
#include <vector>
#include <cstdint>
#include <core/de_serializer.h>
#include <iostream>

namespace de {
namespace profile {    

    enum DspProfileType{
        kNNP0 = 0,
        kNNP1 = 1,
        kNNP2 = 2,
        kNNP3 = 3,
        kCrgh = 4,
        kCrghNnp0 = 5,
        kCrghNnp1 = 6,
        kCrghFft = 7,
        kBoxdma = 8,

        kSchedRateMax,
    };

    struct DspProfileInfo {
        int 	 type;
        int      reserved;
        uint64_t period_start_cycle;
        uint64_t period_end_cycle;
        double	 period_sched_rate;
        uint64_t tot_start_cycle;
        uint64_t tot_end_cycle;
        double	 tot_sched_rate;
    };

    class DE_DLL DspProfile {
    public:
        

        DspProfile();
        ~DspProfile();

        /**@brief 打开dsp profile功能
        * @param[in]	type_bit_mask  指定要打开的设备
        * @return  	无
        */
        void Open(int type_bit_mask=0xf, uint32_t dsp_id=0, uint64_t peroid=1000);

        /**@brief 关闭dsp profile功能
        * @param	无
        * @return  	无
        */
        void Close(int type_bit_mask=0xf, uint32_t dsp_id=0);

        /**@brief 关闭dsp profile功能
        * @param[in]   type_bit_mask  指定某个类型的硬件链路，该接口指定一个链路，通过bit mask实现，每个bit位表示的链路参见enum DspProfileType定义 
        * @return  	profile_info   返回的dsp profile信息
        */
        DspProfileInfo GetProfile(int type, uint32_t dsp_id=0);

        /**@brief 关闭dsp profile功能
        * @param[in]   type_bit_mask   指定某个类型的硬件链路，该接口可以同时指定多个，通过bit mask实现，每个bit位表示的链路参见enum DspProfileType定义
        * @return  	v_profile_info  返回的dsp profile信息，如果指定获取单个链路的信息，vector的size为1
        */
        std::vector<DspProfileInfo> GetMultiProfile(int type_bit_mask=0xf, uint32_t dsp_id=0);

        
    private:
    };

    inline std::ostream& operator<<(std::ostream& os, const DspProfileInfo& profile){
        os    <<   " hwlink:  type " << profile.type
                <<    " period:  [ "  << profile.period_start_cycle << ", " << profile.period_end_cycle << ", " << profile.period_sched_rate << " ]"
                <<    " total: [ " << profile.tot_start_cycle << ", " << profile.tot_end_cycle << ", " << profile.tot_sched_rate << " ]";

        return os;
    }   

} // namespace profile
} // namespace de



namespace de
{
namespace serializer
{

STRUCT_SERIALIZE_8(de::profile::DspProfileInfo,
	   int32_t,  type,
	   int32_t,  reserved,
	   uint64_t, period_start_cycle,
	   uint64_t, period_end_cycle,
	   double,   period_sched_rate,
	   uint64_t, tot_start_cycle,
	   uint64_t, tot_end_cycle,
	   double,   tot_sched_rate);

} // namespace serializer
} // namespace de


#endif 

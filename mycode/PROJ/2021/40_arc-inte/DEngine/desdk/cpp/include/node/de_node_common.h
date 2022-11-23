/**
 * @file de_node_common.h
 * @brief desdk标准node通用定义
 * @author DeSdk Team
 * @date 2020/5/21
 * @copyright Intellif
 */

#ifndef DE_NODE_COMMON_H
#define DE_NODE_COMMON_H

#include <core/de_serializer.h>
#include <core/de_thread.h>


namespace de
{

using FTaskSer = std::function<NDArray(void* task)>;
using FTaskDeser = std::function<void*(NDArray)>;
using FTaskDeleter = std::function<void(TaskQueue::Task)>;

struct ImageTask{
  uint32_t stream_id;  ///< stream id used for multi-stream
  uint32_t frame_id;
  uint64_t pts;       ///< timestamp       
  std::vector<de::NDArray> tensors;      
};

/*namespace serializer {
STRUCT_SERIALIZE_3(NnJpegRslt,
                   uint32_t, stream_id,
                   uint64_t, pts,
                   std::vector<de::NDArray>, tensors);*/
/* frame status */
enum FrameStatus{
    FRAME_OK = 0,          ///< frame is normal
	FRAME_EOS = 1,         ///< no frame, end of stream now
	FRAME_TIMEOUT = 2,     ///< no frame, get frame time out
};


/*the task is passed between multimedia operators*/
typedef struct{
    NDArray	 array;	                   ///< data object
    int32_t  streamid;                 ///< stream id used for multi-stream
    uint64_t pts;	                   ///< timestamp
    uint32_t frameid;                  ///< frame id
    uint32_t flag;                     ///< frame status indicator, see enum FrameStatus
    std::vector<uint8_t> baredata;     ///< used to store user defined data

    inline de::NDArray Serialize() {
		int32_t *ptr = static_cast<int32_t*>(array.GetBearData());
		ptr[0] = streamid;
		ptr[1] = pts & 0xFFFFFFFF;
		ptr[2] = (pts >> 32) & 0xFFFFFFFF;
		ptr[3] = frameid;
        ptr[4] = flag;

		int seq_num = 5;
		int baredata_size = 0;
		int space_size = static_cast<int>(array.GetBearDataCap()) - seq_num*sizeof(int32_t);
		if (space_size > 0 && baredata.size() > 0) {
			baredata_size = std::min(static_cast<int>(baredata.size()), space_size);
			memcpy((uint8_t*)&ptr[seq_num],  baredata.data(), baredata_size);
		}

		std::vector<int64_t> shapes; 
		array.Shrink(shapes, baredata_size + seq_num*sizeof(int32_t));
		return array;
    }

    inline void DeSerialize(de::NDArray data) {
		int32_t *ptr = static_cast<int32_t*>(data.GetBearData());
		streamid = ptr[0];
		pts = (((int64_t)ptr[2])<<32) | ((int64_t)ptr[1] & 0xFFFFFFFF);
		frameid = ptr[3];
        flag = ptr[4];
		int seq_num = 5;

		int baredata_size = static_cast<int>(data.GetBearDataSize()) - seq_num*sizeof(int32_t); 

		if (baredata_size > 0) {
			uint8_t* beardata_ptr = (uint8_t*)(&ptr[seq_num]);
			baredata = std::vector<uint8_t>(beardata_ptr, beardata_ptr + baredata_size);
		}
		
		array = data;
   }
}MediaTask;

namespace serializer {
STRUCT_SERIALIZE_6(MediaTask,
	NDArray, array,
	int32_t, streamid,
	uint64_t, pts,
	uint32_t, frameid,
	uint32_t, flag,
	std::vector<uint8_t>, baredata);
}

inline std::ostream& operator<<(std::ostream& os, const MediaTask& task)																														   
{
	os << "MediaTask:\n";
	os << "streamid=" << task.streamid << ", pts=" << task.pts << ", frameid=" << task.frameid 
	   << ", flag=" << task.flag << std::endl;
    os << task.array;
	
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const MediaTask* task)																														   
{
	os << "MediaTask:\n";
	os << "streamid=" << task->streamid << ", pts=" << task->pts << ", frameid=" << task->frameid 
	   << ", flag=" << task->flag << std::endl;
    os << task->array;
	
	return os;
}




}//end of namespace de

#endif





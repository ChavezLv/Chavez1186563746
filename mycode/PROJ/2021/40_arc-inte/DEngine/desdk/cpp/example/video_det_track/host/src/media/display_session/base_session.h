#ifndef IF_SESSION_H
#define IF_SESSION_H
#include "de_serializer.h"
#include "stdint.h"
#include <string.h>
#include "if_queue.hpp"
#include "ws_c_api.h"


#define IF_MSG_MAGIC	0x5ABC	///< 通信消息标识
#define host_from_array(T, data) de::fromByteArray<T>(data)

#define host_to_array(T, obj) { de::UserDataBinaryInStream stream = std::move(de::toByteArray<T>(obj)); DEByteArray byte_array = stream; std::string str(byte_array.data, byte_array.size);}

typedef enum {
    E_CLIENT = 0,
    E_SERVER = 1
}SessionType;

enum CfgMsg{
  CFG_UPDATE_NOTICE = 1,
  CFG_UPDATE_NOTICE_RSP =2
};

enum MediaMsg{
  MEDIA_SRC_URL = 1,
  MEDIA_NEW_URL = 2,
  MEDIA_SYNC_OFFSET = 4,
  MEDIA_FRAME_INFO = 5,
  MEDIA_FRAME_DATA = 6
};

typedef struct {
	uint16_t magic;										///< 消息标识, IF_COM_MSG_MAGIC
	int16_t state;										///< 消息处理状态，0表示成功，其他表示If_Com_ErrNo错误码
	uint8_t rsvd[4]; 									///< 保留字段
	uint16_t sequence;									///< 消息流水号
	uint16_t msg_id;									///< 消息id。非ack消息:奇数,ack消息:偶数
	uint32_t len;										///< 消息体长度
}IfMsgHeader;

typedef struct {
  int left; ///< 矩形最左边的坐标
  int top;  ///< 矩形最上边的坐标
  int right;  ///< 矩形最右边的坐标
  int bottom; ///< 矩形最下边的坐标
} IfWsRect;

typedef struct 
{
	unsigned int track_id;  //跟踪id
	IfWsRect rect;	//目标框位置
	std::string attrs; //属性文本描述
	int type;//目标类型
} IfWsObj;

typedef struct 
{
	int type; //目标类型
	unsigned int frame_id; //图像帧号
	unsigned long time_stamp; //时间戳
	unsigned long delay; 
    std::vector<IfWsObj> objs;
} IfWsFrame;

namespace de {
	namespace serializer {
		STRUCT_SERIALIZE_4(IfWsRect,
			int, left,
			int, top,
			int, right,
			int, bottom);

		STRUCT_SERIALIZE_4(IfWsObj,
			unsigned int, track_id,
			IfWsRect, rect,
			std::string, attrs,
			int, type);

		STRUCT_SERIALIZE_5(IfWsFrame,
			int, type,
			unsigned int, frame_id,
			unsigned long, time_stamp,
			unsigned long, delay,
			std::vector<IfWsObj>, objs);
	}
};

typedef struct
{
  int stream_type;
  int frame_size;                 /* frame size */
	uint64_t ts; 					          /* time stamp */
	int width;                      /* frame resolution width */
  int height;                     /* frame resolution height */
  uint8_t frame_data[0];            /* frame data pointer */
} IfWsFrameData;


typedef struct
{
  IfMsgHeader header;
  std::shared_ptr<uint8_t> buffer;
}MsgPacket;

class MsgQueuePredict;

class MsgQueue : public IfQueue<MsgPacket> {
 public:
  friend class MsgQueuePredict;
  MsgQueue() {
    //pred_ = std::bind(&MsgQueue::predicate, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
  }
  ~MsgQueue() {}

  bool wait_for(uint16_t msg_id, uint16_t seq, MsgPacket &packet, int timeout_ms = 5000);

 private:
  //std::function<bool(uint16_t, uint16_t, MsgPacket&)> pred_;
};

class MsgQueuePredict {
  public:
   MsgQueuePredict(uint16_t msg_id, uint16_t seq, MsgPacket &packet, MsgQueue &msg_queue) 
    : msg_id_(msg_id), seq_(seq), packet_(packet), msg_queue_(msg_queue) {}

  static std::chrono::time_point<std::chrono::steady_clock> g_begin;
  static uint64_t g_duration;
  static uint16_t g_msg_id;
  static uint16_t g_seq;

   bool predicate() {
    for (auto iter = msg_queue_.queue_.begin(); iter != msg_queue_.queue_.end();) {
      if ((iter->header.msg_id == msg_id_) && (iter->header.sequence == seq_)) {
        packet_ = *iter;
        iter = msg_queue_.queue_.erase(iter);
        return true;
      }else if (abs(seq_ - iter->header.sequence) > 100) {
        printf("delete dead message id %d, seq %d, cur seq %d\n", iter->header.msg_id, iter->header.sequence, seq_);
        iter = msg_queue_.queue_.erase(iter); //清除死数据
      }else {
        iter++;
      }
    }
    g_begin = std::chrono::steady_clock::now();
    g_msg_id = msg_id_;
    g_seq = seq_;
    printf("MsgQueuePredict msg_id %d, seq %d\n", msg_id_, seq_);
    return false;
  }

  inline bool operator()() {
    return predicate();
  }

  private:
   uint16_t msg_id_;
   uint16_t seq_;
   MsgPacket &packet_;
   MsgQueue &msg_queue_;
};


class BaseSession {
public:
    BaseSession () {   }

    virtual void on_msg(const void *data, int len) = 0;

    int session_id(){return m_session_id;}

    int send_msg(uint16_t msg_id, uint16_t &seq, int data_len = 0, void *data = NULL, int ext_len = 0, void *ext_data = NULL) {
        IfMsgHeader header;
        header.magic = IF_MSG_MAGIC;
        header.msg_id = msg_id;
        if (seq == 0)
          header.sequence = m_sequence_++;
        else
          header.sequence = seq;
        header.len = data_len + ext_len;
        seq = header.sequence;
        header.state = 0;
        int len = sizeof(header) + data_len + ext_len;

        //std::chrono::time_point<std::chrono::steady_clock> begin = std::chrono::steady_clock::now();
        //uint64_t duration = 0;
		
		// timespec time1;
	    // clock_gettime(CLOCK_MONOTONIC,&time1);
	    // uint64_t sendtime = time1.tv_sec*1000000000 + time1.tv_nsec; 
		// sendtime=sendtime/1000000;
		// printf("===sendtime %ld seq %d\n",sendtime,seq);
        
        uint8_t *buf;
        buf = new uint8_t[len];
        if (!buf) return -1;
        memcpy(buf, &header, sizeof(header));
        if (data) memcpy(buf + sizeof(header), data, data_len);
        if (ext_data) memcpy(buf + sizeof(header) + data_len, ext_data, ext_len);
        //printf("[%d %d] send_msg id %d, data_len %d, ext_len %d, sequence %u\n", m_session_type,m_session_id, header.msg_id, header.len, ext_len, header.sequence);
        //duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() -begin).count();
        int ret = 0;
        if(m_session_type == E_CLIENT){
            if (ret = (int)if_ws_client_send(m_session_id, (void*)buf, len)) {
                if (buf) delete[] buf;
                return ret;
            }
        }
        else if(m_session_type == E_SERVER){
            if (ret = (int)if_ws_server_send(m_session_id, (void*)buf, len)) {
                if (buf) delete[] buf;
                return ret;
            }
        }
        //printf("send_msg send ok\n");
        if (buf) delete[] buf;
        
        //duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() -begin).count();
        //printf("send_msg elapsed time = %lld ms\n", duration);
        return 0;
    }
  
protected:
    SessionType m_session_type;
    int m_session_id; 
    std::atomic<uint16_t> m_sequence_;

};


#endif //IF_SESSION_H

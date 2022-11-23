#ifndef IF_MEDIA_SESSION_H
#define IF_MEDIA_SESSION_H
#include "base_session.h"
#include "if_log.h"
#include "if_queue.hpp"
#include "if_common.hpp"
#include "media_display.hpp"
#include "de_serializer.h"


extern IfQueue<SPImage> g_disp_queue;
extern std::mutex mtx_session_map;

namespace intellif {
class MediaSession;
extern std::map<int, MediaSession*> media_session_map;

class MediaSession : public BaseSession {
public:
    MediaSession(int session_id, SessionType type) {
        m_session_id = session_id;
        m_session_type = type;
    }

    int src_frame_send(uint8_t* frame_data, int frame_size, int width, int height, uint64_t ts, int stream_type){
      uint16_t seq = 0;
      IfWsFrameData req;
      req.frame_size = frame_size;
      req.height = height;
      req.width = width;
      req.ts = ts;
	  req.stream_type = stream_type;
      std::unique_lock<std::mutex> lock(mutex_send_);
      int ret;
      if (ret = send_msg(MEDIA_FRAME_DATA, seq, sizeof(req), &req, req.frame_size, frame_data)){
          DEMO_LOG_ERROR("send_msg failed with errorcode %d\n", ret);
          return ret;
      }
      
      return 0;
    }
	
	int frame_send(IfWsFrame& frame) {
        de::UserDataBinaryInStream stream = std::move(de::toByteArray(frame)); 
        DEByteArray byte_array = stream; 
        std::string data(byte_array.data, byte_array.size) ;
        uint16_t seq = 0;
        std::unique_lock<std::mutex> lock(mutex_send_);
        int ret;
        if (ret = send_msg(MEDIA_FRAME_INFO, seq, data.size(), (void*)data.c_str())){
            DEMO_LOG_ERROR("send_msg failed with errorcode %d\n", ret);
            return ret;
        }
        return 0;
    }

    void on_msg(const void *data, int len) {
        void* pdata = const_cast<void *>(data);
        IfMsgHeader* header = reinterpret_cast<IfMsgHeader*>(pdata);
        if ((header->magic != IF_MSG_MAGIC) || (len < (sizeof(IfMsgHeader) + header->len))) {
          DEMO_LOG_ERROR("unknown message, magic %d, len %d, header_len %d\n", header->magic, len, header->len);
          return;
        }
		// timespec time1;
	    // clock_gettime(CLOCK_MONOTONIC,&time1);
	    // uint64_t recvtime = time1.tv_sec*1000000000 + time1.tv_nsec; 
		// recvtime/=1000000;
        // DEMO_LOG_INFO("on_msg recieve msgid %d len %d sequence %d recvtime %ld\n", header->msg_id, header->len, header->sequence,recvtime);
        uint8_t *msg_body = (uint8_t*)data + sizeof(IfMsgHeader);
        unsigned int body_len = header->len;

        if (body_len == 0) {
          DEMO_LOG_ERROR("media session msg id %d, len 0!\n", header->msg_id);
          return;
        }

        if (header->msg_id == MEDIA_FRAME_DATA) {
          IfWsFrameData *frame_data = (IfWsFrameData*)msg_body;
          if_image_t image;
          image.time_stamp = frame_data->ts;
          image.format = frame_data->stream_type;
          image.width = frame_data->width;
          image.height = frame_data->height;
          image.size = frame_data->frame_size;
		  image.extra_data[0] = nullptr;
          image.extra_data[1] = nullptr;
		  if (frame_data->stream_type == DE_PIX_FMT_MJPEG){
			std::vector<char> vecImg((char*)(frame_data->frame_data), (char*)(frame_data->frame_data)+image.size);
			// static FILE *file = fopen("tmp333.jpg", "w");
			// fwrite((char*)&vecImg[0], vecImg.size(), 1, file);
			cv::Mat mat = cv::imdecode(vecImg, CV_LOAD_IMAGE_COLOR);
			image.size = mat.elemSize() * mat.rows * mat.cols;
			image.data = new uint8_t[image.size];
			image.width = mat.cols;
			image.height = mat.rows;
			image.format = IF_IMG_BGR888;
			memcpy(image.data, mat.data, image.size);
		  }else{
          image.data = new uint8_t[image.size];
          memcpy(image.data, frame_data->frame_data, image.size);
          }
          SPImage image_ptr(new Image(image));
          disp_obj_.feed_frame(image_ptr);
          DEMO_LOG_INFO("frame data: frame_size %d, timestamp %ld\n", image.size, image.time_stamp );

        }else if (header->msg_id == MEDIA_FRAME_INFO) {
			      std::string str((char*)msg_body,body_len);
            IfWsFrame frame = host_from_array(IfWsFrame, str);
            DisplayInfo data;
            ObjectDisplayInfo info;
            data.time_stamp = frame.time_stamp; 
			data.frame_id = frame.frame_id;
			data.trk_delay = frame.delay;
            int obj_num = frame.objs.size();
            DEMO_LOG_INFO("frame info: frametype %d, frameid %d, timestamp %ld, obj_num %d\n", frame.type, frame.frame_id, frame.time_stamp, obj_num);
            for (int i = 0; i < obj_num; i++) {
              //printf("[%s %d %d %d %d %d]\n",frame.obj[i].attrs.c_str(), frame.obj[i].track_id, frame.obj[i].rect.left,frame.obj[i].rect.top,frame.obj[i].rect.right,frame.obj[i].rect.bottom);
              info.rect.left = frame.objs[i].rect.left;
              info.rect.right = frame.objs[i].rect.right;
              info.rect.top = frame.objs[i].rect.top;
              info.rect.bottom = frame.objs[i].rect.bottom;
              info.side_text = frame.objs[i].attrs.c_str();
              info.top_text = "tid:" + std::to_string(frame.objs[i].track_id);
              data.obj_infos.emplace_back(info);
            }
            disp_obj_.feed_data(data);
 
        }else{
          DEMO_LOG_ERROR("unsupported msgid %d!\n",header->msg_id);
        }
    }
        
private: 
	MediaDisplay disp_obj_;
  std::mutex mutex_send_;
};

} //namespace intellif

#endif //IF_MEDIA_SESSION_H

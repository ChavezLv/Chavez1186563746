/**
 * @file 
 * @brief 
 * @author 
 * @version 1.0.0
 * @date 2019/02/25
 * @copyright Intellifusion Corp.
 */

#ifndef DE_VIDEO_DECODER_NODE_H_
#define DE_VIDEO_DECODER_NODE_H_

#include <core/de_thread.h>
#include "de_video_in_node.h"
#include <string>
#include <pthread.h>

namespace de{
  
/**
* @brief namespace for multimedia operator, such as video capturer, video codecs ...
*/
namespace mm{
      
/**
* @brief a struct for showing video decoder status 
*/
struct DecoderStatus{
    int total_bufs;  ///< total output bffers, set by user
    int used_bufs;   ///< numbers of output buffers used
    int run_status;  ///< decoder running status: 0 - idle, 1 - running, 2 - timeout, 3 - eos           
};

/**
* @brief video stream decoder node, support h264/h265
* the property of the node as follows:  \n
*           "stream-type" - stream type, see enum PixelFormat in de_type.h. \n
*           "output-buffers" - decoder output buffer numbers, 4-64, default: 8  \n
*           "in-timeout" - capture frame timeout time,  <=0 is unlimited,  default: 0 unit:second \n
*           "out-timeout" - capture frame timeout time,  <=0 is unlimited, default: 3s unit:second \n
*           "fps-ctrl" - adaptive frame rate control, 0-disabled(default)  1-enabled \n
*           "fps-ctrl-start-bufs" - so much output buffs in using, frame rate control start to work \n
*           "blank-frames" - when stream end, send specify number blank frame, default is 0 \n
*   
* @note usage: \n
*      auto dec = new VideoDecoder;  \n
*      TaskQueue* out_q = new TaskQueue(16, TaskQueue::MISO);  \n
*          dec->Set<int>("stream-type", DE_PIX_FMT_H264); \n
*          dec->UpdatePara();  \n
*          dec->pout[0].SetQueue(out_q);  \n
*          dec->pout[0].SetTaskTypeId(1);  \n
*      int ret = dec->Start();
*           \n
*      JpegDecTask* task_in = new VideoDecTask;  \n
*      fill the task ... ...  \n
*      TaskQueue* in_q = dec->pin.GetQueue();  \n
*          in_q->Push(task_in, 0);  \n
*           \n
*      TaskQueue::Task job;  \n
*      out_q->Pop(&job, 0);   \n
*           \n
*      do something else ... ...   \n
*           \n
*          VideoDecTask* task_out = static_cast<VideoDecTask*>(job.task);  \n
*          delete task_out;  \n
*           \n
*      dec->Stop();   \n
*           \n
*          dec->pin.DestroyQueue<VideoDecTask>();  \n
*          out_q->SignalForKill<StreamTask>();   \n
*          delete in_q;  \n
*      delete dec;   \n     
*          delete out_q; \n
*
*/
class DE_DLL VideoDecoder : public Thread{
private:      
    void* dec_ = nullptr;                 ///< hal video decoder 
    int strm_fmt_ = DE_PIX_FMT_H264;      ///< stream format, see PixelFormat in hal_mm_video_common.h
    pthread_t pth_out_;                   ///< internal frame ouput thread
    int out_stop_ = 0;                    ///< frame out thread stop flag
    int stream_id_ = 0;                   ///< stream id 
    uint32_t frame_id_ = 0;               ///< frame id
    int in_timeout_ = 0;                  ///< input timeout unit:second <=0 is unlimited
    int out_timeout_ = 4;                 ///< output timeout unit:second <=0 is unlimited
    NDArray blank_frame_;                 ///< store blank frame
    int blank_cnt_ = 0;                   ///< blank frame send counter

public:    
    uint32_t strm_cnt_[2]={0,0};           ///< for debug, [1]- in counter, [0]-free counter

private:
    /**
    *  @brief get a decoded frame
        *
    * @param handle   frame data
    * @param timeout  waiting time, unit: second
    *
    * @return
    *  @retval  0 - ok
    *  @retval  1 - end of frame
    *  @retval  2 - timeout
    */
    int GetFrame(NDArray& frame, int timeout = 4); 
    void SendFrame(NDArray& frame, uint32_t flag);
    void SendBlankFrames(uint32_t flag);
    void SendException(uint32_t flag);
    void CreateBlankFrame(NDArray& frame);
    uint32_t MinOutDepth();
    uint32_t MaxOutDepth();

    static void* FrameOut(void* udata);
    static void FrameFree(NDArray::Container* self);
    static void StreamFree(void* udata);

protected:
    /**
    * jpeg encoder process function on async mode
    *
    * @param  rx_task       task object in input queue
    * @param  task_type     task type
    *
    * @return 
    */
    virtual void Proc(void* rx_task, int32_t task_type, POutType pout_type = DEFAULT_POUT) override;

public:     
    VideoDecoder(); 
    VideoDecoder(const VideoDecoder& dec) = delete;
    VideoDecoder& operator=(const VideoDecoder& dec) = delete;
    ~VideoDecoder();

    /**
    * @brief Open a video deocder with specified property
    * The decoder will start receiving video stream and decoding
    *
    * @param
    * @return     
    *
    */
    virtual void Start() override;


    /**
    * @brief Close a video decoder
    * The decoder will stop receiving video stream and sending decoded frame, 
    * Resources hold by the decoder will be released.
    *
    * @param
    * @return
    */
    virtual void Stop() override;

    /**
    * @brief get status of a video decoder  
    *
    * @param sts  decoder status, see struct DecoderStatus
    *
    * @return
    */
    void GetStatus(struct DecoderStatus& sts);

    /**
    * @brief get video stream id
    *
    * @param
    *
    * @return  stream id
    */
    int GetStreamId() { return stream_id_; }

    /**
    * @brief get video frame id, it increase Monotonously
    *
    * @param
    *
    * @return  frame id
    */
    uint32_t GetFrameId() { return frame_id_; }

    /**
    * @brief increase video frame id
    *
    * @param
    *
    * @return 
    */
    void IncrFrameId() { frame_id_++; }
      
};
  
} //namespace mm
} //namespace de


#endif // DE_VIDEO_DECODER_NODE_H_


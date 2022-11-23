/**
 * @file 
 * @brief 
 * @author 
 * @version 1.0.0
 * @date 2019/02/25
 * @copyright Intellifusion Corp.
 */

#ifndef DE_VIDEO_IN_NODE_H_
#define DE_VIDEO_IN_NODE_H_

#include <string>
#include <cstdio>

#include "core/de_thread.h"
#include "core/de_serializer.h"

namespace de{

/**
* @brief namespace for multimedia operator, such as video capturer, video codecs ...
*/
namespace mm{
  
/**
* @brief video capturer node
* it can get video stream from webcarmera/usbcamera/mipi/bt/local file   \n
* the property of the node as follows:  \n
*            for v4l2 video device:  \n
*                 "width" - set stream source resolution's width value.  \n
*                 "height" - set stream source resolution's height value.  \n
*                 "framerate" - set stream source rate vaule.    \n 
*                 "test-pattern" - color bar mode  \n 
*
*            for web camera:  \n
*                 "lower-trans-type" - set rtsp transport type(for tcp/udp/upd_mcast default:udp),   \n
*                                      see "enum de_RtspTransport".   \n
*            for all:   \n
*                 "stream-type" - select stream source type, see enum PixelFormat in de_type.h.   \n
*                 "uri" - The location of the stream to be opened, which can be from an Web Camera/USB Camera/Local file
*                         /other v4l2 devices. \n
*                         The parameter complies with uri specification，and it uses format as follows: \n
*                         "rtsp://login:passwd@the.ipc.ip.addr" : specifies a rtsp stream location.  \n
*                         "v4l2:///dev/videoX" : specifies a USB Camera or other v4l2 devices.  \n
*                         "file:///data/your_video.h264" : specifies a local stream file.  \n
*                 "timeout" - capture frame timeout time,  <=0 is unlimited, unit:second \n
*                 "sync-mode" - in this mode, pout is unusable. 0-disable(default) 1-enbale  \n
*
*  \n
*  \n
* @note  usage: \n
*            auto video_in = new VideoInput;   \n
*            string uri = "v4l2:///dev/video0"   \n
*                       or "rtsp://username:password@192.168.1.10:554"  \n
*                       or "file:///tmp/test.h264"  \n
*            int type = video_in->StreamType(uri);  \n
*            TaskQueue out_q = new TaskQueue(16, TaskQueue::MISO);  \n
*            video_in->pout[0].SetQueue(ptask->out_q);  \n
*            video_in->pout[0].SetTaskTypeId(1);  \n
*  \n
*            video_in->Set<int>("uri", uri);  \n
*            video_in->Set<int>("stream-id", 1);  \n
*            video_in->Set<int>("width", 1920);  \n
*            video_in->Set<int>("height", 1080);  \n
*            video_in->Set<double>("framerate", 25.0);  \n
*            video_in->Set<int>("stream-type", DE_PIX_FMT_H264);  \n
*  \n
*            int ret = video_in->Start();  \n
*  \n
*            TaskQueue::Task job;  \n
*            out_q->Pop(&job, 0);  \n
*  \n
*            do something else ... ...   \n
*  \n
*            StreamTask* task_out = static_cast<StreamTask*>(job.task);   \n
*            delete task_out;  \n
*  \n
*            out_q->SignalForKill<StreamTask>();     \n         
*            video_in->Stop();  \n
*            delete video_in;  \n
*            delete(out_q);    \n
*
*/  
class DE_DLL VideoInput  : public Thread{
private:
    void* stream_ = nullptr;             ///< videoInput hal layer handle
    pthread_t pth_out_;                  ///< internal stream ouput thread
    int stream_id_ = 0;                  ///< stream id set by user
    int sync_mode_ = 0;                  ///< sync mode
    int mode_ = 0;
    bool out_stop_ = false;
    uint8_t gain_ = 0x20;
	int save_stream_ = 0;

    /** available when stream_save_ != 0**/
	FILE* save_fp_ = nullptr;
	uint32_t file_idx_ = 0;
	bool opened_ = false;
	uint32_t frames_per_file_ = 0;
	
 
    /**
    * @brief  get a raw stream, encoded or uncoded
    *
    * @param stream  stream data
    * @param timeout  waiting time, unit: second
    *
    * @return
    *  @retval  0 - ok, 
    *  @retval  1 - end of stream
    *  @retval  2 - timeout
    */
    int GetStream(NDArray& stream, int timeout = 3);
    void SendStream(NDArray& stream, uint32_t flag);
	void strm_file_save(void *buf,  size_t size, uint32_t dec_idx);
	void strm_file_close();

    static void* StreamOut(void* udata);
    static void FrameFree(NDArray::Container* self);


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
    VideoInput();
    ~VideoInput()
    {
        LOG(INFO) << "class VideoInput instance destoryed !!!";
    }

    /**
    * @brief  Start capture stream
    *
    * @param 
    *
    * @return     
    *        
    */  
    virtual void Start() override;

    /**
    * @brief Stop capture stream  and release resource
    *
    * @param
    * @return
    */
    virtual void Stop() override;

    /**
    * @brief get video stream id
    *
    * @param
    *
    * @return  stream_id_
    */
    int GetStreamId(){return stream_id_;}

    /**
    * @brief Get stream type 
    * It will be used if you don't know about stream source format. 
    * 
    * @param uri see Open()comment
    *
    * @return 
    *  @retval stream type, see enum PixelFormat in de_type.h
    *
    */
    static int StreamType(const std::string& uri); 

    /**
    * @brief set v4l2 device output mode in running
    *
    * @param mode - 0-normal(default), 
    *               1-3 used in dual channel mipi senario:
    *                   1-get primary channel frame
    *                   2-get secondary channel frame
    *                   3-get both channels frames
    *
    * @return 
    */
    void SetMode(uint8_t mode);

    /**
    * @brief set v4l2 sensor gain in running
    *
    * @param value  sensor gain value from 0-0x3f
    *
    * @return 
    */
    void SetGain(uint8_t value);

    /**
    * @brief  get a raw frame, encoded or uncoded
    * This api get frame from hal directly, do not pass through pout.
    *
    * @param frame   frame data
    * @param timeout  waiting time, unit: second
    *
    * @return
    *  @retval  0  - ok, 
    *  @retval  1  - end of stream
    *  @retval  2  - timeout
    *  @retval  -1 - invalid call, becasuse sync-mode is not enabled or videoinput is not opened
    */
    int GetFrame(NDArray& frame, int timeout = 3);

};
  
} //namespace mm

} //namespace de


#endif // DE_VIDEO_IN_NODE_H_


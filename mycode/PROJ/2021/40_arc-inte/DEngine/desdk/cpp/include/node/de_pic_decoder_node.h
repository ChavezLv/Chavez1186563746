/**
 * @file 
 * @brief 
 * @author 
 * @version 1.0.0
 * @date 2019/02/25
 * @copyright Intellifusion Corp.
 */

#ifndef DE_PIC_DECODER_H_
#define DE_PIC_DECODER_H_

#include <core/de_thread.h>

namespace de{

/**
* @brief namespace for multimedia operator, such as video capturer, video codecs ...
*/
namespace mm{

/**
* @brief jpeg decoder node
*
* @note usage: \n
*   1. Thread method \n
*      auto dec = new JpegDecoder; \n
*      TaskQueue* out_q = new TaskQueue(16, TaskQueue::MISO); \n
*          dec->UpdatePara(); \n
*          dec->pout[0].SetQueue(out_q); \n
*          dec->pout[0].SetTaskTypeId(1); \n
*      int ret = dec->Start(); \n
* \n
*      JpegDecTask* task_in = new JpegDecTask; \n
*      fill the task ... ... \n
*      TaskQueue* in_q = dec->pin.GetQueue(); \n
*          in_q->Push(task_in, 0); \n
* \n
*      TaskQueue::Task job; \n
*      out_q->Pop(&job, 0); \n
* \n
*      do something else ... ...  \n
* \n
*          VideoDecTask* task_out = static_cast<VideoDecTask*>(job.task);  \n
*          delete task_out;  \n
* \n
*      dec->Stop();  \n
*  \n         
*          dec->pin.DestroyQueue<VideoDecTask>(); \n
*          out_q->SignalForKill<StreamTask>(); \n
*          delete in_q; \n
*          delete dec;  \n      
*          delete out_q;  \n
* \n
* \n
*   2. solo method
*      int ret = JpegDecoder::JpegDecode(pic_in, img_out);  \n
*      
*/
class DE_DLL JpegDecoder : public Thread{
private:
    long dec_ = 0;    ///< jpeg decoder handle
    int errno_ = 0;   ///< jpeg erro number, see enum de_JpegErrorCode in hal_mm_jpeg.h
    uint32_t frame_id_ = 0;               ///< frame id

    static int Decode(long& dec, NDArray& pic_in, NDArray& img_out);
    void IncrFrameId() { frame_id_++; }

protected:
    /**
    * jpeg decoder process function on async mode
    *
    * @param  rx_task       task object in input queue
    * @param  task_type     task type
    *
    * @return 
    */
    virtual void Proc(void* rx_task, int32_t task_type, POutType pout_type = DEFAULT_POUT) override;

public:
    JpegDecoder();
    ~JpegDecoder();

    /**
    * @brief dp1000 hardware jpeg decoder
    * 
    * @param pic_in   jpeg picture input
    * @param img_out  decode image output
    *
    * @return see enum de_JpegErrorCode in hal_mm_jpeg.h
    *
    * @note  this api can be used independently on local if you do not
    *    want to use ASyncThread method
    */  
    static int JpegDecode(NDArray& pic_in, NDArray& img_out);

    /**
    * @brief get jpeg decoder error number when decode failed
    *
    * @return see enum de_JpegErrorCode in hal_mm_jpeg.h
    */
    int GetErrorno(){ return errno_;}

    /**
    * @brief Open the jpeg decoder and start decoding
    * Once the jpeg decoder is started, it starts getting jpeg picture from rx queue,
    * and decoding, and putting decoded image to tx queue.
    *
    * @param
    * @return 
    */
    virtual void Start() override;

    /**
    * @brief  Close the jpeg decoder
    * Once the jpeg decoder is closing, it stops getting jpeg picture and putting decoded image.
    * Resources hold by the decoder will be released.
    *
    * @param
    * @return
    */
    virtual void Stop() override;

    /**
    * @brief get image frame id, it increase Monotonously
    *
    * @param
    *
    * @return  frame id
    */
    uint32_t GetFrameId() { return frame_id_; }
  
};

} //namespace mm
} //namespace de

#endif // FML_PIC_DECODER_NODE_H_


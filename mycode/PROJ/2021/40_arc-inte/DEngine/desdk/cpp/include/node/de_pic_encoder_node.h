/**
 * @file 
 * @brief 
 * @author 
 * @version 1.0.0
 * @date 2019/02/25
 * @copyright Intellifusion Corp.
 */

#ifndef DE_PIC_ENCODER_NODE_H_
#define DE_PIC_ENCODER_NODE_H_

#include <core/de_thread.h>
#include <core/de_serializer.h>

namespace de{

/**
 * @brief namespace for multimedia operator, such as video capturer, video codecs ...
 */
namespace mm{ 

/**
* @brief jpeg encoder node
* the property of the node as follows:  \n
*     "quality" -  encode quantization parameter(0-10)
*     "out-size" - 0: encoder will auto set output buffer size; \n
*                  others: output buffer size specified by user \n
*     "preprocess" - hw coder has restriction on resolution, preprocess can adjust for hw. \n
*                    true: enable preprocess; false: disable \n
* \n
* @note usage:  \n
*       1. Thread method  \n
*      auto enc = new JpegEncoder;  \n
*      TaskQueue* out_q = new TaskQueue(16, TaskQueue::MISO);  \n
*      enc->Set<int>("quality", ENC_QUALITY_7);   \n
*      enc->UpdatePara();  \n
*      enc->pout[0].SetQueue(out_q);  \n
*      enc->pout[0].SetTaskTypeId(1);  \n
*      int ret = dec->Start();  \n
* \n
*      JpegEncTask* task_in = new JpegEncTask;  \n
*      fill the task ... ...
*      TaskQueue* in_q = enc->pin.GetQueue();   \n
*      in_q->Push(task_in, 0);  \n
* \n
*      TaskQueue::Task job;  \n
*      out_q->Pop(&job, 0);  \n
* \n
*      do something else ... ...
* \n
*      JpegEncTask* task_out = static_cast<JpegEncTask*>(job.task);  \n
*      delete task_out;  \n
* \n
*      enc->Stop();  \n
* \n       
*      enc->pin.DestroyQueue<VideoDecTask>();  \n
*      out_q->SignalForKill<StreamTask>();  \n
*      delete in_q;   \n
*      delete enc;    \n    
*      delete out_q;  \n
* \n
*       2. solo method  \n
*          int ret = JpegEncoder::JpegEncode(img_in, pic_out);  \n
*          
*/
class DE_DLL JpegEncoder : public Thread{
private:
	long enc_ = 0;             ///< jpeg encoder handle
    int errno_ = 0;            ///< jpeg erro number, see enum de_JpegErrorCode in hal_mm_jpeg.h
    int qp_ = 6;               ///< quantization parameter(0-10), see enum de_EncQuality in hal_mm_jpeg.h
    int out_size_ = 0;         ///< 0: encoder will auto set output buffer size
    bool preprocess_ = false;  ///< preprocess enable(true)/disable(false) 
          
    static int PreProcess(NDArray& img_in, NDArray& img_out);
	static int Encode(long& enc, int qp, NDArray& img_in, NDArray& pic_out, int out_size, bool pp_en);

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
    JpegEncoder();
    ~JpegEncoder();    
  
    /**
    *  dp1000 hardware jpeg encoder
    * 
    * @param qp       quantization parameter(0-10), 
    *                 see enum de_EncQuality in hal_mm_jpeg.h
    * @param img_in   image input
    * @param pic_out  jpeg picture output
    * @param out_size output buffer size, valid range: 0x10000 -- 0x3000000
    *                 othes valid makes encoder decide internal   
    * @param pp_en    preprocess enable
    *
    * @return  see enum de_JpegErrorCode in hal_mm_jpeg.h
    *
    * @note  this api can be used independently on local if you do not
    *        want to use ASyncThread method
    */  
    static int JpegEncode(int qp, NDArray& img_in, NDArray& pic_out, 
                             int out_size = 0, bool pp_en = 0);

    /**
    * get jpeg encoder error number when decode failed
    *
    * @param
    *
    * @return see enum de_JpegErrorCode in hal_mm_jpeg.h
    */
    int GetErrorno(){ return errno_;}

    /**
    * @brief Open the jpeg encoder and start encoding
    * Once the jpeg encoder is opened, it starts getting yuv picture from rx queue,
    * and decoding, and putting jpeg image to tx queue.
    *
    * @param
    * @return 
    */
    virtual void Start() override;

    /**
    * @brief  Close the jpeg encoder
    * Once the jpeg encoder is closing, it stops getting yuv picture and putting jpeg image.
    * Resources hold by the encoder will be released.
    *
    * @param
    * @return
    */
    virtual void Stop() override;
    
};
  
} //namespace mm
} //namespace de


#endif // DE_PIC_ENCODER_NODE_H_


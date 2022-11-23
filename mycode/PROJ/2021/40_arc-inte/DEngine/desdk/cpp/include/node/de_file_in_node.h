/**
 * @file 
 * @brief 
 * @author 
 * @version 1.0.0
 * @date 2020/10/20
 * @copyright Intellifusion Corp.
 */

#ifndef DE_file_in_NODE_H_
#define DE_file_in_NODE_H_

#include <string>
#include <fstream>

#include "core/de_thread.h"
#include "core/de_serializer.h"

namespace de{

/**
* @brief namespace for multimedia operator, such as video capturer, video codecs ...
*/
namespace mm{
  
/**
* @brief video file input node
* it can get video stream from single local file or local files in list file  \n
* the property of the node as follows:  \n
*          "size" - size of buffer for each time file reading, default 256KB.  \n
*          "width" - image width, used when input file is raw image.
*          "height" - image height, used when input file is raw image.
*          "stream-id" - stream id for distinguish different stream. \n
*          "framerate" - set output frame rate. \n 
*          "stream-type" - select stream source type, see enum PixelFormat in de_type.h. \n
*          "file" - local file name, example: file://./test.h264 or ./test.h264. \n
*          "file-list" - the file contains local file name list, each file name takes one line. \n
*                        file contents example: \n
*                          people.h264 \n
*                          car.h264 \n
*                          dog.h264 \n
*                        If the property is used, "file" property is ignored.\n
*          "timeout" - get stream timeout time,  <=0 is unlimited, unit:second \n
*          "start-index" - Start value of index.  When the end of one loop is reached, the index will be set to the value start-index. \n
*                          -1-disable start index function(default). It is only valid when "file-list" is set. \n
*          "stop-index" - Stop value of index. It is only valid when "file-list" is set. -1-disable start index function(default). \n
*          "loop" - loop count, 0-disable(default), >0-stop getting when reach the value, <0-unlimited loop. \n
*          "sync-mode" - in this mode, pout is unusable. 0-disable(default) 1-enbale  \n
*
*  \n
*  \n
* @note  usage: \n
*            auto file_in = new FileInput;   \n
*            TaskQueue out_q = new TaskQueue(16, TaskQueue::MISO);  \n
*            file_in->pout[0].SetQueue(ptask->out_q);  \n
*            file_in->pout[0].SetTaskTypeId(1);  \n
*  \n
*            file_in->Set<int>("stream-id", 1);  \n
*            file_in->Set<double>("framerate", 25.0);  \n
*            file_in->Set<string>("file", "streams.h264");  \n
*            file_in->Set<int>("start-index", 0);  \n
*            file_in->Set<int>("stop-index", 16);  \n
*            file_in->Set<int>("timeout", 3);  \n
*            file_in->Set<int>("loop", 100);  \n
*  \n
*            file_in->Start();  \n
*  \n
*            TaskQueue::Task job;  \n
*            out_q->Pop(&job, 0);  \n
*  \n
*            do something else ... ...   \n
*  \n
*            MediaTask* task_out = static_cast<MediaTask*>(job.task);   \n
*            delete task_out;  \n
*  \n
*            out_q->SignalForKill<MediaTask>();     \n         
*            file_in->Stop();  \n
*            delete file_in;  \n
*            delete(out_q);  \n
*
*/  
class DE_DLL FileInput  : public Thread{
private:
    void* stream_ = nullptr;             ///< FileInput hal layer handle
    std::thread strm_out_;               ///< internal stream ouput thread
    std::thread strm_in_;                ///< internal stream input thread
    int stream_id_ = 0;                  ///< stream id set by user
	int sync_mode_ = 0;                  ///< sync mode
	std::vector<std::string> flist_;     ///< file list
	std::string file_;                   ///< filename of input file or file list
	std::ifstream ifs_cur_;              ///< current file be reading
	bool flist_en_ = false;              ///< use file list as input
	int start_index_ = -1;               ///< begin of file list index
	int stop_index_ = -1;                ///< end of file list index
	int cur_index_ = 0;                  ///< current file index	
	int loop_cnt_ = 0;                   ///< loop counter
	int cur_loop_cnt_ = 0;               ///< current loop count
	bool in_stop_ = false;               ///< stream in stop flag
	int buf_size_ = 0x40000;             ///< buffer size for each time file reading    
 
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
    int GetFrameInternal(NDArray& stream, int timeout = 3);
    void SendStream(NDArray& stream, uint32_t flag);
	void CreateFileList();
	void GetNextFileSingle(std::string& file_name);
	void GetNextFileFromList(std::string& file_name);
	std::string GetNextFile();
	void Proc(void* rx_task, int32_t task_type, POutType pout_type);

    static void StreamOut(void* udata);
	static void StreamIn(void* udata);
    static void FrameFree(NDArray::Container* self);
	static void StreamFree(void* udata);

public:
    FileInput();
    ~FileInput()
    {
        LOG(INFO) << "class FileInput instance destoryed !!!";
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
    * @brief get stream data
    * user must not put data exceeding buf_size, and do not care about buffer address
    *
    * @param buf       store stream data, it is allocated by FileInput
    * @param buf_size  buffer size
    *
    * @return 0:end of file or read failed  >0: the number of bytes got
    */
	virtual int GetData(uint8_t* buf, int buf_size);

    /**
    * @brief get video stream id
    *
    * @param
    *
    * @return  stream_id_
    */
    int GetStreamId(){return stream_id_;}

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
    *  @retval  -1 - invalid call, becasuse sync-mode is not enabled or FileInput is not opened
    */
    int GetFrame(NDArray& frame, int timeout = 3);

};
  
} //namespace mm

} //namespace de


#endif // DE_file_in_NODE_H_


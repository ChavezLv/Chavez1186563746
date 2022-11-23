#pragma once

#include "infer_engine_common.h"
#include "de_pic_decoder_node.h"

namespace de {

class Receiver : de::Thread {
private:
  enum class TASK_TYPE : int32_t {
    SENDERTASK,
  };

public:
  Receiver() {
    pin.SetTypeInfo(static_cast<int>(TASK_TYPE::SENDERTASK), "de::SenderTask", de::TaskDeSerializeCustom<SenderTask>, de::TaskDeleter<SenderTask>);
    pouts_[0]->SetTypeInfo("de::MediaTask", de::TaskSerializeCustom<de::MediaTask>);
    pouts_[1]->SetTypeInfo("de::NNTask", de::TaskSerialize<de::NNTask>);
    SetName("Receiver");
  }

  ~Receiver() {
    pin.DestroyQueue();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  void Proc(void* rx_task, int32_t task_type, POutType pout_type) {
    auto task = reinterpret_cast<SenderTask*>(rx_task);
    de::NDArray decoded;

    if (task) {
      switch (static_cast<TASK_TYPE>(task_type)) {
        case TASK_TYPE::SENDERTASK: {
          int32_t width, height, format;
          task->array.GetImgInfo(height, width, format);

          last_cnt_++;
          LOG(INFO) << "receive id=" << task->id << " last_id_=" << last_id_ << " last_cnt_=" << last_cnt_ << " task->total=" << task->total 
                         << " format= " << format << " width= " << width << " height= " << height;
          if ((last_cnt_ != 1) && (last_id_ != task->id)) { // id不同且不是该任务的首个输入
            LOG(ERROR) << "receive imcomplete task id = " << last_id_ << " expect cnt = " << (last_cnt_ + 1) << " current id = " << task->id;
            if (last_task_) delete last_task_;
            last_cnt_ = 1;
          }
          if (last_cnt_ == 1) { // 任务的首个输入
            last_task_ = new de::NNTask();
            last_task_->id = task->id;
            last_id_ = task->id;
          }
          
          if (format == DE_PIX_FMT_JPEG) { // JPEG数据送去解码
            mm::JpegDecoder::JpegDecode(task->array, decoded);
            last_task_->tensors.emplace_back(decoded);
            decoded.GetImgInfo(height, width, format);
            LOG(INFO) << "image decoded id=" << last_id_ << " format= " << format << " width= " << width << " height= " << height;
          } else {
            last_task_->tensors.emplace_back(task->array);
          }
          
          if (task->total == last_cnt_) {
            if (pouts_[1]) pouts_[1]->SendTask(last_task_);
            LOG(INFO) << "send to aiengine id=" << last_id_ << " input_size=" << last_task_->tensors.size();
            last_task_ = nullptr;
            last_cnt_ = 0;
          } 
          
        break;
        }

        default:
          LOG(ERROR) << "undefined task received!";
        break;
      }
    }

    pin.DelTask(rx_task, task_type);
  }

  void Stop(void) {
    pin.DestroyQueue();
  }

private:
  int last_id_ = 0;
  int last_cnt_ = 0;
  de::NNTask *last_task_ = nullptr;
};

}
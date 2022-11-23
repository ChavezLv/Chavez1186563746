#pragma once

#include "infer_engine_common.h"
#include "de_thread.h"
#include "de_aiengine_node.h"

namespace de {

// @brief 接收者node，用于接收芯片侧结果返回
class Receiver : public de::Thread
{
private:
  enum class TASK_TYPE : int32_t {
    SENDERTASK,
  };

public:
  Receiver() {
    pin.SetTypeInfo(static_cast<int>(TASK_TYPE::SENDERTASK), "de::SenderTask", de::TaskDeSerializeCustom<SenderTask>, de::TaskDeleter<SenderTask>);
    pouts_[0]->SetTypeInfo("de::NNTask", de::TaskSerialize<de::NNTask>);
    SetName("Receiver");
  }

  ~Receiver() {
    // 删除入队列，停止算子
    pin.DestroyQueue();

    // 等待队列线程退出
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  virtual void Proc(void* rx_task, int32_t task_type, POutType pout_type) {
    auto task = reinterpret_cast<SenderTask*>(rx_task);

    if (task) {
      switch (static_cast<TASK_TYPE>(task_type)) {
        case TASK_TYPE::SENDERTASK :
          if (pouts_[0]->IsLinked()) {
            last_cnt_++;
            // LOG(IMPORTANT) << "receive id=" << task->id << " last_id_=" << last_id_ << " last_cnt_=" << last_cnt_ << " task->total=" << task->total;
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
            
            last_task_->tensors.emplace_back(task->array);
            
            if (task->total == last_cnt_) {
              pouts_[0]->SendTask(last_task_);
              // LOG(IMPORTANT) << "send to output id=" << last_id_ << " input_size=" << last_task_->tensors.size();
              last_task_ = nullptr;
              last_cnt_ = 0;
            } 
          }
        break;

        default :
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
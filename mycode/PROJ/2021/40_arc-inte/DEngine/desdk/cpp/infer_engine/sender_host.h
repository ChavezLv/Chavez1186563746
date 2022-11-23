#pragma once

#include "infer_engine_common.h"
#include <thread>
#include <chrono>
#include "de_thread.h"
#include "de_task_serialize.h"


namespace de {

// @brief 发送者node，用于将数据发送给芯片
class Sender : public de::Thread
{
private:
  enum class TASK_TYPE : uint32_t {
    SENDERTASK,
  };

public:
  Sender() {
    pin.SetTypeInfo(static_cast<int>(TASK_TYPE::SENDERTASK), "de::SenderTask", de::TaskDeSerializeCustom<SenderTask>, de::TaskDeleter<SenderTask>);
    pouts_[0]->SetTypeInfo("de::SenderTask", de::TaskSerializeCustom<SenderTask>);
    SetName("Sender");
  }

  ~Sender() {
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
            pouts_[0]->SendTask(task);
          }
        break;

        default :
        break;
      }
    }
  }

  void Stop(void) {
    pin.DestroyQueue();
  }
};

}
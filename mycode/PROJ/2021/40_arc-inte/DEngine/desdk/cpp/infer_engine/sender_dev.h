#pragma once

#include "infer_engine_common.h"

namespace de {
 
class Sender : de::Thread {
private:
  enum class TASK_TYPE : int32_t {
    NNTASK,
  };

public:
  Sender() {
    pin.SetTypeInfo(static_cast<int>(TASK_TYPE::NNTASK), "de::NNTask", de::TaskDeSerialize<de::NNTask>, de::TaskDeleter<de::NNTask>);
    pouts_[0]->SetTypeInfo("de::SenderTask", de::TaskSerializeCustom<SenderTask>);
    SetName("Sender");
  }

  ~Sender() {
    pin.DestroyQueue();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  void Proc(void* rx_task, int32_t task_type, POutType pout_type) {
    auto task = reinterpret_cast<de::NNTask*>(rx_task);

    if (task) {
      switch (static_cast<TASK_TYPE>(task_type)) {
        case TASK_TYPE::NNTASK: {
          if (pouts_[0]->IsLinked()) {
            int total = task->tensors.size();
            int id = task->id;
            for (size_t i = 0; i < total; i++) {
              auto tx_task = new SenderTask;

              tx_task->array = task->tensors[i];
              tx_task->id = id;
              tx_task->total = total;
              
              pouts_[0]->SendTask(tx_task);
            }
          }
        break;
        }
        
        default:
        break;
      }
    }

    pin.DelTask(rx_task, task_type);
  }

  void Stop(void) {
    pin.DestroyQueue();
  }
};    
}
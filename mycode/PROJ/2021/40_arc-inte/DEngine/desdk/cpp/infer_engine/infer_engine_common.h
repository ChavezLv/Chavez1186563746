#pragma once

#include <vector>
#include "de_ndarray.h"
#include "de_serializer.h"

// class name
const char* HOST_SENDER_CLASS = "HostSender";
const char* HOST_RECEIVER_CLASS = "HostReceiver";

const char* DEVICE_SENDER_CLASS = "de::DevSender";
const char* DEVICE_RECEIVER_CLASS = "de::DevReceiver";
const char* DEVICE_AIENGINE_CLASS = "de::AiEngineExt";

// node name
const char* HOST_SENDER_NODE = "HostSender";
const char* HOST_RECEIVER_NODE = "HostReceiver";

const char* DEVICE_SENDER_NODE = "DeviceSender";
const char* DEVICE_RECEIVER_NODE = "DeviceReceiver";
const char* DEVICE_AIENGINE_NODE = "AiEngineExt";

namespace de {
typedef struct {
  de::NDArray array;        ///< data object
  int         id;           ///< request id, response return same id
  int         total;        ///< total num for a whole task

  de::NDArray Serialize() {
    int32_t *ptr = static_cast<int32_t*>(array.GetBearData());
    ptr[0] = id;
    ptr[1] = total;
    std::vector<int64_t> shapes;
    array.Shrink(shapes, 2 * sizeof(int32_t));
    return array;
  }

  void DeSerialize(de::NDArray data) {
    int32_t *ptr = static_cast<int32_t*>(data.GetBearData());
    id = ptr[0];
    total = ptr[1];
    array = data;
  }
} SenderTask;
}

namespace de {
namespace serializer {
  STRUCT_SERIALIZE_3(de::SenderTask,
    de::NDArray, array,
    int, id,
    int, total
  )
}
}

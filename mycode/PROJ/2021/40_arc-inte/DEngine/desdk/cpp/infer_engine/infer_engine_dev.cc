#include <fstream>
#include <random>
#include "de_graph_api.h"
#include "de_node.h"
#include <de_video_in_node.h>
#include <de_video_decoder_node.h>
#include "de_aiengine_ext_node.h"
#include "de_pic_decoder_node.h"
#include "de_pic_encoder_node.h"
#include "de_node_common.h"
#include "infer_engine_common.h"
#include "sender_dev.h"
#include "receiver_dev.h"


namespace de {
DE_CLASS_REGISTER(DEVICE_SENDER_CLASS, Sender);
DE_CLASS_REGISTER(DEVICE_RECEIVER_CLASS, Receiver);
//DE_CLASS_REGISTER(DEVICE_AIENGINE_CLASS, AiEngineExt);

void __attribute__((destructor)) x_fini(void)
{
  de::NodeClassRegistry::Remove(DEVICE_SENDER_CLASS);
  de::NodeClassRegistry::Remove(DEVICE_RECEIVER_CLASS);
  //de::NodeClassRegistry::Remove(DEVICE_AIENGINE_CLASS);
}
}
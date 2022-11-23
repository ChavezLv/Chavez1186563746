#include <iostream>
#include <thread>
#include <fstream>

#include "de_base_api.h"
#include "de_config.h"
#include "de_system_init.h"
#include "de_ndarray.h"
#include "de_model_api.h"
#include "de_pic_decoder_node.h"
#include "de_aiengine_node.h"

using namespace de;
using namespace de::mm;

std::ostream& operator << (std::ostream& os, ModelInfo* info) {
  os << "name: " << info->name << std::endl
         << "id: " << info->id << std::endl;
  return os;
}

int main(int argc, char *argv[]) {
  de::SDKCfg::Global()->ReadFile("/root/config/desdk.cfg");
  de::DeSystemInit(0);
  de::DeSDKInit();

  while(de::DeSDKAlive()) {
      std::this_thread::sleep_for(std::chrono::seconds(10));
      std::cout<< "SDK Alive!" << std::endl;
  }
  
  de::DeSDKDeInit();
  de::DeSystemDeinit();

  return 0;
}


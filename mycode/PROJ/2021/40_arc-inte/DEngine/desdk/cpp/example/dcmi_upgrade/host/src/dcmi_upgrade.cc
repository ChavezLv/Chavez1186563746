#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include "de_dcmi_api.h"

using namespace de::dcmi;

const int PROBE_USB_DEV_TIMES = 20 ;    //循环探测次数
const int PROBE_USB_DEV_WAIT = 2;          //2 seconds

int main(int argc, char *argv[]) {
  std::cout << "upgrade:\n";

  int ret;
  int devid;
  std::vector<VersionCfg>config;
  std::vector<DevId>configId;
  std::vector<de::ErrorCode> result;
  DevStatus status;
  int probe_times = 0;

  ret = de::dcmi::Init(1);
  if(ret < 0){
    std::cout << "gtest upgrade init fail.\n";
    return -1;
  }

  VersionCfg cfg = { 0, "/DEngine/desdk/archive/evb/file/images.tar.bz2", 300000, 0 };
  config.emplace_back(cfg);
  configId.emplace_back(cfg.dev_id);

  ret = de::dcmi::Upgrade(config, result);
  if(ret < 0){
    for(std::vector<de::ErrorCode>::iterator it=result.begin(); it!=result.end(); it++){
      std::cout << "result=" << std::hex << *it << std::dec << "\n";
    }
    return -1;
  }

  for(std::vector<DevId>::iterator it=configId.begin(); it!=configId.end(); it++){
    while(1){
      std::this_thread::sleep_for(std::chrono::seconds(PROBE_USB_DEV_WAIT));
      probe_times++;
      devid = (*it);
      de::dcmi::GetDevStatus(devid, status);
      if(probe_times == PROBE_USB_DEV_TIMES){
        break;
      }
      if(DEVICE_ONLINE == status){
        std::cout << "upgrade devid=" << devid << ", status=" << std::hex << status << std::dec << ", line="<< __LINE__ << "\n";
        break;
      }
    }
    if(probe_times == PROBE_USB_DEV_TIMES){
      return -1;
    }
  }

  return 0;
}
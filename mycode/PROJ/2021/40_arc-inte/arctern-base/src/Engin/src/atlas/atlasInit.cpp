//
// Created by gh on 2020/12/9.
//

#include "include/atlasInit.h"
#include <iostream>
#include "acl/acl.h"
#include "acl/ops/acl_dvpp.h"
bool initAclDeviceEnv( int deviceId) {
  auto ret = aclInit(nullptr); // Initialize ACL
  if (ret != 0) {
    std::cout << "Failed to init acl, ret =  "<< ret ;
    return false;
  }

  ret = aclrtSetDevice(deviceId);
  if (ret != 0) {
    std::cout << "Failed to open acl device: " << deviceId << std::endl;
    return false;
  }
  std::cout << "Open device " << deviceId << " successfully." << std::endl;
  return true;
}

bool destroyAclDeviceEnv( int deviceId) {

  auto ret = aclrtResetDevice(deviceId);
  if (ret != 0) {
    std::cout << "Failed to reset device, ret = " << ret << "." << std::endl;
    return false;
  }

  ret = aclFinalize();
  if (ret != 0) {
    std::cout << "Failed to finalize acl, ret = " << ret << "." << std::endl;
    return false;
  }
  std::cout << "Finalized acl successfully." << std::endl;

  return true;
}


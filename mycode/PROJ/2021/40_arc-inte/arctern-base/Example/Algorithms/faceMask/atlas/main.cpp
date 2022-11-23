/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.09
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "src/Algorithms/faceMask/faceMask.h"
#include "include/atlasInit.h"
#include <assert.h>
#include "src/common/file_buffer.h"
#include <thread>
#include <vector>
void test(){
  std::string imagePath = "/home/HwHiAiUser/users/chengaohua/old/arctern/tests-atlas/../data/image-cuda/P16-1920x1080.jpg";
  arctern::ArcternAtlasImgData imageData;
  arctern::FileBuffer filebuf(imagePath);

  arctern::FaceMask recog;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.model_path = "/home/HwHiAiUser/users/chengaohua/arctern-models/develop/atlas-models/face-mask-b1-acl-atlas300-f16-d-0.2.1.bin";
  auto errCode = recog.Init(&initParam);

  assert(errCode == arctern::ErrorType::ERR_SUCCESS);

  ::arctern::ArcternImage img;
  img.imgdata.atlasData.rawData =static_cast<void*>(filebuf.buffer_);
  img.imgdata.atlasData.rawDataLen = filebuf.length_; 
  arctern::FaceMaskRunParameter runParameter;
  runParameter.imageV_.push_back(img);

  ///> face rect, different picture has different face rect
  runParameter.rectsV_.push_back({1241, 372, 95,95});
  
  arctern::FaceMaskResult result;
  errCode = recog.Process(&runParameter, &result);

  if (errCode == arctern::ErrorType::ERR_SUCCESS) {
    std::cout << result.FaceMasks_[0].statusScore << std::endl;
  }

  

  arctern::FaceMask recog1;
  arctern::InitParameter initParam1;
  initParam1.batchNum_ = 8;
  initParam1.model_path = "/home/HwHiAiUser/users/chengaohua/arctern-models/develop/atlas-models/face-mask-b8-acl-atlas300-f16-d-0.2.1.bin";
  errCode = recog1.Init(&initParam1);

  assert(errCode == arctern::ErrorType::ERR_SUCCESS);

  arctern::FaceMaskRunParameter runParameter1;
  runParameter1.imageV_.push_back(img);

  ///> face rect, different picture has different face rect
  runParameter1.rectsV_.push_back({1241, 372, 95,95});
  
  arctern::FaceMaskResult result1;
  errCode = recog1.Process(&runParameter1, &result1);

  if (errCode == arctern::ErrorType::ERR_SUCCESS) {
    std::cout << result1.FaceMasks_[0].statusScore << std::endl;
  }
}
int main(int argc, char **argv) {
  const int deviceId = 0;
  ///> one process only call initAclDeviceEnv onece
  auto ret = initAclDeviceEnv(deviceId);
  if(ret)
  {
    long long index = 1;
    while(1){
      std::cout <<"======================="<< index++<<"============================"<<std::endl;
      std::vector<std::thread> vThread;
      for(int i=0;i<1;i++){
        vThread.emplace_back(std::thread(test));
      }
      for(int i=0;i<vThread.size();i++){
        vThread[i].join();
      }
    }
  } 
  ret = destroyAclDeviceEnv(deviceId);
  return 0; 
}

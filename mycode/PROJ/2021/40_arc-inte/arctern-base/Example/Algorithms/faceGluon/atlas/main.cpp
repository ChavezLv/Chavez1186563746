/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.09
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "src/Algorithms/faceGluon/faceGluon.h"
#include "include/atlasInit.h"
#include <assert.h>
#include "src/common/file_buffer.h"
#include <thread>
void test() {
  
  std::string imagePath = "/home/HwHiAiUser/users/chengaohua/old/arctern/tests-atlas/../data/image-cuda/P16-1920x1080.jpg";
  arctern::ArcternAtlasImgData imageData;
  arctern::FileBuffer filebuf(imagePath);

  arctern::FaceGluon recog;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.model_path = "/data/leizhiming/arctern-models/develop/atlas-models/face-gluon-b1-acl-atlas300-f16-d-3.0.0.bin";
  auto errCode = recog.Init(&initParam);

  assert(errCode == arctern::ErrorType::ERR_SUCCESS);

  ::arctern::ArcternImage img;
  img.imgdata.atlasData.rawData =static_cast<void*>(filebuf.buffer_);
  img.imgdata.atlasData.rawDataLen = filebuf.length_; 
  arctern::FaceGluonRunParameter runParameter;
  runParameter.imageV_.push_back(img);

  ///> face rect, different picture has different face rect
  runParameter.rectsV_.push_back({1241, 372, 95,95});
  
  arctern::FaceGluonResult result;
  errCode = recog.Process(&runParameter, &result);

  if (errCode == arctern::ErrorType::ERR_SUCCESS) {
    std::cout << result.features_.size() << std::endl;
    for(int i=0;i<result.features_[0].size();i++){
      std::cout << (int) result.features_[0][i] <<" ";
    }
    std::cout << std::endl;
  }
 
}

int main() {
  const int deviceId = 0;
  ///> one process only call initAclDeviceEnv onece
  auto ret = initAclDeviceEnv(deviceId);
  if(ret)
  {
    long long index = 1;
    while(1){
      std::cout <<"======================="<< index++<<"============================"<<std::endl;
      std::vector<std::thread> vThread;
      for(int i=0;i<10;i++){
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
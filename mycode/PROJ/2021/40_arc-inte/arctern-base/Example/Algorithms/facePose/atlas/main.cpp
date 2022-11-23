/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.09.09
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "src/Algorithms/facePose/facePose.h"
#include "include/atlasInit.h"
#include <assert.h>
#include "src/common/file_buffer.h"

int main(int argc, char **argv) {
  const int deviceId = 0;
  ///> one process only call initAclDeviceEnv onece
  auto ret = initAclDeviceEnv(deviceId);
  if(ret)
  {

    std::string imagePath = "/home/HwHiAiUser/users/chengaohua/old/arctern/tests-atlas/../data/image-cuda/P16-1920x1080.jpg";
    arctern::ArcternAtlasImgData imageData;
    arctern::FileBuffer filebuf(imagePath);

    arctern::FacePose recog;
    arctern::InitParameter initParam;
    initParam.batchNum_ = 1;
    initParam.model_path = "/home/HwHiAiUser/users/chengaohua/arctern-models/develop/atlas-models/face-pose-cls-b1-acl-atlas300-f16-d-1.0.0.bin";
    auto errCode = recog.Init(&initParam);

    assert(errCode == arctern::ErrorType::ERR_SUCCESS);

    ::arctern::ArcternImage img;
    img.imgdata.atlasData.rawData =static_cast<void*>(filebuf.buffer_);
    img.imgdata.atlasData.rawDataLen = filebuf.length_; 
    arctern::FacePoseRunParameter runParameter;
    runParameter.imageV_.push_back(img);

    ///> face rect, different picture has different face rect
    runParameter.rectsV_.push_back({1241, 372, 95,95});
    
    arctern::FacePoseResult result;
    errCode = recog.Process(&runParameter, &result);

    if (errCode == arctern::ErrorType::ERR_SUCCESS) {
      std::cout << result.facePose_[0].pitch << std::endl;
      std::cout << result.facePose_[0].yaw << std::endl;
      std::cout << result.facePose_[0].roll << std::endl;
      std::cout << result.facePose_[1].pitch << std::endl;
      std::cout << result.facePose_[1].yaw << std::endl;
      std::cout << result.facePose_[1].roll << std::endl;
    }
  } 
  ret = destroyAclDeviceEnv(deviceId);
  return 0; 
}

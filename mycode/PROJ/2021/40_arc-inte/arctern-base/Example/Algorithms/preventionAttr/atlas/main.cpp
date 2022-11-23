#include "src/Algorithms/preventionAttr/preventionAttr.h"
#include "include/atlasInit.h"
#include <assert.h>
#include "src/common/file_buffer.h"
#include "opencv2/opencv.hpp"
#include <thread>
#include <iostream>
void test() {

  std::string imagePath = "/home/leizhiming/0002.JPG";
  arctern::ArcternAtlasImgData imageData;
  arctern::FileBuffer filebuf(imagePath);

  arctern::PreventionAttr recog;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.model_path ="/home/leizhiming/prevention-attributes-b1-acl-atlas300-f16-d-0.1.0.bin";
  auto errCode = recog.Init(&initParam);

  assert(errCode == arctern::ErrorType::ERR_SUCCESS);

  ::arctern::ArcternImage img;
  img.imgdata.atlasData.rawData =static_cast<void*>(filebuf.buffer_);
  img.imgdata.atlasData.rawDataLen = filebuf.length_; 
  arctern::PreventionAttrRunParameter runParameter;
  runParameter.imageV_.push_back(img);
  runParameter.rectsV_.push_back({445,221,96,247});
  
  arctern::PreventionAttrResult result;
  errCode = recog.Process(&runParameter, &result);
  assert(errCode == arctern::ERR_SUCCESS);

  int pictureNum = result.PreventionAttrs_.size();
  std::cout << " picture num = " << pictureNum << std::endl;

  for (int pId = 0; pId < pictureNum; pId++) {
    std::cout <<(int) result.PreventionAttrs_[pId].exposeNeckband <<" " <<result.PreventionAttrs_[pId].exposeNeckbandScore<< " ";
    std::cout <<(int) result.PreventionAttrs_[pId].touchMask <<" " <<result.PreventionAttrs_[pId].touchMaskScore<< " ";
    std::cout <<(int) result.PreventionAttrs_[pId].playPhone <<" " <<result.PreventionAttrs_[pId].playPhoneScore<< std::endl;
  }
}

int main(int argc, char **argv) {
  const int deviceId = 0;
  ///> one process only call initAclDeviceEnv onece
  auto ret = initAclDeviceEnv(deviceId);
  if(ret)
  {
    test();
  } 
  ret = destroyAclDeviceEnv(deviceId);
  return 0; 
}
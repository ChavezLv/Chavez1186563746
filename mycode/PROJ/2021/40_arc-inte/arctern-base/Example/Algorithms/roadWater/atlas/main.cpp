#include "src/Algorithms/roadWater/roadWater.h"
#include "include/atlasInit.h"
#include <assert.h>
#include "src/common/file_buffer.h"
#include <thread>
#include <vector>
void test(){
  std::string imagePath = "/home/leizhiming/arctern-metadata/ModelsVerifyData/data/atlas/road_water/1.jpg";
  arctern::ArcternAtlasImgData imageData;
  arctern::FileBuffer filebuf(imagePath);

  arctern::RoadWater recog;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 8;
  initParam.model_path = "/home/leizhiming/arctern-models/develop/atlas-models/road-water-b8-acl-atlas300-f16-d-0.0.0.bin";
  auto errCode = recog.Init(&initParam);

  assert(errCode == arctern::ErrorType::ERR_SUCCESS);

  ::arctern::ArcternImage img;
  img.imgdata.atlasData.rawData =static_cast<void*>(filebuf.buffer_);
  img.imgdata.atlasData.rawDataLen = filebuf.length_; 
  arctern::RoadWaterRunParameter runParameter;
  runParameter.imageV_.push_back(img);

  arctern::RoadWaterResult result;
  errCode = recog.Process(&runParameter, &result);

  if (errCode == arctern::ErrorType::ERR_SUCCESS) {
    std::cout << result.roadWaterInfos_[0].confidence << std::endl;
  }
  recog.Release();
  

  arctern::RoadWater recog1;
  arctern::InitParameter initParam1;
  initParam1.batchNum_ = 1;
  initParam1.model_path = "/home/leizhiming/arctern-models/develop/atlas-models/road-water-b1-acl-atlas300-f16-d-0.0.0.bin";
   errCode = recog1.Init(&initParam1);

  assert(errCode == arctern::ErrorType::ERR_SUCCESS);

 
  arctern::RoadWaterRunParameter runParameter1;
  ::arctern::ArcternImage img1;
   arctern::FileBuffer filebuf1(imagePath);
  img1.imgdata.atlasData.rawData =static_cast<void*>(filebuf1.buffer_);
  img1.imgdata.atlasData.rawDataLen = filebuf1.length_;
  runParameter1.imageV_.push_back(img1);
  
  arctern::RoadWaterResult result1;
  errCode = recog1.Process(&runParameter1, &result1);

  if (errCode == arctern::ErrorType::ERR_SUCCESS) {
    std::cout << result1.roadWaterInfos_[0].confidence << std::endl;
  }
  recog1.Release();
}
void test1(){
  std::string imagePath = "/home/leizhiming/arctern-metadata/ModelsVerifyData/data/atlas/road_water/1.jpg";
  arctern::ArcternAtlasImgData imageData;
  arctern::FileBuffer filebuf(imagePath);

  arctern::RoadWater recog;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.model_path = "/home/leizhiming/arctern-models/develop/atlas-models/road-water-b1-acl-atlas300-f16-d-0.0.0.bin";
  auto errCode = recog.Init(&initParam);

  assert(errCode == arctern::ErrorType::ERR_SUCCESS);

  ::arctern::ArcternImage img;
  img.imgdata.atlasData.rawData =static_cast<void*>(filebuf.buffer_);
  img.imgdata.atlasData.rawDataLen = filebuf.length_; 
  arctern::RoadWaterRunParameter runParameter;
  runParameter.imageV_.push_back(img);
  
  arctern::RoadWaterResult result;
  errCode = recog.Process(&runParameter, &result);

  if (errCode == arctern::ErrorType::ERR_SUCCESS) {
    std::cout << result.roadWaterInfos_[0].confidence << std::endl;
  }
}
int main(int argc, char **argv) {
  const int deviceId = 0;
  ///> one process only call initAclDeviceEnv onece
  auto ret = initAclDeviceEnv(deviceId);
  if(ret)
  {
    //test();
    //test1();
    long long index = 1;
    while(1){
      std::cout <<"======================="<< index++<<"============================"<<std::endl;
      std::vector<std::thread> vThread;
      for(int i=0;i<1;i++){
        vThread.emplace_back(std::thread(test));
        //vThread.emplace_back(std::thread(test1));
      }
      for(int i=0;i<vThread.size();i++){
        vThread[i].join();
      }
    }
  } 
  ret = destroyAclDeviceEnv(deviceId);
  return 0; 
}

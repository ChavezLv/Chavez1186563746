

#include "src/Algorithms/advertisementDet/advertisementDet.h"
#include "include/atlasInit.h"
#include <assert.h>
#include "src/common/file_buffer.h"
#include <thread>
#include <vector>
void test(){
  std::string imagePath = "/home/intellif/zy/arcternRepo/arctern-adapte-20210604/data/align2/testAdverisement/1.jpg";
  arctern::ArcternAtlasImgData imageData;
  arctern::FileBuffer filebuf(imagePath);

  arctern::AdvertisementDet recog;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.model_path = "/home/leizhiming/arctern-models/develop/atlas-models/advertisement-det-b1-acl-atlas300-f16-d-0.2.0.bin";
  auto errCode = recog.Init(&initParam);

  assert(errCode == arctern::ErrorType::ERR_SUCCESS);

  ::arctern::ArcternImage img;
  img.imgdata.atlasData.rawData =static_cast<void*>(filebuf.buffer_);
  img.imgdata.atlasData.rawDataLen = filebuf.length_; 
  arctern::AdvertisementDetRunParameter runParameter;
  runParameter.imageV_.push_back(img);

  ///> face rect, different picture has different face rect
  //runParameter.rectsV_.push_back({1241, 372, 95,95});
  
  arctern::AdvertisementDetResult result;
  errCode = recog.Process(&runParameter, &result);

  if (errCode == arctern::ErrorType::ERR_SUCCESS) {
    std::cout <<result.detInfos_[0][0].rect.x<<" "<< result.detInfos_[0][0].rect.y<<" "<<result.detInfos_[0][0].rect.width<<" "
    <<result.detInfos_[0][0].rect.height<<" " <<result.detInfos_[0][0].score << std::endl;
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
      for(int i=0;i<2;i++){
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

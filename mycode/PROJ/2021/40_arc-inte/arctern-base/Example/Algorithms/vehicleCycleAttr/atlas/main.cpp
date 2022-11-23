

#include "src/Algorithms/vehicleCycleAttr/vehicleCycleAttr.h"
#include "include/atlasInit.h"
#include <assert.h>
#include "src/common/file_buffer.h"
#include <thread>
#include <vector>
void test(){
  std::string imagePath = "/home/leizhiming/arctern/data/align2/pbike/001.jpg";
  arctern::ArcternAtlasImgData imageData;
  arctern::FileBuffer filebuf(imagePath);

  arctern::VehicleCycleAttr recog;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.model_path = "/home/leizhiming/arctern-models/develop/atlas-models/vehicle-cycle-attributes-b1-acl-atlas300-f16-d-0.0.3.bin";
  auto errCode = recog.Init(&initParam);

  assert(errCode == arctern::ErrorType::ERR_SUCCESS);

  ::arctern::ArcternImage img;
  img.imgdata.atlasData.rawData =static_cast<void*>(filebuf.buffer_);
  img.imgdata.atlasData.rawDataLen = filebuf.length_; 
  arctern::VehicleCycleAttrRunParameter runParameter;
  runParameter.imageV_.push_back(img);

  ///> face rect, different picture has different face rect
  runParameter.rectsV_.push_back({529, 294, 274, 416});
  
  arctern::VehicleCycleAttrResult result;
  errCode = recog.Process(&runParameter, &result);

  if (errCode == arctern::ErrorType::ERR_SUCCESS) {
    std::cout << result.vehicleCycleAttrs_[0].hasUmbrellaScore << std::endl;
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

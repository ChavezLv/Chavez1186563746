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
#include "de_registry.h"

using namespace de;
using namespace de::mm;

std::ostream& operator << (std::ostream& os, ModelInfo* info) {
  os << "name: " << info->name << std::endl
     << "id: " << info->id << std::endl;
  return os;
}

NDArray get_image(int width, int height, const char *pic_path = nullptr, const char *pic_data = nullptr) {
  std::vector<int64_t> shape;

  FILE* fp = fopen(pic_path, "rb");
  if (fp == NULL){
      de::NDArray empty;
      return empty;
  }
  fseek(fp, 0L, SEEK_END);
  size_t fileSize = ftell(fp);
  shape.push_back(fileSize);

  ExtInfo addition;
  addition.shape_indicator = deShapeCode::W;
  addition.format = dePixelFormat::DE_PIX_FMT_MJPEG;
  addition.bear_data_len = 0;
  addition.pts = 0;
  addition.roi_rect = {0, 0, width, height};

  NDArray image = NDArray::Empty(shape, {DLDataTypeCode::kDLUInt, 8, 1}, {DLDeviceType::kDLCPU, 0}, addition);

  if (pic_path) {
    std::string file = pic_path;
    std::ifstream f(file, std::ios_base::binary | std::ios_base::out);
    if(f) {
      f.read(reinterpret_cast<char*>(image.GetTensorData()), image.GetTensorDataSize());
      f.close();
    }
  } else if (pic_data) {
    memcpy((char*)image.GetTensorData(), pic_data, image.GetTensorDataSize());
  }

  return image;
}

int model_predict() {
  // 图片位于芯片/root/app/bin目录
  auto img = get_image(333, 500, "/root/app/bin/1.jpg");
  NDArray img_decode;
  auto result = JpegDecoder::JpegDecode(img, img_decode);

  // 模型文件位于芯片/root/data目录
  AiEngine engine(false);
  result = engine.LoadModel("/root/data/net.bin", "/root/data/model.bin");

  // 推理
  auto ret = 0;

  NNTask task_in;
  task_in.id = 0;
  task_in.tensors.emplace_back(img_decode);

  auto task_out = static_cast<NNTask*>(engine.Call(&task_in, 0));
  if (task_out) {
    std::cout << "tensor count: " << task_out->tensors.size() << std::endl;
    std::for_each(task_out->tensors.begin(), task_out->tensors.end(), [](NDArray& tensor) {
      // 在这里进行后处理
    });
  } else {
    ret = -1;
    std::cout << "predict fail\n";
  }

  return ret;
}

DE_REGISTER_GLOBAL("de.example.model_pred")
.set_body([](de::DEArgs args, de::DERetValue* rv) {
   *rv = model_predict();
});
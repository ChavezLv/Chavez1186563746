//
// Created by gh on 2021/8/19.
//

#include "mnnEngin.h"
#include <numeric>
#include <functional>

namespace arctern {
MnnEngin::MnnEngin() {

}

MnnEngin::~MnnEngin() throw() {
  release();
}

int MnnEngin::init(const void * model,size_t modleSize,InitParameter *p,int h,int w) {
  release();

  //todo
  //const char * modelPath = "/data/leizhiming/yolov5s_face_det_v1.0.1_dynamic_batch1.mnn";

  interpreter_ = std::unique_ptr<MNN::Interpreter>(MNN::Interpreter::createFromBuffer(model,modleSize));

  MNN::ScheduleConfig schedule_config;
  schedule_config.type = MNN_FORWARD_CPU;
  schedule_config.numThread = 4;
  MNN::BackendConfig backend_config;
  backend_config.precision = MNN::BackendConfig::Precision_Normal;
  schedule_config.backendConfig = &backend_config;
  session_ = interpreter_->createSession(schedule_config);
  assert(session_);
  input_tensor_ = interpreter_->getSessionInput(session_, nullptr);

  interpreter_->resizeTensor(input_tensor_, {p->batchNum_, 3, h, w});
  interpreter_->resizeSession(session_);

  return 0;
}

int MnnEngin::release() {
  if(interpreter_) {
    interpreter_->releaseModel();
    interpreter_->releaseSession(session_);

  }
  interpreter_ = nullptr;
  return 0;
}

bool MnnEngin::loadModel(const MxnetModel *model,InitParameter *p,int h,int w) {
  auto mnnModel =  model->params()->data();
  size_t modelSize = model->params()->size();
  init((void*)mnnModel,modelSize,p,h,w);
  return true;
}

static int calcShapeSize(std::vector<int> shape) {
 return  std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<int>());
}


int MnnEngin::forward(const Tensor<float> &input_tensor, std::vector<arctern::Tensor<float>> &outputTensors) {
  std::vector<int> dims;
  for(int i = 0; i< input_tensor.shape().num_dims(); i++) {
    dims.push_back(input_tensor.shape()[i]);
  }

  auto nhwc_tensor = MNN::Tensor::create<float>(dims, (void *)input_tensor.data(), MNN::Tensor::TENSORFLOW);

  input_tensor_->copyFromHostTensor(nhwc_tensor);
  // forward
  interpreter_->runSession(session_);

  auto output =  interpreter_->getSessionOutputAll(session_);
  for(auto &item : output) {
    MNN::Tensor * tensor = interpreter_->getSessionOutput(session_, item.first.c_str());
    auto size = calcShapeSize(tensor->shape());
    TensorShape shape(1, size);
    arctern::Tensor<float>  outputTensor(shape);

    MNN::Tensor hostTensor(tensor,tensor->getDimensionType());
    tensor->copyToHostTensor(&hostTensor);

    memcpy(outputTensor.data(), hostTensor.host<float>(), size * sizeof(float));
    outputTensors.push_back(outputTensor);
  }
//  for(auto item : outputTensors) {
//    std::cout<<"---------------->"<<item.data()[0]<<" "<<item.data()[1]<<"  "<<item.data()[2]<<std::endl;
//  }
  return 0;
}

void MnnEngin::setOutputName(std::vector<std::string> ops) {
  output_names_.swap(ops);
}

}

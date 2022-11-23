//
// Created by gpu on 19-12-2.
//

#include "EqualPlugin.h"
#include "EqualKernel.h"

using namespace arctern::trtPlugin;

namespace {
    static const char *EQUAL_PLUGIN_VERSION{"1"};
    static const char *EQUAL_PLUGIN_NAME{"Equal_ARCTERN"};
}
/// Static class fields initialization
PluginFieldCollection EqualPluginCreator::mFC{};
std::vector<PluginField> EqualPluginCreator::mPluginAttributes;

EqualPlugin::EqualPlugin(const std::string name) : BasePlugin(name){}

EqualPlugin::EqualPlugin(const std::string name, const std::vector<Dims> &inputShape,
                         const std::vector<Dims> &outputShape)
                         :BasePlugin(name, inputShape, outputShape){}

EqualPlugin::EqualPlugin(const std::string name, const void *data, size_t length)
                         :BasePlugin(name){
  deserializeBase(data, length);
}

const char* EqualPlugin::getPluginType() const {
  return EQUAL_PLUGIN_NAME;
}

const char* EqualPlugin::getPluginVersion() const {
  return EQUAL_PLUGIN_VERSION;
}

int EqualPlugin::getNbOutputs() const {
  return 1;
}

Dims EqualPlugin::getOutputDimensions(int index, const Dims *inputs, int nbInputDims) {
  ASSERT(index == 0);
  ASSERT(nbInputDims == 2);
  ASSERT(inputs[0].nbDims == inputs[1].nbDims);

  Dims outputShape{};
  outputShape.nbDims = inputs[0].nbDims;
  for(int d = 0; d < inputs[0].nbDims; ++d){
    outputShape.type[d] = inputs[0].type[d];
    outputShape.d[d] = inputs[0].d[d];
  }

  return outputShape;
}

bool EqualPlugin::supportsFormat(DataType type, PluginFormat format) const {
  return type == DataType::kFLOAT && format == PluginFormat::kNCHW;
}

int EqualPlugin::initialize() {
  int nbOutput = getNbOutputs();
  for(int i = 0; i < nbOutput; ++i){
    mOutputShape.push_back(getOutputDimensions(i, &mInputShape[0], static_cast<int>(mInputShape.size())));
  }

  return 0;
}

void EqualPlugin::terminate() {}

int EqualPlugin::enqueue(int batchSize, const void *const *inputs, void **outputs, void *workspace,
                         cudaStream_t stream) {
  int threadNum = batchSize;
  for(int i = 0; i < mOutputShape[0].nbDims; ++i){
    threadNum *= mOutputShape[0].d[i];
  }

  auto status = EqualInference(stream, threadNum, outputs[0], inputs[0], inputs[1]);
  ASSERT(status == STATUS_SUCCESS);

  return 0;
}

size_t EqualPlugin::getSerializationSize() const {
  return (getBaseSerializationSize());
}

void EqualPlugin::serialize(void *buffer) const {
  serializeBase(buffer);
}

IPluginV2Ext* EqualPlugin::clone() const {
  return new EqualPlugin(mLayerName, mInputShape, mOutputShape);
}

DataType EqualPlugin::getOutputDataType(int index, const nvinfer1::DataType *inputTypes, int nbInputs) const {
  ASSERT(index ==0);
  return DataType::kFLOAT;
}

void EqualPlugin::configurePlugin(const Dims *inputDims, int nbInputs, const Dims *outputDims, int nbOutputs,
                                  const DataType *inputTypes, const DataType *outputTypes,
                                  const bool *inputIsBroadcast, const bool *outputIsBroadcast,
                                  PluginFormat floatFormat, int maxBatchSize) {
  ASSERT(nbInputs == 2);
  ASSERT(nbOutputs == 1);
  ASSERT(inputTypes[0] == DataType::kFLOAT);
  ASSERT(inputTypes[1] == DataType::kFLOAT);
  ASSERT(floatFormat == PluginFormat::kNCHW);

  mInputShape.assign(inputDims, inputDims + nbInputs);
}

EqualPluginCreator::EqualPluginCreator() {}

const char* EqualPluginCreator::getPluginName() const {
  return EQUAL_PLUGIN_NAME;
}

const char* EqualPluginCreator::getPluginVersion() const {
  return EQUAL_PLUGIN_VERSION;
}

const PluginFieldCollection* EqualPluginCreator::getFieldNames() {
  return &mFC;
}

IPluginV2Ext* EqualPluginCreator::createPlugin(const char *name, const PluginFieldCollection *fc) {
  IPluginV2Ext* plugin = new EqualPlugin(name);
  plugin->setPluginNamespace(mNamespace.c_str());

  return plugin;
}

IPluginV2Ext* EqualPluginCreator::deserializePlugin(const char *name, const void *serialData,
                                                    size_t serialLength) {
  IPluginV2Ext* plugin = new EqualPlugin(name, serialData, serialLength);
  plugin->setPluginNamespace(mNamespace.c_str());

  return plugin;
}


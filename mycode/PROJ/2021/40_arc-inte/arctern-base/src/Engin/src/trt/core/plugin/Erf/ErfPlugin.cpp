//
// Created by gpu on 19-12-2.
//

#include "ErfPlugin.h"
#include "ErfKernel.h"

using namespace arctern::trtPlugin;

namespace {
    static const char *ERF_PLUGIN_VERSION{"1"};
    static const char *ERF_PLUGIN_NAME{"Erf_ARCTERN"};
}
/// Static class fields initialization
PluginFieldCollection ErfPluginCreator::mFC{};
std::vector<PluginField> ErfPluginCreator::mPluginAttributes;

ErfPlugin::ErfPlugin(const std::string name) : BasePlugin(name){}

ErfPlugin::ErfPlugin(const std::string name, const std::vector<Dims> &inputShape,
                     const std::vector<Dims> &outputShape)
                     :BasePlugin(name, inputShape, outputShape){}

ErfPlugin::ErfPlugin(const std::string name, const void *data, size_t length)
                     :BasePlugin(name){
  deserializeBase(data, length);
}

const char* ErfPlugin::getPluginType() const {
  return ERF_PLUGIN_NAME;
}

const char* ErfPlugin::getPluginVersion() const {
  return ERF_PLUGIN_VERSION;
}

int ErfPlugin::getNbOutputs() const {
  return 1;
}

Dims ErfPlugin::getOutputDimensions(int index, const Dims *inputs, int nbInputDims) {
  ASSERT(index == 0);
  ASSERT(nbInputDims == 1);

  Dims outputShape{};
  outputShape.nbDims = inputs[0].nbDims;
  for(int d = 0; d < inputs[0].nbDims; ++d){
    outputShape.type[d] = inputs[0].type[d];
    outputShape.d[d] = inputs[0].d[d];
  }

  return outputShape;
}

bool ErfPlugin::supportsFormat(DataType type, PluginFormat format) const {
  return type == DataType::kFLOAT && format == PluginFormat::kNCHW;
}

int ErfPlugin::initialize() {
  int nbOutput = getNbOutputs();
  for(int i = 0; i < nbOutput; ++i){
    mOutputShape.push_back(getOutputDimensions(i, &mInputShape[0], static_cast<int>(mInputShape.size())));
  }

  return 0;
}

void ErfPlugin::terminate() {}

int ErfPlugin::enqueue(int batchSize, const void *const *inputs, void **outputs, void *workspace,
                       cudaStream_t stream) {
  int threadNum = batchSize;
  for(int i = 0; i < mOutputShape[0].nbDims; ++i){
    threadNum *= mOutputShape[0].d[i];
  }

  auto status = ErfInference(stream, threadNum, outputs[0], inputs[0]);
  ASSERT(status == STATUS_SUCCESS);

  return 0;
}

size_t ErfPlugin::getSerializationSize() const {
  return (getBaseSerializationSize());
}

void ErfPlugin::serialize(void *buffer) const {
  serializeBase(buffer);
}

IPluginV2Ext* ErfPlugin::clone() const {
  return new ErfPlugin(mLayerName, mInputShape, mOutputShape);
}

DataType ErfPlugin::getOutputDataType(int index, const nvinfer1::DataType *inputTypes, int nbInputs) const {
  ASSERT(index ==0);
  return DataType::kFLOAT;
}

void ErfPlugin::configurePlugin(const Dims *inputDims, int nbInputs, const Dims *outputDims, int nbOutputs,
                                const DataType *inputTypes, const DataType *outputTypes,
                                const bool *inputIsBroadcast, const bool *outputIsBroadcast,
                                PluginFormat floatFormat, int maxBatchSize) {
  ASSERT(nbInputs == 1);
  ASSERT(nbOutputs == 1);
  ASSERT(inputTypes[0] == DataType::kFLOAT);
  ASSERT(floatFormat == PluginFormat::kNCHW);

  mInputShape.assign(inputDims, inputDims + nbInputs);
}

ErfPluginCreator::ErfPluginCreator() {}

const char* ErfPluginCreator::getPluginName() const {
  return ERF_PLUGIN_NAME;
}

const char* ErfPluginCreator::getPluginVersion() const {
  return ERF_PLUGIN_VERSION;
}

const PluginFieldCollection* ErfPluginCreator::getFieldNames() {
  return &mFC;
}

IPluginV2Ext* ErfPluginCreator::createPlugin(const char *name, const PluginFieldCollection *fc) {
  IPluginV2Ext* plugin = new ErfPlugin(name);
  plugin->setPluginNamespace(mNamespace.c_str());

  return plugin;
}

IPluginV2Ext* ErfPluginCreator::deserializePlugin(const char *name, const void *serialData,
                                                    size_t serialLength) {
  IPluginV2Ext* plugin = new ErfPlugin(name, serialData, serialLength);
  plugin->setPluginNamespace(mNamespace.c_str());

  return plugin;
}


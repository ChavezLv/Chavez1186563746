//
// Created by gpu on 19-12-2.
//
#include "ModSaclar.h"
#include "ModSaclarKernal.h"

using namespace arctern::trtPlugin;

namespace {
    static const char *MODSACLAR_PLUGIN_VERSION{"1"};
    static const char *MODSACLAR_PLUGIN_NAME{"ModScalar_ARCTERN"};
}
/// Static class fields initialization
PluginFieldCollection ModSaclarPluginCreator::mFC{};
std::vector<PluginField> ModSaclarPluginCreator::mPluginAttributes;

ModSaclarPlugin::ModSaclarPlugin(const std::string name, int scalar, int mode)
                               : BasePlugin(name), scalarVal(scalar), mode(mode){}

ModSaclarPlugin::ModSaclarPlugin(const std::string name, const std::vector<Dims> &inputShape,
                                 const std::vector<Dims> &outputShape, int scalar, int mode)
                                :BasePlugin(name, inputShape, outputShape), scalarVal(scalar), mode(mode){}

ModSaclarPlugin::ModSaclarPlugin(const std::string name, const void *data, size_t length)
                                :BasePlugin(name){
  deserializeBase(data, length);
  deserialize_value(&data, &length, &scalarVal);
  deserialize_value(&data, &length, &mode);
}

const char* ModSaclarPlugin::getPluginType() const {
  return MODSACLAR_PLUGIN_NAME;
}

const char* ModSaclarPlugin::getPluginVersion() const {
  return MODSACLAR_PLUGIN_VERSION;
}

int ModSaclarPlugin::getNbOutputs() const {
  return 1;
}

Dims ModSaclarPlugin::getOutputDimensions(int index, const Dims *inputs, int nbInputDims) {
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

bool ModSaclarPlugin::supportsFormat(DataType type, PluginFormat format) const {
  return type == DataType::kFLOAT && format == PluginFormat::kNCHW;
}

int ModSaclarPlugin::initialize() {
  int nbOutput = getNbOutputs();
  for(int i = 0; i < nbOutput; ++i){
    mOutputShape.push_back(getOutputDimensions(i, &mInputShape[0], static_cast<int>(mInputShape.size())));
  }

  return 0;
}

void ModSaclarPlugin::terminate() {}

int ModSaclarPlugin::enqueue(int batchSize, const void *const *inputs, void **outputs, void *workspace,
                         cudaStream_t stream) {
  int threadNum = batchSize;
  for(int i = 0; i < mOutputShape[0].nbDims; ++i){
    threadNum *= mOutputShape[0].d[i];
  }

  auto status = ModSaclarInference(stream, threadNum, outputs[0], inputs[0], scalarVal, mode);
  ASSERT(status == STATUS_SUCCESS);

  return 0;
}

size_t ModSaclarPlugin::getSerializationSize() const {
  return (getBaseSerializationSize() + serialized_size(scalarVal) + serialized_size(mode));
}

void ModSaclarPlugin::serialize(void *buffer) const {
  serializeBase(buffer);
  serialize_value(&buffer, scalarVal);
  serialize_value(&buffer, mode);
}

IPluginV2Ext* ModSaclarPlugin::clone() const {
  return new ModSaclarPlugin(mLayerName, mInputShape, mOutputShape, scalarVal, mode);
}

DataType ModSaclarPlugin::getOutputDataType(int index, const nvinfer1::DataType *inputTypes, int nbInputs) const {
  ASSERT(index ==0);
  return DataType::kFLOAT;
}

void ModSaclarPlugin::configurePlugin(const Dims *inputDims, int nbInputs, const Dims *outputDims, int nbOutputs,
                                      const DataType *inputTypes, const DataType *outputTypes,
                                      const bool *inputIsBroadcast, const bool *outputIsBroadcast,
                                      PluginFormat floatFormat, int maxBatchSize) {
  ASSERT(nbInputs == 1);
  ASSERT(nbOutputs == 1);
  ASSERT(inputTypes[0] == DataType::kFLOAT);
  ASSERT(floatFormat == PluginFormat::kNCHW);

  mInputShape.assign(inputDims, inputDims + nbInputs);
}

ModSaclarPluginCreator::ModSaclarPluginCreator() {
  mPluginAttributes.emplace_back(PluginField("scalar", nullptr, PluginFieldType::kINT32, 1));
  mPluginAttributes.emplace_back(PluginField("mode", nullptr, PluginFieldType::kINT32, 1));

  mFC.nbFields = static_cast<int>(mPluginAttributes.size());
  mFC.fields = mPluginAttributes.data();
}

const char* ModSaclarPluginCreator::getPluginName() const {
  return MODSACLAR_PLUGIN_NAME;
}

const char* ModSaclarPluginCreator::getPluginVersion() const {
  return MODSACLAR_PLUGIN_VERSION;
}

const PluginFieldCollection* ModSaclarPluginCreator::getFieldNames() {
  return &mFC;
}

IPluginV2Ext* ModSaclarPluginCreator::createPlugin(const char *name, const PluginFieldCollection *fc) {
  int scalar;
  int mode;
  const PluginField* fields = fc->fields;
  for (int i = 0; i < fc->nbFields; ++i)
  {
    const char* attrName = fields[i].name;
    if (strcmp(attrName, "scalar") == 0)
    {
      ASSERT(fields[i].type == PluginFieldType::kINT32);
      scalar = *(static_cast<const int*>(fields[i].data));
    }
    if (strcmp(attrName, "mode") == 0)
    {
      ASSERT(fields[i].type == PluginFieldType::kINT32);
      mode = *(static_cast<const int*>(fields[i].data));
    }
  }

  IPluginV2Ext* plugin = new ModSaclarPlugin(name, scalar, mode);
  plugin->setPluginNamespace(mNamespace.c_str());
  return plugin;
}

IPluginV2Ext* ModSaclarPluginCreator::deserializePlugin(const char *name, const void *serialData,
                                                        size_t serialLength) {
  IPluginV2Ext* plugin = new ModSaclarPlugin(name, serialData, serialLength);
  plugin->setPluginNamespace(mNamespace.c_str());

  return plugin;
}
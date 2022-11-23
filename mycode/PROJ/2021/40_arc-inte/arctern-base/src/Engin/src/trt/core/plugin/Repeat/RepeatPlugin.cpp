//
// Created by gpu on 19-12-2.
//

#include "RepeatPlugin.h"
#include "RepeatKernel.h"

using namespace arctern::trtPlugin;

namespace {
    static const char *REPEAT_PLUGIN_VERSION{"1"};
    static const char *REPEAT_PLUGIN_NAME{"Repeat_ARCTERN"};
}
/// Static class fields initialization
PluginFieldCollection RepeatPluginCreator::mFC{};
std::vector<PluginField> RepeatPluginCreator::mPluginAttributes;

RepeatPlugin::RepeatPlugin(const std::string name, int repeatCnt, int axis)
              :repeatCnt_(repeatCnt), axis_(axis), BasePlugin(name){}

RepeatPlugin::RepeatPlugin(const std::string name, const std::vector<Dims> &inputShape,
                          const std::vector<Dims> &outputShape, int repeatCnt, int axis)
                         :repeatCnt_(repeatCnt), axis_(axis),
                          BasePlugin(name, inputShape, outputShape){}

RepeatPlugin::RepeatPlugin(const std::string name, const void *data, size_t length)
                          :BasePlugin(name){
  deserializeBase(data, length);
  deserialize_value(&data, &length, &repeatCnt_);
  deserialize_value(&data, &length, &axis_);
}

const char* RepeatPlugin::getPluginType() const {
  return REPEAT_PLUGIN_NAME;
}

const char* RepeatPlugin::getPluginVersion() const {
    return REPEAT_PLUGIN_VERSION;
}

int RepeatPlugin::getNbOutputs() const {
  return 1;
}

Dims RepeatPlugin::getOutputDimensions(int index, const Dims *inputs, int nbInputDims) {
  ASSERT(index == 0);
  ASSERT(nbInputDims == 1);

  const Dims dshape = inputs[0];
  auto rank = dshape.nbDims;
  assert(axis_ <= rank);

  Dims oshape= dshape;
  oshape.d[axis_] *= repeatCnt_;

  return oshape;
}

bool RepeatPlugin::supportsFormat(DataType type, PluginFormat format) const {
  return type == DataType::kFLOAT && format == PluginFormat::kNCHW;
}

int RepeatPlugin::initialize() {
  int nbOutput = getNbOutputs();
  for(int i = 0; i < nbOutput; ++i){
    mOutputShape.push_back(getOutputDimensions(i, &mInputShape[0], static_cast<int>(mInputShape.size())));
  }

  return 0;
}

void RepeatPlugin::terminate() {}

int RepeatPlugin::enqueue(int batchSize, const void *const *inputs, void **outputs, void *workspace,
                         cudaStream_t stream) {

  auto status = RepeatInference(stream, outputs[0], inputs[0],
                                batchSize, mInputShape[0].d, mOutputShape[0].d, repeatCnt_, axis_);
  ASSERT(status == STATUS_SUCCESS);

  return 0;
}

size_t RepeatPlugin::getSerializationSize() const {
  return (getBaseSerializationSize() + serialized_size(repeatCnt_) + serialized_size(axis_));
}

void RepeatPlugin::serialize(void *buffer) const {
  serializeBase(buffer);
  serialize_value(&buffer, repeatCnt_);
  serialize_value(&buffer, axis_);
}

IPluginV2Ext* RepeatPlugin::clone() const {
  return new RepeatPlugin(mLayerName, mInputShape, mOutputShape, repeatCnt_, axis_);
}

DataType RepeatPlugin::getOutputDataType(int index, const nvinfer1::DataType *inputTypes, int nbInputs) const {
  ASSERT(index ==0);
  return DataType::kFLOAT;
}

void RepeatPlugin::configurePlugin(const Dims *inputDims, int nbInputs, const Dims *outputDims, int nbOutputs,
                                  const DataType *inputTypes, const DataType *outputTypes,
                                  const bool *inputIsBroadcast, const bool *outputIsBroadcast,
                                  PluginFormat floatFormat, int maxBatchSize) {
  ASSERT(nbInputs == 1);
  ASSERT(nbOutputs == 1);
  ASSERT(inputDims->nbDims == 4);

  mInputShape.assign(inputDims, inputDims + nbInputs);
}

RepeatPluginCreator::RepeatPluginCreator() {
  mPluginAttributes.emplace_back(PluginField("repeats", nullptr, PluginFieldType::kINT32, 1));
  mPluginAttributes.emplace_back(PluginField("axis", nullptr, PluginFieldType::kINT32, 1));

  mFC.nbFields = static_cast<int>(mPluginAttributes.size());
  mFC.fields = mPluginAttributes.data();
}

const char* RepeatPluginCreator::getPluginName() const {
  return REPEAT_PLUGIN_NAME;
}

const char* RepeatPluginCreator::getPluginVersion() const {
  return REPEAT_PLUGIN_VERSION;
}

const PluginFieldCollection* RepeatPluginCreator::getFieldNames() {
  return &mFC;
}

IPluginV2Ext* RepeatPluginCreator::createPlugin(const char *name, const PluginFieldCollection *fc) {
  int repeatCnt;
  int axis;
  const PluginField* fields = fc->fields;
  for(int i = 0; i < fc->nbFields; ++i){
    const char* attrName = fields[i].name;
    if(strcmp(attrName, "repeats") == 0){
      ASSERT(fields[i].type == PluginFieldType::kINT32);
      repeatCnt = *(static_cast<const int*>(fields[i].data));
    }
    else if (strcmp(attrName, "axis") == 0){
      ASSERT(fields[i].type == PluginFieldType::kINT32);
      repeatCnt = *(static_cast<const int*>(fields[i].data));
    }
  }

  RepeatPlugin* plugin = new RepeatPlugin(name, repeatCnt, axis);
  plugin->setPluginNamespace(mNamespace.c_str());

  return plugin;
}

IPluginV2Ext* RepeatPluginCreator::deserializePlugin(const char *name, const void *serialData,
                                                    size_t serialLength) {
  RepeatPlugin* plugin = new RepeatPlugin(name, serialData, serialLength);
  plugin->setPluginNamespace(mNamespace.c_str());

  return plugin;
}


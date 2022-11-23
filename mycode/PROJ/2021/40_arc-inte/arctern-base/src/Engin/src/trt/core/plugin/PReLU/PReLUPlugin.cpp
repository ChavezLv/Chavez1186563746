#include "PReLUKernel.h"
#include "PReLUPlugin.h"

namespace
{
static const char* PRELU_PLUGIN_VERSION{"1"};
static const char* PRELU_PLUGIN_NAME{"PReLU_ARCTERN"};
} // namespace

// Static class fields initialization
PluginFieldCollection PReLUPluginCreator::mFC{};
std::vector<PluginField> PReLUPluginCreator::mPluginAttributes;

PReLUPlugin::PReLUPlugin(const std::string name, std::vector<float> alpha)
    : BasePlugin(name)
{
    mAlpha = alpha;
    mWeightsAlpha = copyToDevice<float>(alpha.data(), alpha.size());
}

PReLUPlugin::PReLUPlugin(const std::string name, const std::vector<Dims>& inputShape,
    const std::vector<Dims>& outputShape, std::vector<float> alpha)
    : BasePlugin(name, inputShape, outputShape)
{
    mAlpha = alpha;
    mWeightsAlpha = copyToDevice<float>(alpha.data(), alpha.size());
}

PReLUPlugin::PReLUPlugin(const std::string name, const void* data, size_t length)
    : BasePlugin(name)
{
    deserializeBase(data, length);
    deserialize_value(&data, &length, &mAlpha);
    mWeightsAlpha = copyToDevice<float>(mAlpha.data(), mAlpha.size());
}

const char* PReLUPlugin::getPluginType() const
{
    return PRELU_PLUGIN_NAME;
}

const char* PReLUPlugin::getPluginVersion() const
{
    return PRELU_PLUGIN_VERSION;
}

int PReLUPlugin::getNbOutputs() const
{
    return 1;
}

Dims PReLUPlugin::getOutputDimensions(int index, const Dims* inputs, int nbInputDims)
{
    ASSERT(index == 0);
    ASSERT(nbInputDims == 1);

    Dims outputShape{};
    outputShape.nbDims = inputs[prelu_enum::kData].nbDims;
    for (int d = 0; d < inputs[prelu_enum::kData].nbDims; ++d)
    {
        outputShape.type[d] = inputs[prelu_enum::kData].type[d];
        outputShape.d[d] = inputs[prelu_enum::kData].d[d];
    }

    return outputShape;
}

bool PReLUPlugin::supportsFormat(DataType type, PluginFormat format) const
{
    return type == DataType::kFLOAT && format == PluginFormat::kNCHW;
}

int PReLUPlugin::initialize()
{
    int nbOutput = getNbOutputs();
    for (int i = 0; i < nbOutput; ++i)
    {
        mOutputShape.push_back(getOutputDimensions(i, &mInputShape[0], static_cast<int>(mInputShape.size())));
    }

    return 0;
}

void PReLUPlugin::terminate() {}

int PReLUPlugin::enqueue(int batchSize, const void* const* inputs, void** outputs, void* workspace, cudaStream_t stream)
{
    int num_threads = batchSize;
    for (int i = 0; i < mInputShape[prelu_enum::kData].nbDims; ++i)
    {
        num_threads *= mInputShape[prelu_enum::kData].d[i];
    }
    pluginStatus_t status = PReLUInference(stream, num_threads, outputs[prelu_enum::kOut], inputs[prelu_enum::kData],
        static_cast<const void*>(mWeightsAlpha.values), mInputShape[prelu_enum::kData].nbDims,
        mInputShape[prelu_enum::kData].d);
    ASSERT(status == STATUS_SUCCESS);

    return 0;
}

size_t PReLUPlugin::getSerializationSize() const
{
    return (getBaseSerializationSize() + serialized_size(mAlpha));
}

void PReLUPlugin::serialize(void* buffer) const
{
    serializeBase(buffer);
    serialize_value(&buffer, mAlpha);
}

IPluginV2Ext* PReLUPlugin::clone() const
{
    return new PReLUPlugin(mLayerName, mInputShape, mOutputShape, mAlpha);
}

DataType PReLUPlugin::getOutputDataType(int index, const nvinfer1::DataType* inputTypes, int nbInputs) const
{
    // one outputs
    ASSERT(index == 0)
    return DataType::kFLOAT;
}

void PReLUPlugin::configurePlugin(const Dims* inputDims, int nbInputs, const Dims* outputDims, int nbOutputs,
    const DataType* inputTypes, const DataType* outputTypes, const bool* inputIsBroadcast,
    const bool* outputIsBroadcast, PluginFormat floatFormat, int maxBatchSize)
{
    ASSERT(nbInputs == 1);
    ASSERT(nbOutputs == 1);
    ASSERT(inputDims[prelu_enum::kData].nbDims > 1);
    ASSERT(inputDims[prelu_enum::kData].d[1] == mAlpha.size());
    ASSERT(inputTypes[0] == DataType::kFLOAT)
    ASSERT(floatFormat == PluginFormat::kNCHW)

    mInputShape.assign(inputDims, inputDims + nbInputs);
}

PReLUPluginCreator::PReLUPluginCreator()
{
    mPluginAttributes.emplace_back(PluginField("alpha", nullptr, PluginFieldType::kFLOAT32, 1));

    mFC.nbFields = mPluginAttributes.size();
    mFC.fields = mPluginAttributes.data();
}

const char* PReLUPluginCreator::getPluginName() const
{
    return PRELU_PLUGIN_NAME;
}

const char* PReLUPluginCreator::getPluginVersion() const
{
    return PRELU_PLUGIN_VERSION;
}

const PluginFieldCollection* PReLUPluginCreator::getFieldNames()
{
    return &mFC;
}

IPluginV2Ext* PReLUPluginCreator::createPlugin(const char* name, const PluginFieldCollection* fc)
{
    std::vector<float> alpha;
    const PluginField* fields = fc->fields;
    for (int i = 0; i < fc->nbFields; ++i)
    {
        const char* attrName = fields[i].name;
        if (!strcmp(attrName, "alpha"))
        {
            ASSERT(fields[i].type == PluginFieldType::kFLOAT32);
            int size = fields[i].length;
            alpha.resize(size);
            memcpy(alpha.data(), fields[i].data, sizeof(float) * size);
        }
    }

    IPluginV2Ext* plugin = new PReLUPlugin(name, alpha);
    plugin->setPluginNamespace(mNamespace.c_str());
    return plugin;
}

IPluginV2Ext* PReLUPluginCreator::deserializePlugin(const char* name, const void* serialData, size_t serialLength)
{
    PReLUPlugin* plugin = new PReLUPlugin(name, serialData, serialLength);
    plugin->setPluginNamespace(mNamespace.c_str());
    return plugin;
}

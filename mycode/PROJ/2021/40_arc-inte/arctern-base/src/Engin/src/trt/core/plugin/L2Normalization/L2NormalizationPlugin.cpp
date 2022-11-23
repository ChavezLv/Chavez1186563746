#include "L2NormalizationKernel.h"
#include "L2NormalizationPlugin.h"

namespace
{
static const char* L2_NORMALIZATION_PLUGIN_VERSION{"1"};
static const char* L2_NORMALIZATION_PLUGIN_NAME{"L2Normalization_ARCTERN"};
} // namespace

// Static class fields initialization
PluginFieldCollection L2NormalizationPluginCreator::mFC{};
std::vector<PluginField> L2NormalizationPluginCreator::mPluginAttributes;

L2NormalizationPlugin::L2NormalizationPlugin(const std::string name, float eps)
    : BasePlugin(name)
    , mEps(eps)
{
}

L2NormalizationPlugin::L2NormalizationPlugin(
    const std::string name, const std::vector<Dims>& inputShape, const std::vector<Dims>& outputShape, float eps)
    : BasePlugin(name, inputShape, outputShape)
    , mEps(eps)
{
}

L2NormalizationPlugin::L2NormalizationPlugin(const std::string name, const void* data, size_t length)
    : BasePlugin(name)
{
    deserializeBase(data, length);
    deserialize_value(&data, &length, &mEps);
}

const char* L2NormalizationPlugin::getPluginType() const
{
    return L2_NORMALIZATION_PLUGIN_NAME;
}

const char* L2NormalizationPlugin::getPluginVersion() const
{
    return L2_NORMALIZATION_PLUGIN_VERSION;
}

int L2NormalizationPlugin::getNbOutputs() const
{
    return 1;
}

Dims L2NormalizationPlugin::getOutputDimensions(int index, const Dims* inputs, int nbInputDims)
{
    ASSERT(index == 0);
    ASSERT(nbInputDims == 1);

    Dims outputShape{};
    outputShape.nbDims = inputs[l2normalization_enum::kData].nbDims;
    for (int d = 0; d < inputs[l2normalization_enum::kData].nbDims; ++d)
    {
        outputShape.type[d] = inputs[l2normalization_enum::kData].type[d];
        outputShape.d[d] = inputs[l2normalization_enum::kData].d[d];
    }

    return outputShape;
}

bool L2NormalizationPlugin::supportsFormat(DataType type, PluginFormat format) const
{
    return type == DataType::kFLOAT && format == PluginFormat::kNCHW;
}

int L2NormalizationPlugin::initialize()
{
    int nbOutput = getNbOutputs();
    for (int i = 0; i < nbOutput; ++i)
    {
        mOutputShape.push_back(getOutputDimensions(i, &mInputShape[0], static_cast<int>(mInputShape.size())));
    }

    return 0;
}

void L2NormalizationPlugin::terminate() {}

int L2NormalizationPlugin::enqueue(
    int batchSize, const void* const* inputs, void** outputs, void* workspace, cudaStream_t stream)
{
    int num_threads = 1;
    for (int i = 0; i < mInputShape[l2normalization_enum::kData].nbDims; ++i)
    {
        num_threads *= mInputShape[l2normalization_enum::kData].d[i];
    }
    L2NormalizationInference(
        stream, outputs[l2normalization_enum::kOut], inputs[l2normalization_enum::kData], batchSize, num_threads, mEps);
    return 0;
}

size_t L2NormalizationPlugin::getSerializationSize() const
{
    return (getBaseSerializationSize() + serialized_size(mEps));
}

void L2NormalizationPlugin::serialize(void* buffer) const
{
    serializeBase(buffer);
    serialize_value(&buffer, mEps);
}

IPluginV2Ext* L2NormalizationPlugin::clone() const
{
    return new L2NormalizationPlugin(mLayerName, mInputShape, mOutputShape, mEps);
}

DataType L2NormalizationPlugin::getOutputDataType(int index, const nvinfer1::DataType* inputTypes, int nbInputs) const
{
    // one outputs
    ASSERT(index == 0)
    return DataType::kFLOAT;
}

void L2NormalizationPlugin::configurePlugin(const Dims* inputDims, int nbInputs, const Dims* outputDims, int nbOutputs,
    const DataType* inputTypes, const DataType* outputTypes, const bool* inputIsBroadcast,
    const bool* outputIsBroadcast, PluginFormat floatFormat, int maxBatchSize)
{
    ASSERT(nbInputs == 1);
    ASSERT(nbOutputs == 1);
    ASSERT(inputDims[l2normalization_enum::kData].nbDims > 1);

    mInputShape.assign(inputDims, inputDims + nbInputs);
}

L2NormalizationPluginCreator::L2NormalizationPluginCreator()
{
    mPluginAttributes.emplace_back(PluginField("eps", nullptr, PluginFieldType::kFLOAT32, 1));

    mFC.nbFields = mPluginAttributes.size();
    mFC.fields = mPluginAttributes.data();
}

const char* L2NormalizationPluginCreator::getPluginName() const
{
    return L2_NORMALIZATION_PLUGIN_NAME;
}

const char* L2NormalizationPluginCreator::getPluginVersion() const
{
    return L2_NORMALIZATION_PLUGIN_VERSION;
}

const PluginFieldCollection* L2NormalizationPluginCreator::getFieldNames()
{
    return &mFC;
}

IPluginV2Ext* L2NormalizationPluginCreator::createPlugin(const char* name, const PluginFieldCollection* fc)
{
    float eps = 1e-10;
    const PluginField* fields = fc->fields;

    for (int i = 0; i < fc->nbFields; ++i)
    {
        const char* attrName = fields[i].name;
        if (!strcmp(attrName, "eps"))
        {
            ASSERT(fields[i].type == PluginFieldType::kFLOAT32);
            eps = *(static_cast<const float*>(fields[i].data));
        }
    }

    IPluginV2Ext* plugin = new L2NormalizationPlugin(name, eps);
    plugin->setPluginNamespace(mNamespace.c_str());
    return plugin;
}

IPluginV2Ext* L2NormalizationPluginCreator::deserializePlugin(
    const char* name, const void* serialData, size_t serialLength)
{
    L2NormalizationPlugin* plugin = new L2NormalizationPlugin(name, serialData, serialLength);
    plugin->setPluginNamespace(mNamespace.c_str());
    return plugin;
}

#include "LastDimTopKKernel.h"
#include "LastDimTopKPlugin.h"

namespace
{
static const char* LASTDIMTOPK_PLUGIN_VERSION{"1"};
static const char* LASTDIMTOPK_PLUGIN_NAME{"LastDimTopK_ARCTERN"};
} // namespace

// Static class fields initialization
PluginFieldCollection LastDimTopKPluginCreator::mFC{};
std::vector<PluginField> LastDimTopKPluginCreator::mPluginAttributes;

LastDimTopKPlugin::LastDimTopKPlugin(const std::string name, bool is_ascend, int ret_typ, int k)
    : BasePlugin(name)
    , mIsAscend(is_ascend)
    , mReturnType(ret_typ)
    , mK(k)
{
}

LastDimTopKPlugin::LastDimTopKPlugin(const std::string name, const std::vector<Dims>& inputShape,
    const std::vector<Dims>& outputShape, bool is_ascend, int ret_typ, int k)
    : BasePlugin(name, inputShape, outputShape)
    , mIsAscend(is_ascend)
    , mReturnType(ret_typ)
    , mK(k)
{
}

LastDimTopKPlugin::LastDimTopKPlugin(const std::string name, const void* data, size_t length)
    : BasePlugin(name)
{
    deserializeBase(data, length);
    deserialize_value(&data, &length, &mIsAscend);
    deserialize_value(&data, &length, &mReturnType);
    deserialize_value(&data, &length, &mK);
}

const char* LastDimTopKPlugin::getPluginType() const
{
    return LASTDIMTOPK_PLUGIN_NAME;
}

const char* LastDimTopKPlugin::getPluginVersion() const
{
    return LASTDIMTOPK_PLUGIN_VERSION;
}

int LastDimTopKPlugin::getNbOutputs() const
{
    return 2;
}

Dims LastDimTopKPlugin::getOutputDimensions(int index, const Dims* inputs, int nbInputDims)
{
    ASSERT(index < 2);
    ASSERT(nbInputDims == 1);

    Dims outputShape{};
    outputShape.nbDims = inputs[lastdimtopk_enum::kData].nbDims;
    for (int d = 0; d < inputs[lastdimtopk_enum::kData].nbDims - 1; ++d)
    {
        outputShape.type[d] = inputs[lastdimtopk_enum::kData].type[d];
        outputShape.d[d] = inputs[lastdimtopk_enum::kData].d[d];
    }
    outputShape.type[inputs[lastdimtopk_enum::kData].nbDims - 1] = DimensionType::kSPATIAL;
    outputShape.d[inputs[lastdimtopk_enum::kData].nbDims - 1] = mK;

    return outputShape;
}

bool LastDimTopKPlugin::supportsFormat(DataType type, PluginFormat format) const
{
    return type == DataType::kFLOAT && format == PluginFormat::kNCHW;
}

int LastDimTopKPlugin::initialize()
{
    int nbOutput = getNbOutputs();
    for (int i = 0; i < nbOutput; ++i)
    {
        mOutputShape.push_back(getOutputDimensions(i, &mInputShape[0], static_cast<int>(mInputShape.size())));
    }

    return 0;
}

void LastDimTopKPlugin::terminate() {}

int LastDimTopKPlugin::enqueue(
    int batchSize, const void* const* inputs, void** outputs, void* workspace, cudaStream_t stream)
{
    int num_threads = batchSize;
    for (int i = 0; i < mInputShape[lastdimtopk_enum::kData].nbDims - 1; ++i) {
        num_threads *= mInputShape[lastdimtopk_enum::kData].d[i];
    }
    auto status = LastDimTopKInference(stream, num_threads, outputs[lastdimtopk_enum::kValue],
        outputs[lastdimtopk_enum::kIndices], inputs[lastdimtopk_enum::kData], mK,
        mInputShape[lastdimtopk_enum::kData].d[mInputShape[lastdimtopk_enum::kData].nbDims - 1], mIsAscend);
    ASSERT(status == STATUS_SUCCESS);

    return 0;
}

size_t LastDimTopKPlugin::getSerializationSize() const
{
    return (
        getBaseSerializationSize() + serialized_size(mIsAscend) + serialized_size(mReturnType) + serialized_size(mK));
}

void LastDimTopKPlugin::serialize(void* buffer) const
{
    serializeBase(buffer);
    serialize_value(&buffer, mIsAscend);
    serialize_value(&buffer, mReturnType);
    serialize_value(&buffer, mK);
}

IPluginV2Ext* LastDimTopKPlugin::clone() const
{
    return new LastDimTopKPlugin(mLayerName, mInputShape, mOutputShape, mIsAscend, mReturnType, mK);
}

DataType LastDimTopKPlugin::getOutputDataType(int index, const nvinfer1::DataType* inputTypes, int nbInputs) const
{
    // one outputs
    ASSERT(index < 2)
    return DataType::kFLOAT;
}

void LastDimTopKPlugin::configurePlugin(const Dims* inputDims, int nbInputs, const Dims* outputDims, int nbOutputs,
    const DataType* inputTypes, const DataType* outputTypes, const bool* inputIsBroadcast,
    const bool* outputIsBroadcast, PluginFormat floatFormat, int maxBatchSize)
{
    ASSERT(nbInputs == 1);
    ASSERT(nbOutputs == 2);

    mInputShape.assign(inputDims, inputDims + nbInputs);
}

LastDimTopKPluginCreator::LastDimTopKPluginCreator()
{
    mPluginAttributes.emplace_back(PluginField("is_ascend", nullptr, PluginFieldType::kINT32, 1));
    mPluginAttributes.emplace_back(PluginField("ret_typ", nullptr, PluginFieldType::kINT32, 1));
    mPluginAttributes.emplace_back(PluginField("k", nullptr, PluginFieldType::kINT32, 1));

    mFC.nbFields = static_cast<int>(mPluginAttributes.size());
    mFC.fields = mPluginAttributes.data();
}

const char* LastDimTopKPluginCreator::getPluginName() const
{
    return LASTDIMTOPK_PLUGIN_NAME;
}

const char* LastDimTopKPluginCreator::getPluginVersion() const
{
    return LASTDIMTOPK_PLUGIN_VERSION;
}

const PluginFieldCollection* LastDimTopKPluginCreator::getFieldNames()
{
    return &mFC;
}

IPluginV2Ext* LastDimTopKPluginCreator::createPlugin(const char* name, const PluginFieldCollection* fc)
{
    bool is_ascend = false;
    int ret_typ = lastdimtopk_enum::kReturnIndices;
    int k = 1;
    const PluginField* fields = fc->fields;
    for (int i = 0; i < fc->nbFields; ++i)
    {
        const char* attrName = fields[i].name;
        if (strcmp(attrName, "is_ascend") == 0)
        {
            ASSERT(fields[i].type == PluginFieldType::kINT32);
            is_ascend = *(static_cast<const int*>(fields[i].data));
        }
        else if (strcmp(attrName, "ret_typ") == 0)
        {
            ASSERT(fields[i].type == PluginFieldType::kINT32);
            ret_typ = *(static_cast<const int*>(fields[i].data));
        }
        else if (strcmp(attrName, "k") == 0)
        {
            ASSERT(fields[i].type == PluginFieldType::kINT32);
            k = *(static_cast<const int*>(fields[i].data));
        }
    }

    IPluginV2Ext* plugin = new LastDimTopKPlugin(name, is_ascend, ret_typ, k);
    plugin->setPluginNamespace(mNamespace.c_str());
    return plugin;
}

IPluginV2Ext* LastDimTopKPluginCreator::deserializePlugin(const char* name, const void* serialData, size_t serialLength)
{
    LastDimTopKPlugin* plugin = new LastDimTopKPlugin(name, serialData, serialLength);
    plugin->setPluginNamespace(mNamespace.c_str());
    return plugin;
}

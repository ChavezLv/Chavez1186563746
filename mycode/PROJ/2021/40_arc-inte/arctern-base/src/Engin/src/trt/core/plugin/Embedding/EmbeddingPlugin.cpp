#include "EmbeddingKernel.h"
#include "EmbeddingPlugin.h"

namespace
{
static const char* EMBEDDING_PLUGIN_VERSION{"1"};
static const char* EMBEDDING_PLUGIN_NAME{"Embedding_ARCTERN"};
} // namespace

// Static class fields initialization
PluginFieldCollection EmbeddingPluginCreator::mFC{};
std::vector<PluginField> EmbeddingPluginCreator::mPluginAttributes;

EmbeddingPlugin::EmbeddingPlugin(const std::string name, int input_dim, int output_dim, bool sparse_grad)
    : BasePlugin(name)
    , mInputDim(input_dim)
    , mOutputDim(output_dim)
    , mSparseGrad(sparse_grad)
{
}

EmbeddingPlugin::EmbeddingPlugin(const std::string name, const std::vector<Dims>& inputShape,
    const std::vector<Dims>& outputShape, int input_dim, int output_dim, bool sparse_grad)
    : BasePlugin(name, inputShape, outputShape)
    , mInputDim(input_dim)
    , mOutputDim(output_dim)
    , mSparseGrad(sparse_grad)
{
}

EmbeddingPlugin::EmbeddingPlugin(const std::string name, const void* data, size_t length)
    : BasePlugin(name)
{
    deserializeBase(data, length);
    deserialize_value(&data, &length, &mInputDim);
    deserialize_value(&data, &length, &mOutputDim);
    deserialize_value(&data, &length, &mSparseGrad);
}

const char* EmbeddingPlugin::getPluginType() const
{
    return EMBEDDING_PLUGIN_NAME;
}

const char* EmbeddingPlugin::getPluginVersion() const
{
    return EMBEDDING_PLUGIN_VERSION;
}

int EmbeddingPlugin::getNbOutputs() const
{
    return 1;
}

Dims EmbeddingPlugin::getOutputDimensions(int index, const Dims* inputs, int nbInputDims)
{
    ASSERT(index == 0);
    ASSERT(nbInputDims == 2);

    Dims outputShape{};
    outputShape.nbDims = inputs[embedding_enum::kData].nbDims + 1;
    for (int d = 0; d < inputs[embedding_enum::kData].nbDims; ++d)
    {
        outputShape.type[d] = inputs[embedding_enum::kData].type[d];
        outputShape.d[d] = inputs[embedding_enum::kData].d[d];
    }
    outputShape.type[inputs[embedding_enum::kData].nbDims] = DimensionType::kSPATIAL;
    outputShape.d[inputs[embedding_enum::kData].nbDims] = mOutputDim;

    return outputShape;
}

bool EmbeddingPlugin::supportsFormat(DataType type, PluginFormat format) const
{
    return type == DataType::kFLOAT && format == PluginFormat::kNCHW;
}

int EmbeddingPlugin::initialize()
{
    int nbOutput = getNbOutputs();
    for (int i = 0; i < nbOutput; ++i)
    {
        mOutputShape.push_back(getOutputDimensions(i, &mInputShape[0], static_cast<int>(mInputShape.size())));
    }

    return 0;
}

void EmbeddingPlugin::terminate() {}

int EmbeddingPlugin::enqueue(
    int batchSize, const void* const* inputs, void** outputs, void* workspace, cudaStream_t stream)
{
    int num_threads = batchSize;
    for (int i = 0; i < mOutputShape[embedding_enum::kOut].nbDims; ++i)
    {
        num_threads *= mOutputShape[embedding_enum::kOut].d[i];
    }
    auto status = EmbeddingInference(stream, num_threads, outputs[embedding_enum::kOut],
        inputs[embedding_enum::kWeight], inputs[embedding_enum::kData], mOutputDim, mInputDim);
    ASSERT(status == STATUS_SUCCESS);

    return 0;
}

size_t EmbeddingPlugin::getSerializationSize() const
{
    return (getBaseSerializationSize() + serialized_size(mInputDim) + serialized_size(mOutputDim)
        + serialized_size(mSparseGrad));
}

void EmbeddingPlugin::serialize(void* buffer) const
{
    serializeBase(buffer);
    serialize_value(&buffer, mInputDim);
    serialize_value(&buffer, mOutputDim);
    serialize_value(&buffer, mSparseGrad);
}

IPluginV2Ext* EmbeddingPlugin::clone() const
{
    return new EmbeddingPlugin(mLayerName, mInputShape, mOutputShape, mInputDim, mOutputDim, mSparseGrad);
}

DataType EmbeddingPlugin::getOutputDataType(int index, const nvinfer1::DataType* inputTypes, int nbInputs) const
{
    // one outputs
    ASSERT(index == 0)
    return DataType::kFLOAT;
}

void EmbeddingPlugin::configurePlugin(const Dims* inputDims, int nbInputs, const Dims* outputDims, int nbOutputs,
    const DataType* inputTypes, const DataType* outputTypes, const bool* inputIsBroadcast,
    const bool* outputIsBroadcast, PluginFormat floatFormat, int maxBatchSize)
{
    ASSERT(nbInputs == 2);
    ASSERT(nbOutputs == 1);
    ASSERT(inputDims[embedding_enum::kWeight].d[0] == mInputDim);
    ASSERT(inputDims[embedding_enum::kWeight].d[1] == mOutputDim);

    mInputShape.assign(inputDims, inputDims + nbInputs);
}

EmbeddingPluginCreator::EmbeddingPluginCreator()
{
    mPluginAttributes.emplace_back(PluginField("input_dim", nullptr, PluginFieldType::kINT32, 1));
    mPluginAttributes.emplace_back(PluginField("output_dim", nullptr, PluginFieldType::kINT32, 1));
    mPluginAttributes.emplace_back(PluginField("sparse_grad", nullptr, PluginFieldType::kINT32, 1));

    mFC.nbFields = static_cast<int>(mPluginAttributes.size());
    mFC.fields = mPluginAttributes.data();
}

const char* EmbeddingPluginCreator::getPluginName() const
{
    return EMBEDDING_PLUGIN_NAME;
}

const char* EmbeddingPluginCreator::getPluginVersion() const
{
    return EMBEDDING_PLUGIN_VERSION;
}

const PluginFieldCollection* EmbeddingPluginCreator::getFieldNames()
{
    return &mFC;
}

IPluginV2Ext* EmbeddingPluginCreator::createPlugin(const char* name, const PluginFieldCollection* fc)
{
    int input_dim = 0;
    int output_dim = 0;
    bool sparse_grad = false;
    const PluginField* fields = fc->fields;
    for (int i = 0; i < fc->nbFields; ++i)
    {
        const char* attrName = fields[i].name;
        if (strcmp(attrName, "input_dim") == 0)
        {
            ASSERT(fields[i].type == PluginFieldType::kINT32);
            input_dim = *(static_cast<const int*>(fields[i].data));
        }
        else if (strcmp(attrName, "output_dim") == 0)
        {
            ASSERT(fields[i].type == PluginFieldType::kINT32);
            output_dim = *(static_cast<const int*>(fields[i].data));
        }
        else if (strcmp(attrName, "sparse_grad") == 0)
        {
            ASSERT(fields[i].type == PluginFieldType::kINT32);
            sparse_grad = *(static_cast<const int*>(fields[i].data));
        }
    }

    IPluginV2Ext* plugin = new EmbeddingPlugin(name, input_dim, output_dim, sparse_grad);
    plugin->setPluginNamespace(mNamespace.c_str());
    return plugin;
}

IPluginV2Ext* EmbeddingPluginCreator::deserializePlugin(const char* name, const void* serialData, size_t serialLength)
{
    EmbeddingPlugin* plugin = new EmbeddingPlugin(name, serialData, serialLength);
    plugin->setPluginNamespace(mNamespace.c_str());
    return plugin;
}

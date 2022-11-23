#include "GridGeneratorPlugin.h"
#include <thread>
#include <mutex>

static std::mutex mtx;

using namespace arctern::trtPlugin;

namespace
{
static const char* GRID_GENERATOR_PLUGIN_VERSION{"1"};
static const char* GRID_GENERATOR_PLUGIN_NAME{"GridGenerator_ARCTERN"};
} // namespace

// Static class fields initialization
PluginFieldCollection GridGeneratorPluginCreator::mFC{};
std::vector<PluginField> GridGeneratorPluginCreator::mPluginAttributes;

GridGeneratorPlugin::GridGeneratorPlugin(const std::string name, std::vector<int> targetShape, int transferType)
    : BasePlugin(name)
    , mInitialized(false)
    , mTargetShape(targetShape)
    , mTransformType(transferType)
{
    ASSERT(targetShape.size() == 2);
}

GridGeneratorPlugin::GridGeneratorPlugin(const std::string name, const std::vector<Dims>& inputShape,
    const std::vector<Dims>& outputShape, std::vector<int> targetShape, int transferType)
    : BasePlugin(name, inputShape, outputShape)
    , mInitialized(false)
    , mTargetShape(targetShape)
    , mTransformType(transferType)
{
    ASSERT(targetShape.size() == 2);
}

GridGeneratorPlugin::GridGeneratorPlugin(const std::string name, const void* data, size_t length)
    : BasePlugin(name)
    , mInitialized(false)
{
    deserializeBase(data, length);
    deserialize_value(&data, &length, &mTargetShape);
    deserialize_value(&data, &length, &mTransformType);
}

const char* GridGeneratorPlugin::getPluginType() const
{
    return GRID_GENERATOR_PLUGIN_NAME;
}

const char* GridGeneratorPlugin::getPluginVersion() const
{
    return GRID_GENERATOR_PLUGIN_VERSION;
}

int GridGeneratorPlugin::getNbOutputs() const
{
    return 1;
}

Dims GridGeneratorPlugin::getOutputDimensions(int index, const Dims* inputs, int nbInputDims)
{
    ASSERT(index == 0);
    ASSERT(nbInputDims == 1);
    ASSERT(mTransformType == grid_enum::kAffine);

    ASSERT(inputs[grid_enum::kLoc].nbDims == 2);
    if (mTransformType == grid_enum::kAffine)
    {
        ASSERT(inputs[grid_enum::kLoc].d[1] == 6);
    }

    ASSERT(mTargetShape[0] > 0);
    ASSERT(mTargetShape[1] > 0);
    Dims gridSrcShape{};
    gridSrcShape.nbDims = 4;
    gridSrcShape.type[0] = DimensionType::kINDEX;
    gridSrcShape.type[1] = DimensionType::kCHANNEL;
    gridSrcShape.type[2] = DimensionType::kSPATIAL;
    gridSrcShape.type[3] = DimensionType::kSPATIAL;
    gridSrcShape.d[0] = inputs[grid_enum::kLoc].d[0];
    gridSrcShape.d[1] = 2;
    gridSrcShape.d[2] = mTargetShape[0];
    gridSrcShape.d[3] = mTargetShape[1];


    return gridSrcShape;
}

bool GridGeneratorPlugin::supportsFormat(DataType type, PluginFormat format) const
{
    return type == DataType::kFLOAT && format == PluginFormat::kNCHW;
}

void GridGeneratorPlugin::cudnnInitialize()
{
    CUDNNASSERT(cudnnCreate(&mCudnn))
    // Create cudnn tensor descriptors
    CUDNNASSERT(cudnnCreateSpatialTransformerDescriptor(&mSTDesc))
    mInitialized = true;
}

int GridGeneratorPlugin::initialize()
{
    int nbOutput = getNbOutputs();
    for (int i = 0; i < nbOutput; ++i)
    {
        mOutputShape.push_back(getOutputDimensions(i, &mInputShape[0], static_cast<int>(mInputShape.size())));
    }

    if (!mInitialized)
    {
        cudnnInitialize();
    }
    return 0;
}

void GridGeneratorPlugin::terminate()
{
    if (!mInitialized)
    {
        return;
    }
    CUDNNASSERT(cudnnDestroySpatialTransformerDescriptor(mSTDesc));
    CUDNNASSERT(cudnnDestroy(mCudnn));
    mInitialized = false;
}

int GridGeneratorPlugin::enqueue(
    int batchSize, const void* const* inputs, void** outputs, void* workspace, cudaStream_t stream)
{
    /// mCudnn, mInDesc, mOutDesc, mSTDesc are shared in multi-thread, so must be has lock
    std::lock_guard<std::mutex> lk(mtx);

    if (!mInitialized)
    {
        cudnnInitialize();
    }
    ASSERT(mInitialized)

    int dim[] = {mOutputShape[grid_enum::kGridSrc].d[0] * batchSize, mOutputShape[grid_enum::kGridSrc].d[1],
        mOutputShape[grid_enum::kGridSrc].d[2], mOutputShape[grid_enum::kGridSrc].d[3]};
    cudnnDataType_t cudnn_data_type = CUDNN_DATA_FLOAT;

    CUDNNASSERT(cudnnSetSpatialTransformerNdDescriptor(mSTDesc, CUDNN_SAMPLER_BILINEAR, cudnn_data_type, 4, dim));

    CUDNNASSERT(cudnnSetStream(mCudnn, stream));
    if (mTransformType == grid_enum::kAffine)
    {
        CUDNNASSERT(
            cudnnSpatialTfGridGeneratorForward(mCudnn, mSTDesc, reinterpret_cast<const float*>(inputs[grid_enum::kLoc]),
                reinterpret_cast<float*>(outputs[grid_enum::kGridSrc])));

        cudaStreamSynchronize(stream);
    }
    return 0;
}

size_t GridGeneratorPlugin::getSerializationSize() const
{
    return (getBaseSerializationSize() + serialized_size(mTargetShape) + serialized_size(mTransformType));
}

void GridGeneratorPlugin::serialize(void* buffer) const
{
    serializeBase(buffer);
    serialize_value(&buffer, mTargetShape);
    serialize_value(&buffer, mTransformType);
}

IPluginV2Ext* GridGeneratorPlugin::clone() const
{
    return new GridGeneratorPlugin(mLayerName, mInputShape, mOutputShape, mTargetShape, mTransformType);
}

DataType GridGeneratorPlugin::getOutputDataType(int index, const nvinfer1::DataType* inputTypes, int nbInputs) const
{
    // one outputs
    ASSERT(index == 0)
    return DataType::kFLOAT;
}

void GridGeneratorPlugin::configurePlugin(const Dims* inputDims, int nbInputs, const Dims* outputDims, int nbOutputs,
    const DataType* inputTypes, const DataType* outputTypes, const bool* inputIsBroadcast,
    const bool* outputIsBroadcast, PluginFormat floatFormat, int maxBatchSize)
{
    ASSERT(nbInputs == 1)
    ASSERT(nbOutputs == 1)
    ASSERT(inputTypes[0] == DataType::kFLOAT)
    ASSERT(floatFormat == PluginFormat::kNCHW)

    mInputShape.assign(inputDims, inputDims + nbInputs);
}

GridGeneratorPluginCreator::GridGeneratorPluginCreator() {}

const char* GridGeneratorPluginCreator::getPluginName() const
{
    return GRID_GENERATOR_PLUGIN_NAME;
}

const char* GridGeneratorPluginCreator::getPluginVersion() const
{
    return GRID_GENERATOR_PLUGIN_VERSION;
}

const PluginFieldCollection* GridGeneratorPluginCreator::getFieldNames()
{
    return &mFC;
}

IPluginV2Ext* GridGeneratorPluginCreator::createPlugin(const char* name, const PluginFieldCollection* fc)
{
    std::vector<int> targetShape{};
    int transferType = 0;
    const PluginField* fields = fc->fields;

    // Parse fields from PluginFieldCollection
    ASSERT(fc->nbFields == 2);
    for (int i = 0; i < fc->nbFields; i++)
    {
        const char* attrName = fields[i].name;
        if (strcmp(attrName, "target_shape") == 0)
        {
            ASSERT(fields[i].type == PluginFieldType::kINT32);
            int pad_width_num = fields[i].length;
            targetShape.resize(static_cast<unsigned long>(pad_width_num));
            memcpy(targetShape.data(), fields[i].data, sizeof(int) * pad_width_num);
        }
        else if (strcmp(attrName, "transfer_type") == 0)
        {
            ASSERT(fields[i].type == PluginFieldType::kINT32);
            transferType = *(static_cast<const int*>(fields[i].data));
        }
    }

    ASSERT(transferType == grid_enum::kAffine);

    IPluginV2Ext* plugin = new GridGeneratorPlugin(name, targetShape, transferType);
    plugin->setPluginNamespace(mNamespace.c_str());
    return plugin;
}

IPluginV2Ext* GridGeneratorPluginCreator::deserializePlugin(
    const char* name, const void* serialData, size_t serialLength)
{
    GridGeneratorPlugin* plugin = new GridGeneratorPlugin(name, serialData, serialLength);
    plugin->setPluginNamespace(mNamespace.c_str());
    return plugin;
}

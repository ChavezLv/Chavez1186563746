#include "SpatialTransformerPlugin.h"
#include <thread>
#include <mutex>

static std::mutex mtx;

using namespace arctern::trtPlugin;

namespace
{
static const char* SPATIAL_TRANSFORMER_PLUGIN_VERSION{"1"};
static const char* SPATIAL_TRANSFORMER_PLUGIN_NAME{"SpatialTransformer_ARCTERN"};
} // namespace

// Static class fields initialization
PluginFieldCollection SpatialTransformerPluginCreator::mFC{};
std::vector<PluginField> SpatialTransformerPluginCreator::mPluginAttributes;

SpatialTransformerPlugin::SpatialTransformerPlugin(
    const std::string name, std::vector<int> targetShape, int transferType, int samplerType)
    : BasePlugin(name)
    , mInitialized(false)
    , mTargetShape(targetShape)
    , mTransformType(transferType)
{
    ASSERT(samplerType == st::kBilinear);
    mSamplerType = CUDNN_SAMPLER_BILINEAR;
    ASSERT(targetShape.size() == 2);
}

SpatialTransformerPlugin::SpatialTransformerPlugin(const std::string name, const std::vector<Dims>& inputShape,
    const std::vector<Dims>& outputShape, std::vector<int> targetShape, int transferType, int samplerType)
    : BasePlugin(name, inputShape, outputShape)
    , mInitialized(false)
    , mTargetShape(targetShape)
    , mTransformType(transferType)
{
    ASSERT(samplerType == st::kBilinear);
    mSamplerType = CUDNN_SAMPLER_BILINEAR;
    ASSERT(targetShape.size() == 2);
}

SpatialTransformerPlugin::SpatialTransformerPlugin(const std::string name, const void* data, size_t length)
    : BasePlugin(name)
    , mInitialized(false)
{
    deserializeBase(data, length);
    deserialize_value(&data, &length, &mSamplerType);
    deserialize_value(&data, &length, &mTargetShape);
    deserialize_value(&data, &length, &mTransformType);
}

const char* SpatialTransformerPlugin::getPluginType() const
{
    return SPATIAL_TRANSFORMER_PLUGIN_NAME;
}

const char* SpatialTransformerPlugin::getPluginVersion() const
{
    return SPATIAL_TRANSFORMER_PLUGIN_VERSION;
}

int SpatialTransformerPlugin::getNbOutputs() const
{
    return 3;
}

Dims SpatialTransformerPlugin::getOutputDimensions(int index, const Dims* inputs, int nbInputDims)
{
    ASSERT(index < 3);
    ASSERT(nbInputDims == 2);
    ASSERT(mTransformType == st::kAffine);
    ASSERT(mSamplerType == CUDNN_SAMPLER_BILINEAR);

    ASSERT(inputs[st::kData].nbDims == 4);
    ASSERT(inputs[st::kLoc].nbDims == 2);
    if (mTransformType == st::kAffine)
    {
        ASSERT(inputs[st::kLoc].d[1] == 6);
    }

    ASSERT(mTargetShape[0] > 0);
    ASSERT(mTargetShape[1] > 0);
    if (index == 0)
    {
        Dims outputShape{};
        outputShape.nbDims = inputs[st::kData].nbDims;
        for (int d = 0; d < inputs[st::kData].nbDims; ++d)
        {
            outputShape.type[d] = inputs[st::kData].type[d];
            outputShape.d[d] = inputs[st::kData].d[d];
        }
        outputShape.d[2] = mTargetShape[0];
        outputShape.d[3] = mTargetShape[1];

        return outputShape;
    }
    else if (index == 1)
    {
        Dims gridDstShape{};
        gridDstShape.nbDims = 2;
        gridDstShape.type[0] = DimensionType::kCHANNEL;
        gridDstShape.type[1] = DimensionType::kSPATIAL;
        gridDstShape.d[0] = 3;
        gridDstShape.d[1] = mTargetShape[0] * mTargetShape[1];

        return gridDstShape;
    }
    else
    {
        Dims gridSrcShape{};
        gridSrcShape.nbDims = 3;
        gridSrcShape.type[0] = DimensionType::kINDEX;
        gridSrcShape.type[1] = DimensionType::kCHANNEL;
        gridSrcShape.type[2] = DimensionType::kSPATIAL;
        gridSrcShape.d[0] = inputs[st::kData].d[0];
        gridSrcShape.d[1] = 2;
        gridSrcShape.d[2] = mTargetShape[0] * mTargetShape[1];

        return gridSrcShape;
    }
}

bool SpatialTransformerPlugin::supportsFormat(DataType type, PluginFormat format) const
{
    return type == DataType::kFLOAT && format == PluginFormat::kNCHW;
}

void SpatialTransformerPlugin::cudnnInitialize()
{
    mTensorFormat = CUDNN_TENSOR_NCHW;
    CUDNNASSERT(cudnnCreate(&mCudnn))
    // Create cudnn tensor descriptors
    CUDNNASSERT(cudnnCreateSpatialTransformerDescriptor(&mSTDesc))
    CUDNNASSERT(cudnnCreateTensorDescriptor(&mInDesc))
    CUDNNASSERT(cudnnCreateTensorDescriptor(&mOutDesc))
    mInitialized = true;
}

int SpatialTransformerPlugin::initialize()
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

void SpatialTransformerPlugin::terminate()
{
    if (!mInitialized)
    {
        return;
    }
    CUDNNASSERT(cudnnDestroyTensorDescriptor(mInDesc))
    CUDNNASSERT(cudnnDestroyTensorDescriptor(mOutDesc))
    CUDNNASSERT(cudnnDestroySpatialTransformerDescriptor(mSTDesc))
    CUDNNASSERT(cudnnDestroy(mCudnn))
    mInitialized = false;
}

int SpatialTransformerPlugin::enqueue(
    int batchSize, const void* const* inputs, void** outputs, void* workspace, cudaStream_t stream)
{
    /// mCudnn, mInDesc, mOutDesc, mSTDesc are shared in multi-thread, so must be has lock
    std::lock_guard<std::mutex> lk(mtx);
    if (!mInitialized)
    {
        initialize();
    }
    ASSERT(mInitialized);

    mDType = CUDNN_DATA_FLOAT;

    CUDNNASSERT(cudnnSetTensor4dDescriptor(mInDesc, mTensorFormat, mDType, mInputShape[st::kData].d[0] * batchSize,
        mInputShape[st::kData].d[1], mInputShape[st::kData].d[2], mInputShape[st::kData].d[3]));
    CUDNNASSERT(cudnnSetTensor4dDescriptor(mOutDesc, mTensorFormat, mDType, mOutputShape[st::kOut].d[0] * batchSize,
        mOutputShape[st::kOut].d[1], mOutputShape[st::kOut].d[2], mOutputShape[st::kOut].d[3]));
    if (mSamplerType == CUDNN_SAMPLER_BILINEAR)
    {
        int dim[] = {mOutputShape[st::kOut].d[0] * batchSize, mOutputShape[st::kOut].d[1], mOutputShape[st::kOut].d[2],
            mOutputShape[st::kOut].d[3]};
        CUDNNASSERT(cudnnSetSpatialTransformerNdDescriptor(mSTDesc, mSamplerType, mDType, 4, dim));
    }

    constexpr float alpha = 1.0f;
    constexpr float beta = 0.0f;


    CUDNNASSERT(cudnnSetStream(mCudnn, stream));
    if (mTransformType == st::kAffine)
    {
        CUDNNASSERT(cudnnSpatialTfGridGeneratorForward(mCudnn, mSTDesc,
            reinterpret_cast<const float*>(inputs[st::kLoc]), reinterpret_cast<float*>(outputs[st::kGridSrc])));
    }
    cudaStreamSynchronize(stream);
    CUDNNASSERT(cudnnSpatialTfSamplerForward(mCudnn, mSTDesc, &alpha, mInDesc,
        reinterpret_cast<const float*>(inputs[st::kData]), reinterpret_cast<const float*>(outputs[st::kGridSrc]), &beta,
        mOutDesc, reinterpret_cast<float*>(outputs[st::kOut])));
    cudaStreamSynchronize(stream);
    return 0;
}

size_t SpatialTransformerPlugin::getSerializationSize() const
{
    return (getBaseSerializationSize() + serialized_size(mSamplerType) + serialized_size(mTargetShape)
        + serialized_size(mTransformType));
}

void SpatialTransformerPlugin::serialize(void* buffer) const
{
    serializeBase(buffer);
    serialize_value(&buffer, mSamplerType);
    serialize_value(&buffer, mTargetShape);
    serialize_value(&buffer, mTransformType);
}

IPluginV2Ext* SpatialTransformerPlugin::clone() const
{
    return new SpatialTransformerPlugin(
        mLayerName, mInputShape, mOutputShape, mTargetShape, mTransformType, mSamplerType);
}

DataType SpatialTransformerPlugin::getOutputDataType(
    int index, const nvinfer1::DataType* inputTypes, int nbInputs) const
{
    // one outputs
    ASSERT(index < 3)
    return DataType::kFLOAT;
}

void SpatialTransformerPlugin::configurePlugin(const Dims* inputDims, int nbInputs, const Dims* outputDims,
    int nbOutputs, const DataType* inputTypes, const DataType* outputTypes, const bool* inputIsBroadcast,
    const bool* outputIsBroadcast, PluginFormat floatFormat, int maxBatchSize)
{
    ASSERT(nbInputs == 2)
    ASSERT(nbOutputs == 3)
    ASSERT(inputTypes[0] == DataType::kFLOAT)
    ASSERT(floatFormat == PluginFormat::kNCHW)

    mInputShape.assign(inputDims, inputDims + nbInputs);
}

SpatialTransformerPluginCreator::SpatialTransformerPluginCreator() {}

const char* SpatialTransformerPluginCreator::getPluginName() const
{
    return SPATIAL_TRANSFORMER_PLUGIN_NAME;
}

const char* SpatialTransformerPluginCreator::getPluginVersion() const
{
    return SPATIAL_TRANSFORMER_PLUGIN_VERSION;
}

const PluginFieldCollection* SpatialTransformerPluginCreator::getFieldNames()
{
    return &mFC;
}

IPluginV2Ext* SpatialTransformerPluginCreator::createPlugin(const char* name, const PluginFieldCollection* fc)
{
    std::vector<int> targetShape;
    int transferType = 0;
    int samplerType = 0;
    const PluginField* fields = fc->fields;

    // Parse fields from PluginFieldCollection
    ASSERT(fc->nbFields == 3);
    for (int i = 0; i < fc->nbFields; i++)
    {
        const char* attrName = fields[i].name;
        if (strcmp(attrName, "targetShape") == 0)
        {
            ASSERT(fields[i].type == PluginFieldType::kINT32);
            int pad_width_num = fields[i].length;
            targetShape.resize(static_cast<unsigned long>(pad_width_num));
            memcpy(targetShape.data(), fields[i].data, sizeof(int) * pad_width_num);
        }
        else if (strcmp(attrName, "transferType") == 0)
        {
            ASSERT(fields[i].type == PluginFieldType::kINT32);
            transferType = *(static_cast<const int*>(fields[i].data));
        }
        else if (strcmp(attrName, "samplerType") == 0)
        {
            ASSERT(fields[i].type == PluginFieldType::kINT32);
            samplerType = *(static_cast<const int*>(fields[i].data));
        }
    }

    ASSERT(transferType == st::kAffine);
    ASSERT(samplerType == st::kBilinear);
    cudnnSamplerType_t samplerType_cudnn = CUDNN_SAMPLER_BILINEAR;
    IPluginV2Ext* plugin = new SpatialTransformerPlugin(name, targetShape, transferType, samplerType_cudnn);
    plugin->setPluginNamespace(mNamespace.c_str());
    return plugin;
}

IPluginV2Ext* SpatialTransformerPluginCreator::deserializePlugin(
    const char* name, const void* serialData, size_t serialLength)
{
    auto* plugin = new SpatialTransformerPlugin(name, serialData, serialLength);
    plugin->setPluginNamespace(mNamespace.c_str());
    return plugin;
}

#include "BilinearSamplerPlugin.h"
#include <thread>
#include <mutex>

static std::mutex mtx;

using namespace arctern::trtPlugin;

namespace
{
static const char* BILINEAR_SAMPLER_PLUGIN_VERSION{"1"};
static const char* BILINEAR_SAMPLER_PLUGIN_NAME{"BilinearSampler_ARCTERN"};
} // namespace

// Static class fields initialization
PluginFieldCollection BilinearSamplerPluginCreator::mFC{};
std::vector<PluginField> BilinearSamplerPluginCreator::mPluginAttributes;

BilinearSamplerPlugin::BilinearSamplerPlugin(const std::string name)
    : BasePlugin(name)
    , mInitialized(false)
{
}

BilinearSamplerPlugin::BilinearSamplerPlugin(
    const std::string name, const std::vector<Dims>& inputShape, const std::vector<Dims>& outputShape)
    : BasePlugin(name, inputShape, outputShape)
    , mInitialized(false)
{
}

BilinearSamplerPlugin::BilinearSamplerPlugin(const std::string name, const void* data, size_t length)
    : BasePlugin(name)
    , mInitialized(false)
{
    deserializeBase(data, length);
}

const char* BilinearSamplerPlugin::getPluginType() const
{
    return BILINEAR_SAMPLER_PLUGIN_NAME;
}

const char* BilinearSamplerPlugin::getPluginVersion() const
{
    return BILINEAR_SAMPLER_PLUGIN_VERSION;
}

int BilinearSamplerPlugin::getNbOutputs() const
{
    return 1;
}

Dims BilinearSamplerPlugin::getOutputDimensions(int index, const Dims* inputs, int nbInputDims)
{
    ASSERT(index == 0)
    ASSERT(nbInputDims == 2)

    ASSERT(inputs[bs_enum::kData].nbDims == 4)
    ASSERT(inputs[bs_enum::kGrid].nbDims == 4)

    ASSERT(inputs[bs_enum::kData].d[0] == inputs[bs_enum::kGrid].d[0])
    ASSERT(inputs[bs_enum::kGrid].d[3] == 2)

    Dims outputShape{};
    outputShape.nbDims = inputs[bs_enum::kData].nbDims;
    for (int d = 0; d < inputs[bs_enum::kData].nbDims; ++d)
    {
        outputShape.type[d] = inputs[bs_enum::kData].type[d];
        outputShape.d[d] = inputs[bs_enum::kData].d[d];
    }
    outputShape.d[2] = inputs[bs_enum::kGrid].d[1];
    outputShape.d[3] = inputs[bs_enum::kGrid].d[2];

    return outputShape;
}

bool BilinearSamplerPlugin::supportsFormat(DataType type, PluginFormat format) const
{
    return type == DataType::kFLOAT && format == PluginFormat::kNCHW;
}

void BilinearSamplerPlugin::cudnnInitialize()
{
    mTensorFormat = CUDNN_TENSOR_NCHW;
    CUDNNASSERT(cudnnCreate(&mCudnn))
    // Create cudnn tensor descriptors
    CUDNNASSERT(cudnnCreateSpatialTransformerDescriptor(&mSTDesc))
    CUDNNASSERT(cudnnCreateTensorDescriptor(&mInDesc))
    CUDNNASSERT(cudnnCreateTensorDescriptor(&mOutDesc))

    mInitialized = true;
}

int BilinearSamplerPlugin::initialize()
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

void BilinearSamplerPlugin::terminate()
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

int BilinearSamplerPlugin::enqueue(
    int batchSize, const void* const* inputs, void** outputs, void* workspace, cudaStream_t stream)
{
    /// mCudnn, mInDesc, mOutDesc, mSTDesc are shared in multi-thread, so must be has lock
    std::lock_guard<std::mutex> lk(mtx);

    if (!mInitialized)
    {
        cudnnInitialize();
    }
    ASSERT(mInitialized)

    mDType = CUDNN_DATA_FLOAT;

    CUDNNASSERT(cudnnSetTensor4dDescriptor(mInDesc, mTensorFormat, mDType, mInputShape[bs_enum::kData].d[0] * batchSize,
        mInputShape[bs_enum::kData].d[1], mInputShape[bs_enum::kData].d[2], mInputShape[bs_enum::kData].d[3]))
    CUDNNASSERT(
        cudnnSetTensor4dDescriptor(mOutDesc, mTensorFormat, mDType, mOutputShape[bs_enum::kOut].d[0] * batchSize,
            mOutputShape[bs_enum::kOut].d[1], mOutputShape[bs_enum::kOut].d[2], mOutputShape[bs_enum::kOut].d[3]))
    int dim[] = {mOutputShape[bs_enum::kOut].d[0] * batchSize, mOutputShape[bs_enum::kOut].d[1],
        mOutputShape[bs_enum::kOut].d[2], mOutputShape[bs_enum::kOut].d[3]};
    CUDNNASSERT(cudnnSetSpatialTransformerNdDescriptor(mSTDesc, CUDNN_SAMPLER_BILINEAR, mDType, 4, dim))

    constexpr float alpha = 1.0f;
    constexpr float beta = 0.0f;
    CUDNNASSERT(cudnnSetStream(mCudnn, stream))
    CUDNNASSERT(cudnnSpatialTfSamplerForward(mCudnn, mSTDesc, &alpha, mInDesc,
        reinterpret_cast<const float*>(inputs[bs_enum::kData]), reinterpret_cast<const float*>(inputs[bs_enum::kGrid]),
        &beta, mOutDesc, reinterpret_cast<float*>(outputs[bs_enum::kOut])))
    cudaStreamSynchronize(stream);
    return 0;
}

size_t BilinearSamplerPlugin::getSerializationSize() const
{
    return (getBaseSerializationSize());
}

void BilinearSamplerPlugin::serialize(void* buffer) const
{
    serializeBase(buffer);
}

IPluginV2Ext* BilinearSamplerPlugin::clone() const
{
    return new BilinearSamplerPlugin(mLayerName, mInputShape, mOutputShape);
}

DataType BilinearSamplerPlugin::getOutputDataType(int index, const nvinfer1::DataType* inputTypes, int nbInputs) const
{
    // one outputs
    ASSERT(index == 0)
    return DataType::kFLOAT;
}

void BilinearSamplerPlugin::configurePlugin(const Dims* inputDims, int nbInputs, const Dims* outputDims, int nbOutputs,
    const DataType* inputTypes, const DataType* outputTypes, const bool* inputIsBroadcast,
    const bool* outputIsBroadcast, PluginFormat floatFormat, int maxBatchSize)
{
    ASSERT(nbInputs == 2)
    ASSERT(nbOutputs == 1)
    ASSERT(inputTypes[0] == DataType::kFLOAT)
    ASSERT(floatFormat == PluginFormat::kNCHW)

    mInputShape.assign(inputDims, inputDims + nbInputs);
}

BilinearSamplerPluginCreator::BilinearSamplerPluginCreator() {}

const char* BilinearSamplerPluginCreator::getPluginName() const
{
    return BILINEAR_SAMPLER_PLUGIN_NAME;
}

const char* BilinearSamplerPluginCreator::getPluginVersion() const
{
    return BILINEAR_SAMPLER_PLUGIN_VERSION;
}

const PluginFieldCollection* BilinearSamplerPluginCreator::getFieldNames()
{
    return &mFC;
}

IPluginV2Ext* BilinearSamplerPluginCreator::createPlugin(const char* name, const PluginFieldCollection* fc)
{
    IPluginV2Ext* plugin = new BilinearSamplerPlugin(name);
    plugin->setPluginNamespace(mNamespace.c_str());
    return plugin;
}

IPluginV2Ext* BilinearSamplerPluginCreator::deserializePlugin(
    const char* name, const void* serialData, size_t serialLength)
{
    BilinearSamplerPlugin* plugin = new BilinearSamplerPlugin(name, serialData, serialLength);
    plugin->setPluginNamespace(mNamespace.c_str());
    return plugin;
}

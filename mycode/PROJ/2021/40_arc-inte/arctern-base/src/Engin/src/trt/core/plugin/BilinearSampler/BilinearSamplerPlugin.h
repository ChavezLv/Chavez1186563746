#pragma once

#include <string>
#include <vector>

#include "cudnn.h"
#include "../common/plugin.h"

namespace bs_enum
{
enum BilinearSamplerOpInputs
{
    kData,
    kGrid
};
enum BilinearSamplerOpOutputs
{
    kOut,
    kTmp
};
} // namespace bs_enum

namespace arctern
{
namespace trtPlugin
{

class BilinearSamplerPlugin : public BasePlugin
{
public:
    BilinearSamplerPlugin(const std::string name);

    BilinearSamplerPlugin(
        const std::string name, const std::vector<Dims>& inputShape, const std::vector<Dims>& outputShape);

    BilinearSamplerPlugin(const std::string name, const void* data, size_t length);

    // It doesn't make sense to make BilinearSamplerPlugin without arguments, so we
    // delete default constructor.
    BilinearSamplerPlugin() = delete;

    const char* getPluginType() const override;

    const char* getPluginVersion() const override;

    int getNbOutputs() const override;

    Dims getOutputDimensions(int index, const Dims* inputs, int nbInputDims) override;

    bool supportsFormat(DataType type, PluginFormat format) const override;

    void cudnnInitialize();

    int initialize() override;

    void terminate() override;

    int enqueue(
        int batchSize, const void* const* inputs, void** outputs, void* workspace, cudaStream_t stream) override;

    size_t getSerializationSize() const override;

    void serialize(void* buffer) const override;

    IPluginV2Ext* clone() const override;

    DataType getOutputDataType(int index, const nvinfer1::DataType* inputTypes, int nbInputs) const override;

    void configurePlugin(const Dims* inputDims, int nbInputs, const Dims* outputDims, int nbOutputs,
        const DataType* inputTypes, const DataType* outputTypes, const bool* inputIsBroadcast,
        const bool* outputIsBroadcast, PluginFormat floatFormat, int maxBatchSize) override;

private:
    bool mInitialized{false};
    cudnnHandle_t mCudnn{nullptr};

    cudnnDataType_t mDType{CUDNN_DATA_FLOAT};
    cudnnSpatialTransformerDescriptor_t mSTDesc{};
    cudnnTensorDescriptor_t mInDesc{};
    cudnnTensorDescriptor_t mOutDesc{};
    cudnnTensorFormat_t mTensorFormat{CUDNN_TENSOR_NCHW};
};

class BilinearSamplerPluginCreator : public BaseCreator
{
public:
    BilinearSamplerPluginCreator();

    const char* getPluginName() const override;

    const char* getPluginVersion() const override;

    const PluginFieldCollection* getFieldNames() override;

    IPluginV2Ext* createPlugin(const char* name, const PluginFieldCollection* fc) override;

    IPluginV2Ext* deserializePlugin(const char* name, const void* serialData, size_t serialLength) override;

private:
    static PluginFieldCollection mFC;
    static std::vector<PluginField> mPluginAttributes;
};
} // namespace trtPlugin
} // namespace arctern

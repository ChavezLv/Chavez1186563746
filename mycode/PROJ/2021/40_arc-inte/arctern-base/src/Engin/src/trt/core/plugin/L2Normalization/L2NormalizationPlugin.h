#pragma once

#include "../common/plugin.h"
#include <string>
#include <vector>

using namespace arctern::trtPlugin;

namespace l2normalization_enum
{
enum L2NormalizationOpInputs
{
    kData
};
enum L2NormalizationOpOutputs
{
    kOut
};
} // namespace l2normalization_enum

namespace arctern
{
namespace trtPlugin
{

class L2NormalizationPlugin : public BasePlugin
{
public:
    L2NormalizationPlugin(const std::string name, float eps);

    L2NormalizationPlugin(
        const std::string name, const std::vector<Dims>& inputShape, const std::vector<Dims>& outputShape, float eps);

    L2NormalizationPlugin(const std::string name, const void* data, size_t length);

    // It doesn't make sense to make UpsamplePlugin without arguments, so we
    // delete default constructor.
    L2NormalizationPlugin() = delete;

    const char* getPluginType() const override;

    const char* getPluginVersion() const override;

    int getNbOutputs() const override;

    Dims getOutputDimensions(int index, const Dims* inputs, int nbInputDims) override;

    bool supportsFormat(DataType type, PluginFormat format) const override;

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
    float mEps;
};

class L2NormalizationPluginCreator : public BaseCreator
{
public:
    L2NormalizationPluginCreator();

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

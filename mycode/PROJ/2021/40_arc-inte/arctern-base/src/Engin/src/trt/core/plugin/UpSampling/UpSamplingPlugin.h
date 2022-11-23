#pragma once

#include "../common/plugin.h"
#include <string>
#include <vector>

using namespace arctern::trtPlugin;

namespace up_enum
{
enum UpSamplingOpInputs
{
    kData,
    kWeight
};
enum UpSamplingOpOutputs
{
    kOut
};
enum UpSamplingType
{
    kNearest,
    kBilinear
};
enum UpSamplingMultiInputMode
{
    kConcat,
    kSum
};
} // namespace up_enum

namespace arctern
{
namespace trtPlugin
{

class UpSamplingPlugin : public BasePlugin
{
public:
    UpSamplingPlugin(const std::string name, int scale, int num_filter, int sample_type, int multi_input_mode);

    UpSamplingPlugin(const std::string name, const std::vector<Dims>& inputShape,
        const std::vector<Dims>& outputShape, int scale, int num_filter, int sample_type, int multi_input_mode);

    UpSamplingPlugin(const std::string name, const void* data, size_t length);

    // It doesn't make sense to make UpsamplePlugin without arguments, so we
    // delete default constructor.
    UpSamplingPlugin() = delete;

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
    int mScale;
    int mNumFilter;
    int mSampleType;
    int mMultiInputMode;
};

class UpSamplingPluginCreator : public BaseCreator
{
public:
    UpSamplingPluginCreator();

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

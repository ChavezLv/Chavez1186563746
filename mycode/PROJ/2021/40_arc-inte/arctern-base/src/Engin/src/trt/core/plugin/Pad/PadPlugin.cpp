#include "PadKernel.h"
#include "PadPlugin.h"

namespace
{
static const char* PAD_PLUGIN_VERSION{"1"};
static const char* PAD_PLUGIN_NAME{"Pad_ARCTERN"};
} // namespace

// Static class fields initialization
PluginFieldCollection PadPluginCreator::mFC{};
std::vector<PluginField> PadPluginCreator::mPluginAttributes;

PadPlugin::PadPlugin(const std::string name, int mode, std::vector<int> pad_width, float constant_value)
    : BasePlugin(name)
    , mMode(mode)
    , mPadWidth(std::move(pad_width))
    , mConstantValue(constant_value)
{
}

PadPlugin::PadPlugin(const std::string name, const std::vector<Dims>& inputShape, const std::vector<Dims>& outputShape,
    int mode, std::vector<int> pad_width, float constant_value)
    : BasePlugin(name, inputShape, outputShape)
    , mMode(mode)
    , mPadWidth(std::move(pad_width))
    , mConstantValue(constant_value)
{
}

PadPlugin::PadPlugin(const std::string name, const void* data, size_t length)
    : BasePlugin(name)
{
    deserializeBase(data, length);
    deserialize_value(&data, &length, &mMode);
    deserialize_value(&data, &length, &mPadWidth);
    deserialize_value(&data, &length, &mConstantValue);
}

const char* PadPlugin::getPluginType() const
{
    return PAD_PLUGIN_NAME;
}

const char* PadPlugin::getPluginVersion() const
{
    return PAD_PLUGIN_VERSION;
}

int PadPlugin::getNbOutputs() const
{
    return 1;
}

Dims PadPlugin::getOutputDimensions(int index, const Dims* inputs, int nbInputDims)
{
    ASSERT(index == 0)
    ASSERT(nbInputDims == 1)

    const Dims dshape = inputs[pad_enum::kData];

    auto rank = dshape.nbDims;
    auto pad = mPadWidth;
    auto pad_spec_len = mPadWidth.size();

    // Current implementation only supports 4-D or 5-D input.
    ASSERT(rank == 4 || rank == 5)
    // Current implementation expects padding on the first two axes to be zero.
    ASSERT(pad[0] == 0 && pad[1] == 0 && pad[2] == 0 && pad[3] == 0)
    // Input shape vs padding spec mismatch.
    ASSERT((2 * rank) == pad_spec_len)

    if (mMode == pad_enum::kReflect)
    {
        auto size = dshape.d;
        // Current implementation of reflection padding only supports padding sizes
        // smaller than the input size.
        ASSERT((pad[4] < size[2]) && (pad[5] < size[2]) && (pad[6] < size[3]) && (pad[7] < size[3]));
    }

    Dims oshape = dshape;
    for (size_t i = 0; i < dshape.nbDims; ++i)
    {
        oshape.d[i] = mPadWidth[2 * i] + mPadWidth[2 * i + 1] + dshape.d[i];
    }

    return oshape;
}

bool PadPlugin::supportsFormat(DataType type, PluginFormat format) const
{
    return type == DataType::kFLOAT && format == PluginFormat::kNCHW;
}

int PadPlugin::initialize()
{
    int nbOutput = getNbOutputs();
    for (int i = 0; i < nbOutput; ++i)
    {
        mOutputShape.push_back(getOutputDimensions(i, &mInputShape[0], static_cast<int>(mInputShape.size())));
    }

    return 0;
}

void PadPlugin::terminate() {}

int PadPlugin::enqueue(int batchSize, const void* const* inputs, void** outputs, void* workspace, cudaStream_t stream)
{
    int ndim = mInputShape[pad_enum::kData].nbDims;
    auto status = PadInference(stream, outputs[0], inputs[0], batchSize, ndim, mInputShape[pad_enum::kData].d,
        mOutputShape[pad_enum::kOut].d, mPadWidth, mMode, mConstantValue);
    ASSERT(status == STATUS_SUCCESS);

    return 0;
}

size_t PadPlugin::getSerializationSize() const
{
    return (getBaseSerializationSize() + serialized_size(mMode) + serialized_size(mPadWidth)
        + serialized_size(mConstantValue));
}

void PadPlugin::serialize(void* buffer) const
{
    serializeBase(buffer);
    serialize_value(&buffer, mMode);
    serialize_value(&buffer, mPadWidth);
    serialize_value(&buffer, mConstantValue);
}

IPluginV2Ext* PadPlugin::clone() const
{
    return new PadPlugin(mLayerName, mInputShape, mOutputShape, mMode, mPadWidth, mConstantValue);
}

DataType PadPlugin::getOutputDataType(int index, const nvinfer1::DataType* inputTypes, int nbInputs) const
{
    // one outputs
    ASSERT(index == 0)
    return DataType::kFLOAT;
}

void PadPlugin::configurePlugin(const Dims* inputDims, int nbInputs, const Dims* outputDims, int nbOutputs,
    const DataType* inputTypes, const DataType* outputTypes, const bool* inputIsBroadcast,
    const bool* outputIsBroadcast, PluginFormat floatFormat, int maxBatchSize)
{
    ASSERT(nbInputs == 1);
    ASSERT(nbOutputs == 1);

    mInputShape.assign(inputDims, inputDims + nbInputs);
}

PadPluginCreator::PadPluginCreator()
{
    mPluginAttributes.emplace_back(PluginField("mode", nullptr, PluginFieldType::kINT32, 1));
    mPluginAttributes.emplace_back(PluginField("pad_width", nullptr, PluginFieldType::kINT32, 1));
    mPluginAttributes.emplace_back(PluginField("constant_value", nullptr, PluginFieldType::kFLOAT32, 1));

    mFC.nbFields = static_cast<int>(mPluginAttributes.size());
    mFC.fields = mPluginAttributes.data();
}

const char* PadPluginCreator::getPluginName() const
{
    return PAD_PLUGIN_NAME;
}

const char* PadPluginCreator::getPluginVersion() const
{
    return PAD_PLUGIN_VERSION;
}

const PluginFieldCollection* PadPluginCreator::getFieldNames()
{
    return &mFC;
}

IPluginV2Ext* PadPluginCreator::createPlugin(const char* name, const PluginFieldCollection* fc)
{
    int mode = 0;
    std::vector<int> pad_width;
    float constant_value = 0;
    const PluginField* fields = fc->fields;
    for (int i = 0; i < fc->nbFields; ++i)
    {
        const char* attrName = fields[i].name;
        if (strcmp(attrName, "mode") == 0)
        {
            ASSERT(fields[i].type == PluginFieldType::kINT32);
            mode = *(static_cast<const int*>(fields[i].data));
        }
        else if (strcmp(attrName, "pad_width") == 0)
        {
            ASSERT(fields[i].type == PluginFieldType::kINT32);
            int pad_width_num = fields[i].length;
            pad_width.resize(static_cast<unsigned long>(pad_width_num));
            memcpy(pad_width.data(), fields[i].data, sizeof(int) * pad_width_num);
        }
        else if (strcmp(attrName, "constant_value") == 0)
        {
            ASSERT(fields[i].type == PluginFieldType::kFLOAT32);
            constant_value = *(static_cast<const float*>(fields[i].data));
        }
    }

    IPluginV2Ext* plugin = new PadPlugin(name, mode, pad_width, constant_value);
    plugin->setPluginNamespace(mNamespace.c_str());
    return plugin;
}

IPluginV2Ext* PadPluginCreator::deserializePlugin(const char* name, const void* serialData, size_t serialLength)
{
    PadPlugin* plugin = new PadPlugin(name, serialData, serialLength);
    plugin->setPluginNamespace(mNamespace.c_str());
    return plugin;
}

#include "UpSamplingKernel.h"
#include "UpSamplingPlugin.h"

namespace
{
static const char* UPSAMPLING_PLUGIN_VERSION{"1"};
static const char* UPSAMPLING_PLUGIN_NAME{"UpSampling_ARCTERN"};
} // namespace

// Static class fields initialization
PluginFieldCollection UpSamplingPluginCreator::mFC{};
std::vector<PluginField> UpSamplingPluginCreator::mPluginAttributes;

UpSamplingPlugin::UpSamplingPlugin(
    const std::string name, int scale, int num_filter, int sample_type, int multi_input_mode)
    : BasePlugin(name)
    , mScale(scale)
    , mNumFilter(num_filter)
    , mSampleType(sample_type)
    , mMultiInputMode(multi_input_mode)
{
}

UpSamplingPlugin::UpSamplingPlugin(const std::string name, const std::vector<Dims>& inputShape,
    const std::vector<Dims>& outputShape, int scale, int num_filter, int sample_type, int multi_input_mode)
    : BasePlugin(name, inputShape, outputShape)
    , mScale(scale)
    , mNumFilter(num_filter)
    , mSampleType(sample_type)
    , mMultiInputMode(multi_input_mode)
{
}

UpSamplingPlugin::UpSamplingPlugin(const std::string name, const void* data, size_t length)
    : BasePlugin(name)
{
    deserializeBase(data, length);
    deserialize_value(&data, &length, &mScale);
    deserialize_value(&data, &length, &mNumFilter);
    deserialize_value(&data, &length, &mSampleType);
    deserialize_value(&data, &length, &mMultiInputMode);
}

const char* UpSamplingPlugin::getPluginType() const
{
    return UPSAMPLING_PLUGIN_NAME;
}

const char* UpSamplingPlugin::getPluginVersion() const
{
    return UPSAMPLING_PLUGIN_VERSION;
}

int UpSamplingPlugin::getNbOutputs() const
{
    return 1;
}

Dims UpSamplingPlugin::getOutputDimensions(int index, const Dims* inputs, int nbInputDims)
{
    ASSERT(index == 0);
    ASSERT(nbInputDims >= 1);

    Dims dshape = inputs[up_enum::kData];
    Dims oshape = dshape;
    if (mSampleType == up_enum::kNearest)
    {
        oshape.d[1] = 0;
        for (int i = 0; i < nbInputDims; i++)
        {
            // UpSamplingNearest: Input data should be 4D in (batch, channel, y, x)"
            ASSERT(inputs[i].nbDims == 4);
            int oh = dshape.d[2] * mScale;
            int ow = dshape.d[3] * mScale;

            // UpSamplingNearest: input height does not divide output height
            ASSERT(oh % inputs[i].d[2] == 0);
            // UpSamplingNearest: input width does not divide output width
            ASSERT(ow % inputs[i].d[3] == 0);
            if (mMultiInputMode == up_enum::kSum)
            {
                // Number of channels must be the same when multi_input_mode==sum
                ASSERT(oshape.d[1] == 0 || oshape.d[1] == inputs[i].d[1]);
                oshape.d[1] = inputs[i].d[1];
            }
            else
            {
                oshape.d[1] += inputs[i].d[1];
            }
        }
    }
    else
    {
        // Input:[data, weight]
        ASSERT(nbInputDims == 2);
        // UpSamplingBilinear: Input data should be 4D in (batch, channel, y, x)"
        ASSERT(dshape.nbDims == 4);

        ASSERT(dshape.nbDims > 0);
        int kernel = 2 * mScale - mScale % 2;

        // weight shape check
        ASSERT(inputs[up_enum::kWeight].nbDims == 4);
        ASSERT(inputs[up_enum::kWeight].d[0] == dshape.d[1]);
        ASSERT(inputs[up_enum::kWeight].d[1] == 1);
        ASSERT(inputs[up_enum::kWeight].d[2] == kernel);
        ASSERT(inputs[up_enum::kWeight].d[3] == kernel);
    }
    oshape.d[2] = dshape.d[2] * mScale;
    oshape.d[3] = dshape.d[3] * mScale;

    return oshape;
}

bool UpSamplingPlugin::supportsFormat(DataType type, PluginFormat format) const
{
    return type == DataType::kFLOAT && format == PluginFormat::kNCHW;
}

int UpSamplingPlugin::initialize()
{
    int nbOutput = getNbOutputs();
    for (int i = 0; i < nbOutput; ++i)
    {
        mOutputShape.push_back(getOutputDimensions(i, &mInputShape[0], static_cast<int>(mInputShape.size())));
    }

    return 0;
}

void UpSamplingPlugin::terminate() {}

int UpSamplingPlugin::enqueue(
    int batchSize, const void* const* inputs, void** outputs, void* workspace, cudaStream_t stream)
{
    pluginStatus_t status = UpSamplingInference(stream, outputs[up_enum::kOut], inputs[up_enum::kData], batchSize,
        mInputShape[up_enum::kData].nbDims, mInputShape[up_enum::kData].d, mOutputShape[up_enum::kOut].d, mScale);
    ASSERT(status == STATUS_SUCCESS);

    return 0;
}

size_t UpSamplingPlugin::getSerializationSize() const
{
    return (getBaseSerializationSize() + serialized_size(mScale) + serialized_size(mNumFilter)
        + serialized_size(mSampleType) + serialized_size(mMultiInputMode));
}

void UpSamplingPlugin::serialize(void* buffer) const
{
    serializeBase(buffer);
    serialize_value(&buffer, mScale);
    serialize_value(&buffer, mNumFilter);
    serialize_value(&buffer, mSampleType);
    serialize_value(&buffer, mMultiInputMode);
}

IPluginV2Ext* UpSamplingPlugin::clone() const
{
    return new UpSamplingPlugin(
        mLayerName, mInputShape, mOutputShape, mScale, mNumFilter, mSampleType, mMultiInputMode);
}

DataType UpSamplingPlugin::getOutputDataType(int index, const nvinfer1::DataType* inputTypes, int nbInputs) const
{
    // one outputs
    ASSERT(index == 0)
    return DataType::kFLOAT;
}

void UpSamplingPlugin::configurePlugin(const Dims* inputDims, int nbInputs, const Dims* outputDims, int nbOutputs,
    const DataType* inputTypes, const DataType* outputTypes, const bool* inputIsBroadcast,
    const bool* outputIsBroadcast, PluginFormat floatFormat, int maxBatchSize)
{
    ASSERT(nbInputs >= 1);
    ASSERT(nbOutputs == 1);

    mInputShape.assign(inputDims, inputDims + nbInputs);
}

UpSamplingPluginCreator::UpSamplingPluginCreator()
{
    mPluginAttributes.emplace_back(PluginField("scale", nullptr, PluginFieldType::kINT32, 1));
    mPluginAttributes.emplace_back(PluginField("num_filter", nullptr, PluginFieldType::kINT32, 1));
    mPluginAttributes.emplace_back(PluginField("sample_type", nullptr, PluginFieldType::kINT32, 1));
    mPluginAttributes.emplace_back(PluginField("multi_input_mode", nullptr, PluginFieldType::kINT32, 1));

    mFC.nbFields = static_cast<int>(mPluginAttributes.size());
    mFC.fields = mPluginAttributes.data();
}

const char* UpSamplingPluginCreator::getPluginName() const
{
    return UPSAMPLING_PLUGIN_NAME;
}

const char* UpSamplingPluginCreator::getPluginVersion() const
{
    return UPSAMPLING_PLUGIN_VERSION;
}

const PluginFieldCollection* UpSamplingPluginCreator::getFieldNames()
{
    return &mFC;
}

IPluginV2Ext* UpSamplingPluginCreator::createPlugin(const char* name, const PluginFieldCollection* fc)
{
    int scale = 0;
    int num_filter = 0;
    int sample_type = 0;
    int multi_input_mode = 0;
    const PluginField* fields = fc->fields;
    for (int i = 0; i < fc->nbFields; ++i)
    {
        const char* attrName = fields[i].name;
        if (strcmp(attrName, "scale") == 0)
        {
            ASSERT(fields[i].type == PluginFieldType::kINT32);
            scale = *(static_cast<const int*>(fields[i].data));
        }
        else if (strcmp(attrName, "num_filter") == 0)
        {
            ASSERT(fields[i].type == PluginFieldType::kINT32);
            num_filter = *(static_cast<const int*>(fields[i].data));
        }
        else if (strcmp(attrName, "sample_type") == 0)
        {
            ASSERT(fields[i].type == PluginFieldType::kINT32);
            sample_type = *(static_cast<const int*>(fields[i].data));
        }
        else if (strcmp(attrName, "multi_input_mode") == 0)
        {
            ASSERT(fields[i].type == PluginFieldType::kINT32);
            multi_input_mode = *(static_cast<const int*>(fields[i].data));
        }
    }

    ASSERT(sample_type == up_enum::kNearest);
    IPluginV2Ext* plugin = new UpSamplingPlugin(name, scale, num_filter, sample_type, multi_input_mode);
    plugin->setPluginNamespace(mNamespace.c_str());
    return plugin;
}

IPluginV2Ext* UpSamplingPluginCreator::deserializePlugin(const char* name, const void* serialData, size_t serialLength)
{
    UpSamplingPlugin* plugin = new UpSamplingPlugin(name, serialData, serialLength);
    plugin->setPluginNamespace(mNamespace.c_str());
    return plugin;
}

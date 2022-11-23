//
// Created by gpu on 19-12-2.
//

#ifndef ARCTERN_EQUALPLUGIN_H
#define ARCTERN_EQUALPLUGIN_H
#include "../common/plugin.h"
#include <string>
#include <vector>

namespace arctern{
  namespace trtPlugin{
    class EqualPlugin: public BasePlugin{
    public:
        EqualPlugin(const std::string name);
        EqualPlugin(const std::string name, const std::vector<Dims>& inputShape, const std::vector<Dims>& outputShape);
        EqualPlugin(const std::string name, const void* data, size_t length);

        EqualPlugin() = delete;

        const char* getPluginType() const override;

        const char* getPluginVersion() const override;

        int getNbOutputs() const override;

        Dims getOutputDimensions(int index, const Dims* inputs, int nbInputDims) override;

        bool supportsFormat(DataType type, PluginFormat format) const override;

        int initialize() override;

        void terminate() override;

        int enqueue(int batchSize, const void* const* inputs, void** outputs,
                    void* workspace, cudaStream_t stream) override;

        size_t getSerializationSize() const override;

        void serialize(void* buffer) const override;

        IPluginV2Ext* clone() const override;

        DataType getOutputDataType(int index, const nvinfer1::DataType* inputTypes, int nbInputs) const override;

        void configurePlugin(const Dims* inputDims, int nbInputs, const Dims* outputDims, int nbOutputs,
                             const DataType* inputTypes, const DataType* outputTypes, const bool* inputIsBroadcast,
                             const bool* outputIsBroadcast, PluginFormat floatFormat, int maxBatchSize) override;

    };

    class EqualPluginCreator: public BaseCreator{
    public:
        EqualPluginCreator();
        const char* getPluginName() const override;

        const char* getPluginVersion() const override;

        const PluginFieldCollection* getFieldNames() override;

        IPluginV2Ext* createPlugin(const char* name, const PluginFieldCollection* fc) override;

        IPluginV2Ext* deserializePlugin(const char* name, const void* serialData, size_t serialLength) override;

    private:
        static PluginFieldCollection mFC;
        static std::vector<PluginField> mPluginAttributes;
    };
  } /// namespace trtPlugin
}/// namespace arctern

#endif //ARCTERN_EQUALPLUGIN_H

#include "NvInfer.h"
#include <algorithm>
#include <array>
#include <memory>

#include "BilinearSampler/BilinearSamplerPlugin.h"
#include "Embedding/EmbeddingPlugin.h"
#include "GridGenerator/GridGeneratorPlugin.h"
#include "L2Normalization/L2NormalizationPlugin.h"
#include "LastDimTopK/LastDimTopKPlugin.h"
#include "PReLU/PReLUPlugin.h"
#include "Pad/PadPlugin.h"
#include "SpatialTransformer/SpatialTransformerPlugin.h"
#include "UpSampling/UpSamplingPlugin.h"
#include "Equal/EqualPlugin.h"
#include "ModSaclar/ModSaclar.h"
#include "Repeat/RepeatPlugin.h"
#include "Erf/ErfPlugin.h"
using namespace nvinfer1;

namespace arctern
{
namespace trtPlugin
{
nvinfer1::ILogger* gLogger{};

///\brief Instances of this class are statically constructed in initializePlugin.
///       This ensures that each plugin is only registered a single time, as further calls to
///       initializePlugin will be no-ops.
template <typename CreatorType>
class InitializePlugin
{
public:
    InitializePlugin(void* logger, const char* libNamespace)
        : mCreator{new CreatorType{}}
    {
        mCreator->setPluginNamespace(libNamespace);
        bool status = getPluginRegistry()->registerCreator(*mCreator, libNamespace);
        if (logger)
        {
            arctern::trtPlugin::gLogger = static_cast<nvinfer1::ILogger*>(logger);
            if (!status)
            {
                std::string errorMsg{"Could not register plugin creator:  " + std::string(mCreator->getPluginName())
                    + " in namespace: " + std::string{mCreator->getPluginNamespace()}};
                arctern::trtPlugin::gLogger->log(ILogger::Severity::kERROR, errorMsg.c_str());
            }
            else
            {
                std::string verboseMsg{
                    "Plugin Creator registration succeeded - " + std::string{mCreator->getPluginName()}};
                arctern::trtPlugin::gLogger->log(ILogger::Severity::kVERBOSE, verboseMsg.c_str());
            }
        }
    }

    InitializePlugin(const InitializePlugin&) = delete;
    InitializePlugin(InitializePlugin&&) = delete;

private:
    std::unique_ptr<CreatorType> mCreator;
};

template <typename CreatorType>
void initializePlugin(void* logger, const char* libNamespace)
{
    static InitializePlugin<CreatorType> plugin{logger, libNamespace};
}

bool initArcternTRTPlugins(void* logger, const char* libNamespace)
{
    initializePlugin<arctern::trtPlugin::BilinearSamplerPluginCreator>(logger, libNamespace);
    initializePlugin<arctern::trtPlugin::EmbeddingPluginCreator>(logger, libNamespace);
    initializePlugin<arctern::trtPlugin::GridGeneratorPluginCreator>(logger, libNamespace);
    initializePlugin<arctern::trtPlugin::L2NormalizationPluginCreator>(logger, libNamespace);
    initializePlugin<arctern::trtPlugin::LastDimTopKPluginCreator>(logger, libNamespace);
    initializePlugin<arctern::trtPlugin::PReLUPluginCreator>(logger, libNamespace);
    initializePlugin<arctern::trtPlugin::PadPluginCreator>(logger, libNamespace);
    initializePlugin<arctern::trtPlugin::SpatialTransformerPluginCreator>(logger, libNamespace);
    initializePlugin<arctern::trtPlugin::UpSamplingPluginCreator>(logger, libNamespace);
    initializePlugin<arctern::trtPlugin::EqualPluginCreator>(logger, libNamespace);
    initializePlugin<arctern::trtPlugin::ModSaclarPluginCreator>(logger, libNamespace);
    initializePlugin<arctern::trtPlugin::RepeatPluginCreator>(logger, libNamespace);
    initializePlugin<arctern::trtPlugin::ErfPluginCreator>(logger, libNamespace);
    return true;
}

nvinfer1::IPluginV2* createBilinearSamplerPlugin(const char* layerName)
{
    auto plugin = new BilinearSamplerPlugin(layerName);
    return plugin;
}

nvinfer1::IPluginV2* createEmbeddingPlugin(const char* layerName, int inputDim, int outputDim, bool sparseGrad)
{
    auto plugin = new EmbeddingPlugin(layerName, inputDim, outputDim, sparseGrad);
    return plugin;
}

nvinfer1::IPluginV2* createGridGeneratorPlugin(const char* layerName, std::vector<int>& targetShape, int transferType)
{
    auto plugin = new GridGeneratorPlugin(layerName, targetShape, transferType);
    return plugin;
}

nvinfer1::IPluginV2* createL2NormalizationPlugin(const char* layerName, float eps)
{
    auto plugin = new L2NormalizationPlugin(layerName, eps);
    return plugin;
}

nvinfer1::IPluginV2* createLastDimTopKPlugin(const char* layerName, bool isAscend, int returnType, int k)
{
    auto plugin = new LastDimTopKPlugin(layerName, isAscend, returnType, k);
    return plugin;
}

nvinfer1::IPluginV2* createPadPlugin(const char* layerName, int mode, std::vector<int>& padWidth, float constantValue)
{
    auto plugin = new PadPlugin(layerName, mode, padWidth, constantValue);
    return plugin;
}

nvinfer1::IPluginV2* createRepeatPlugin(const char* layerName, int repeats, int axis){
    auto plugin = new RepeatPlugin(layerName, repeats, axis);
    return plugin;
}

nvinfer1::IPluginV2* createPReLUPlugin(const char* layerName, std::vector<float>& alpha)
{
    auto plugin = new PReLUPlugin(layerName, alpha);
    return plugin;
}

nvinfer1::IPluginV2* createSpatialTransformerPlugin(
    const char* layerName, std::vector<int>& targetShape, int transferType, int samplerType)
{
    auto plugin = new SpatialTransformerPlugin(layerName, targetShape, transferType, samplerType);
    return plugin;
}

nvinfer1::IPluginV2* createUpSamplingPlugin(
    const char* layerName, int scale, int numFilter, int samplerType, int multiInputMode)
{
    auto plugin = new UpSamplingPlugin(layerName, scale, numFilter, samplerType, multiInputMode);
    return plugin;
}

nvinfer1::IPluginV2* createEqualPlugin(const char* layerName)
{
    auto plugin = new EqualPlugin(layerName);
    return plugin;
}

nvinfer1::IPluginV2* createModScalarPlugin(const char* layerName, int& scalar, int& mode){
    auto plugin = new ModSaclarPlugin(layerName, scalar, mode);
    return plugin;
}
nvinfer1::IPluginV2* createErfPlugin(const char* layerName)
{
  auto plugin = new ErfPlugin(layerName);
  return plugin;
}

} // namespace trtPlugin
} // namespace arctern
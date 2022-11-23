#pragma once

#include "NvInfer.h"

#include <vector>

namespace arctern
{
namespace trtPlugin
{
/**
 * \brief Initialize plugins
 *
 * \param logger tensorrt logger
 * \param libNamespace plugins namespace
 */
bool initArcternTRTPlugins(void* logger, const char* libNamespace);
/**
 * \brief Create a Bilinear Sampler Plugin object
 *
 * \param layerName layer name
 * \return
 */
nvinfer1::IPluginV2* createBilinearSamplerPlugin(const char* layerName);
/**
 * \brief Create a Embedding Plugin object
 *
 * \param layerName layer name
 * \param inputDim input dims
 * \param outputDim output dims
 * \param sparseGrad sparse_grad (not used)
 */
nvinfer1::IPluginV2* createEmbeddingPlugin(const char* layerName, int inputDim, int outputDim, bool sparseGrad);
/**
 * \brief Create a Grid Generator Plugin object
 *
 * \param layerName layer name
 * \param targetShape target shape
 * \param transferType transfer type
 */
nvinfer1::IPluginV2* createGridGeneratorPlugin(const char* layerName, std::vector<int>& targetShape, int transferType);
/**
 * \brief Create L2 normalization plugin object
 *
 * \param layerName layer name
 * \param eps eps
 */
nvinfer1::IPluginV2* createL2NormalizationPlugin(const char* layerName, float eps);
/**
 * \brief Create a Last Dim Top K Plugin object
 *
 * \param layerName layer name
 * \param isAscend ascend or deascend
 * \param returnType return type
 * \param k k value
 */
nvinfer1::IPluginV2* createLastDimTopKPlugin(const char* layerName, bool isAscend, int returnType, int k);
/**
 * \brief Create a Pad Plugin object
 *
 * \param layerName layer name
 * \param mode mode
 * \param padWidth padding width
 * \param constantValue constant value
 */
nvinfer1::IPluginV2* createPadPlugin(
    const char* layerName, int mode, std::vector<int>& padWidth, float constantValue);

/**
 * \brief Create a Repeat Plugin object
 *
 * \param layerName layer name
 * \param repeats the count of repeat
 * \param axis
 */
nvinfer1::IPluginV2* createRepeatPlugin(
        const char* layerName, int repeats, int axis);
/**
 * \brief Create PReLU Plugin object
 *
 * \param layerName layer name
 * \param alpha alpha value
 */
nvinfer1::IPluginV2* createPReLUPlugin(const char* layerName, std::vector<float>& alpha);
/**
 * \brief Create a Spatial Transformer Plugin object
 *
 * \param layerName layer name
 * \param targetShape target shape
 * \param transferType transfer type
 * \param samplerType sampler type
 */
nvinfer1::IPluginV2* createSpatialTransformerPlugin(
    const char* layerName, std::vector<int>& targetShape, int transferType, int samplerType);
/**
 * \brief Create a Up Sampling Plugin object
 *
 * \param layerName layer namee
 * \param scale scale
 * \param numFilter num filter
 * \param samplerType sample type
 * \param multiInputMode multi input mode
 */
nvinfer1::IPluginV2* createUpSamplingPlugin(
    const char* layerName, int scale, int numFilter, int samplerType, int multiInputMode);

/**
 * \brief Create a Equal Plugin object
 *
 * \param layerName layer name
 * \return
 */
nvinfer1::IPluginV2* createEqualPlugin(const char* layerName);

/**
 * \brief Create a Mod scalar Plugin object
 *
 * \param layerName layer name
 * \return
 */
nvinfer1::IPluginV2* createModScalarPlugin(const char* layerName, int& scalar, int& mode);

/**
 * \brief Create a Erf Plugin object
 *
 * \param layerName layer name
 * \return
 */
nvinfer1::IPluginV2* createErfPlugin(const char* layerName);

} // namespace trtPlugin
} // namespace arctern

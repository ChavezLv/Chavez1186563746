//
// Created by gpu on 19-2-28.
//

#include "trtInfer.h"
#include <NvInferPlugin.h>
#include "plugin/inferPlugin.h"
#include <fstream>
#include <sstream>
#include "NvOnnxConfig.h"
#include "NvOnnxParser.h"
#include <algorithm>
#include "include/arctern.h"

using namespace arctern::trtParser;
using namespace arctern::trtPlugin;
namespace arctern{

    /// static global variable, the level of log is setted kWARNING default
    //todo(zy): set verbose level for debugging
    static TRT_Logger gLogger;

    bool TrtInferEngine::initTRT(const void* data, std::size_t size)
    {
        /// Initialize and register all the existing TensorRT plugins to the Plugin Registry
        static bool initNvPlugin = initLibNvInferPlugins(&gLogger, "");
        /// Initialize the constom plugin to the Plugin Registry
        static bool initTrtConstomPlugin = initArcternTRTPlugins(&gLogger, "");

        assert(initNvPlugin);
        assert(initTrtConstomPlugin);

        ICudaEngine* engine_t{nullptr};

        if (data)
        {
            /// Try to deserialize engine.
            //cudaSetDevice(gpu_id_); /// set device id
            unique_ptr<IRuntime, Destroy<IRuntime>> runtime{createInferRuntime(gLogger)};
            engine_t = runtime->deserializeCudaEngine(data, size, nullptr);
        }

        if (engine_t)
        {
            engine_ = engine_t;
            return true;
        }

        return false; /// engine init failed
    }

    bool TrtInferEngine::initOnnx(const void *data,const unsigned int& dataSize) {
      assert(data != nullptr);
      assert(dataSize > 0);
      auto builder =  unique_ptr<nvinfer1::IBuilder,Destroy<IBuilder>>(nvinfer1::createInferBuilder(gLogger));
      assert(builder);
      const auto networkFlag = 1U << static_cast<int>(NetworkDefinitionCreationFlag::kEXPLICIT_BATCH);
      auto network = unique_ptr<nvinfer1::INetworkDefinition,
      Destroy<INetworkDefinition>>(builder->createNetworkV2(networkFlag));

      assert(network);
      unique_ptr<nvonnxparser::IParser, Destroy<nvonnxparser::IParser>> parser(nvonnxparser::createParser(*network,gLogger));
      //auto ok = parser->parseFromFile(onnxModelFilePath.c_str(),3);
      auto ok = parser->parse(data,dataSize);
      assert(ok);

      auto profile = builder->createOptimizationProfile();
      auto inputLayer = network->getInput(0);
      auto inputName = inputLayer->getName();
      auto bDynamic = inputLayer->isShapeTensor();
      auto inputDims = inputLayer->getDimensions();
      bDynamic = bDynamic || std::any_of(inputDims.d,inputDims.d+inputDims.nbDims,[](int dim){ return dim==-1;});
      assert(bDynamic);
      inputDims.d[0] = 1;
      profile->setDimensions(inputName,OptProfileSelector::kMIN,inputDims);
      inputDims.d[0] = std::max(max_batchSize_ / 2,1);
      profile->setDimensions(inputName,OptProfileSelector::kOPT,inputDims);
      inputDims.d[0] = std::max(max_batchSize_,2);
      profile->setDimensions(inputName,OptProfileSelector::kMAX,inputDims);
      assert(profile->isValid());

      auto config = unique_ptr<nvinfer1::IBuilderConfig,Destroy<IBuilderConfig>>(builder->createBuilderConfig());
      config->addOptimizationProfile(profile);
      //builder->setMaxBatchSize(max_batchSize_);


      config->setMaxWorkspaceSize(MAX_WORKSPACE_SIZE);
      config->setFlag(BuilderFlag::kSTRICT_TYPES);

      config->setFlag(BuilderFlag::kDEBUG);
      config->setDefaultDeviceType(DeviceType::kGPU);
      config->setFlag(BuilderFlag::kGPU_FALLBACK);
      auto hasFp16 = builder->platformHasFastFp16();
      if(hasFp16 && model_dtype_ == ArcternTypeBits::ARCTERN_FP16){
        config->setFlag(BuilderFlag::kFP16);
      }
      if(engine_!= nullptr){
        engine_->destroy();
        engine_ = nullptr;
      }
      engine_ = builder->buildEngineWithConfig(*network,*config);
      if(engine_ == nullptr) return false;
      return true;


      /*auto builder =  unique_ptr<nvinfer1::IBuilder,Destroy<IBuilder>>(nvinfer1::createInferBuilder(gLogger));
      assert(builder);
      auto network = unique_ptr<nvinfer1::INetworkDefinition,Destroy<INetworkDefinition>>(builder->createNetwork());
      assert(network);
      unique_ptr<nvonnxparser::IParser, Destroy<nvonnxparser::IParser>> parser(nvonnxparser::createParser(*network,gLogger));
      auto ok = parser->parseFromFile(onnxModelFilePath.c_str(),static_cast<int>(ILogger::Severity::kERROR));
      assert(ok);
      builder->setMaxBatchSize(max_batchSize_);
      builder->setMaxWorkspaceSize(MAX_WORKSPACE_SIZE);

      builder->setStrictTypeConstraints(true);
      builder->setDebugSync(false);

      if(engine_!= nullptr){
        engine_->destroy();
        engine_ = nullptr;
      }
      engine_ = builder->buildCudaEngine(*network);
      if(engine_ == nullptr) return false;
      return true;*/
    }

    bool TrtInferEngine::initTRT(const char* mxnetSymbol, const void* mxnetParam, nvinfer1::Dims& inputDim,
                                 std::string dynamicRangeCache, std::string layerPrecision){
        //ARCTERN_ASSERT_MSG(mxnetSymbol != nullptr, "mxnetSymbol is NULL error!");
       // ARCTERN_ASSERT_MSG(mxnetParam != nullptr, "mxnetParam is NULL error!");

        /// create the builder of trt engine
        unique_ptr<IBuilder, Destroy<IBuilder>> trtBuilder{createInferBuilder(gLogger)};
        /// create the network of builder
        unique_ptr<INetworkDefinition, Destroy<INetworkDefinition>> trtNetwork{trtBuilder->createNetwork()};
        /// create the trt parser for parsering mxnet files
        unique_ptr<Parser, Destroy<Parser>> trtParser{createParser(*trtNetwork, gLogger)};

        bool parserOk = trtParser->parseFromFile(mxnetSymbol, mxnetParam, inputDim);
      //  ARCTERN_ASSERT_MSG(parserOk, "Parser mxnet to trt error!");

        bool hasFP16 = trtBuilder->platformHasFastFp16();
        bool hasINT8 = trtBuilder->platformHasFastInt8();

        /// configure builer
        trtBuilder->setMaxBatchSize(max_batchSize_);
        trtBuilder->setMaxWorkspaceSize(MAX_WORKSPACE_SIZE); ///the size is 1GB default
        if(hasINT8 && model_dtype_ == ArcternTypeBits::ARCTERN_INT8 && !dynamicRangeCache.empty()){
            /// Enable INT8 model. Required to set custom per tensor dynamic range or INT8 Calibration
            trtBuilder->setInt8Mode(true);
            /// Mark calibrator as null. As user provides dynamic range for each tensor, no calibrator is required
            trtBuilder->setInt8Calibrator(nullptr);
            /// set INT8 Per Tensor Dynamic range
            bool setOk = setDynamicRange(dynamicRangeCache, *trtNetwork);

            if (!layerPrecision.empty())
            {
                setLayerPrecision(layerPrecision, *trtNetwork);
            }
          //  ARCTERN_ASSERT_MSG(setOk, "Unable to set per tensor dynamic range.");
        }else if(hasFP16 && model_dtype_ == ArcternTypeBits::ARCTERN_FP16){
            trtBuilder->setFp16Mode(true);
        }

        trtBuilder->setStrictTypeConstraints(true);
        trtBuilder->setDebugSync(false);
        /// generate trt engine by builder
        ICudaEngine* engine_t{nullptr};
        engine_t = trtBuilder->buildCudaEngine(*trtNetwork);

        //ARCTERN_ASSERT_MSG(engine_t != nullptr, "Build cuda engine error!");
        engine_ = engine_t;

        return true;
    }

    void TrtInferEngine::destoryTRT()
    {
        engine_->destroy();
    }

    ///********************************************   INT8 REQUIRED   ************************************///
    void TrtInferEngine::writeNetworkTensorNames(nvinfer1::INetworkDefinition& network){
        static int index = 0;
        std::string saveName = "./tensorName-" + std::to_string(index++) + ".txt";
        std::ofstream tensorsFile(saveName, std::ofstream::out);

        /// Iterate through network inputs to write names of input tensors.
        for (int i = 0; i < network.getNbInputs(); ++i)
        {
            string tName = network.getInput(i)->getName();
            tensorsFile << tName << std::endl;
        }

        /// Iterate through network layers.
        for (int i = 0; i < network.getNbLayers(); ++i)
        {
            /// Write output tensors of a layer to the file.
            for (int j = 0; j < network.getLayer(i)->getNbOutputs(); ++j)
            {
                string tName = network.getLayer(i)->getOutput(j)->getName();
                tensorsFile << tName << std::endl;
            }
        }
        tensorsFile.close();
    }

    /// Populate per tensor dyanamic range values
    void TrtInferEngine::readPerTensorDynamicRangeValues(std::string& dynamicRangeCache){
        std::istringstream iDynamicRangeStream(dynamicRangeCache);

        std::string line;
        char delim = ':';
        while (std::getline(iDynamicRangeStream, line))
        {
            std::istringstream iline(line);
            std::string token;
            std::getline(iline, token, delim);
            std::string tensorName = token;
            std::getline(iline, token, delim);
            float dynamicRange = convertHexStrToFloat(token) * 127;
            mPerTensorDynamicRangeMap[tensorName] = dynamicRange;
        }
    }

    bool TrtInferEngine::setDynamicRange(std::string& dynamicRangeCache, nvinfer1::INetworkDefinition& network){
        /// read tensor dynamic range values
        readPerTensorDynamicRangeValues(dynamicRangeCache);

        /// set dynamic range for network input tensors
        for (int i = 0; i < network.getNbInputs(); ++i)
        {
            string tName = network.getInput(i)->getName();
            if (mPerTensorDynamicRangeMap.find(tName) != mPerTensorDynamicRangeMap.end())
            {
                network.getInput(i)->setDynamicRange(-mPerTensorDynamicRangeMap.at(tName),
                                                     mPerTensorDynamicRangeMap.at(tName));
            }
            else
            {
                /// "Missing dynamic range for tensor: " << tName
                return false;
            }
        }

        /// set dynamic range for layer output tensors
        for (int i = 0; i < network.getNbLayers(); ++i)
        {
            for (int j = 0; j < network.getLayer(i)->getNbOutputs(); ++j)
            {
                string tName = network.getLayer(i)->getOutput(j)->getName();
                if (mPerTensorDynamicRangeMap.find(tName) != mPerTensorDynamicRangeMap.end())
                {
                    /// Calibrator generated dynamic range for network tensor can be overriden or set using below API
                    network.getLayer(i)->getOutput(j)->setDynamicRange(-mPerTensorDynamicRangeMap.at(tName),
                                                                       mPerTensorDynamicRangeMap.at(tName));
                }
                else
                {
                    /// "Missing dynamic range for tensor: " << tName
                    return false;
                }
            }
        }

        return true;
    }

    void TrtInferEngine::readLayerPrecision(std::string& layerPrecision){
        std::istringstream iLayerPrecisionStream(layerPrecision);

        std::string line;
        char delim = ':';
        while (std::getline(iLayerPrecisionStream, line))
        {
            std::istringstream iline(line);
            std::string token;
            std::getline(iline, token, delim);
            int layerNo = std::atoi(token.c_str());
            std::getline(iline, token, delim);
            nvinfer1::DataType precision = nvinfer1::DataType::kFLOAT;
            if (token == "float")
            {
                precision = nvinfer1::DataType::kFLOAT;
            }
            else if (token == "int8")
            {
                precision = nvinfer1::DataType::kINT8;
            }
            mLayerPrecisionMap[layerNo] = precision;
        }
    }

    bool TrtInferEngine::setLayerPrecision(std::string& layerPrecision, nvinfer1::INetworkDefinition& network) {
        /// read layer precision
        readLayerPrecision(layerPrecision);
        /// set dynamic range for layer output tensors
        for (int i = 0; i < network.getNbLayers(); ++i)
        {
            string layerName = network.getLayer(i)->getName();
            if (mLayerPrecisionMap.find(i) != mLayerPrecisionMap.end())
            {
                //std::cout << "set layer [" << network.getLayer(i)->getOutput(0)->getName() << "] to float" << std::endl;
                network.getLayer(i)->setPrecision(mLayerPrecisionMap[i]);
            }
        }

        return true;
    }

    float TrtInferEngine::convertHexStrToFloat(std::string &str){
        uint32_t num;
        float f;
        sscanf(str.c_str(), "%x", &num);
        f = *((float*)&num);
        return f;
    }

} // end namespace



//
// Created by gpu on 19-2-28.
//

#ifndef ARCTERN_TRTINFER_H
#define ARCTERN_TRTINFER_H

#include "parsers/parser.h"
//#include "../../utils.h"
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <cassert>
#include <unordered_map>
#include "cuda_runtime.h"
#include "include/arctern.h"
#define MAX_WORKSPACE_SIZE 1 << 30

using namespace nvinfer1;
using namespace std;

namespace arctern{

    ///\brief tensorRT backend infer engine
    class TrtInferEngine{
    public:
        explicit TrtInferEngine(int gpu_id = -1, ArcternTypeBits model_dtype = ArcternTypeBits::ARCTERN_FP32, int max_BatchSize = 1){
            assert(gpu_id > -1);
            gpu_id_ = gpu_id;
            model_dtype_ = model_dtype;
            max_batchSize_ = max_BatchSize;
        }
        ~TrtInferEngine() = default;
        /// init engine by deserialize and set up the context of trt
        bool initTRT(const void* data, std::size_t size);

        bool initOnnx(const void *data,const unsigned int& dataSize);

        /// \brief init engine by trt Build system
        /// \param mxnetSymbol : mxnet symbol for mxnet model
        /// \param mxnetParam : mxnet param for mxnet model
        /// \param inputDim : the input dim for mxnet input layer
        /// \param dynamicRangeCache : it's used in INT8 interface
        /// \return true or false
        bool initTRT(const char* mxnetSymbol, const void* mxnetParam, nvinfer1::Dims& inputDim,
                     std::string dynamicRangeCache = std::string(), std::string layerPrecision = std::string());

        /// destory resources of trt
        void destoryTRT();

        int gpu_id_;
        ///\brief: the type of weights and tensors
        ArcternTypeBits model_dtype_;
        int max_batchSize_;
        ICudaEngine *engine_ = nullptr;

    private:
        ///********************************************   INT8 REQUIRED   ************************************///
        /// write per tensor name of network
        void writeNetworkTensorNames(nvinfer1::INetworkDefinition& network);

        /// Populate per tensor dyanamic range values
        void readPerTensorDynamicRangeValues(std::string& dynamicRangeCache);

        /// Sets custom dynamic range for network tensors
        bool setDynamicRange(std::string& dynamicRangeCache, nvinfer1::INetworkDefinition& network);

        /// read layer precision
        void readLayerPrecision(std::string& layerPrecision);

        /// set custon layer precision
        bool setLayerPrecision(std::string& layerPrecision, nvinfer1::INetworkDefinition& network);

        /// convert from hex string to float
        float convertHexStrToFloat(std::string &str);

        std::unordered_map<std::string, float> mPerTensorDynamicRangeMap; //!< Mapping from tensor name to max absolute dynamic range values
        std::unordered_map<int, nvinfer1::DataType> mLayerPrecisionMap; //!< Mapping from tensor name to max absolute dynamic range values
    };
}

#endif //ARCTERN_TRTINFER_H

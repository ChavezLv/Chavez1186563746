#pragma once

#include "NvInferPlugin.h"
#include "serialize.h"
#include <cuda_runtime.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#ifndef DEBUG

#define ASSERT(assertion)                                                                                              \
    {                                                                                                                  \
        if (!(assertion))                                                                                              \
        {                                                                                                              \
            std::cerr << "#assertion" << __FILE__ << "," << __LINE__ << std::endl;                                     \
            abort();                                                                                                   \
        }                                                                                                              \
    }

#define CUASSERT(status_)                                                                                              \
    {                                                                                                                  \
        auto s_ = status_;                                                                                             \
        if (s_ != cudaSuccess)                                                                                         \
        {                                                                                                              \
            std::cerr << __FILE__ << ", " << __LINE__ << ", " << s_ << ", " << cudaGetErrorString(s_) << std::endl;    \
        }                                                                                                              \
    }
#define CUBLASASSERT(status_)                                                                                          \
    {                                                                                                                  \
        auto s_ = status_;                                                                                             \
        if (s_ != CUBLAS_STATUS_SUCCESS)                                                                               \
        {                                                                                                              \
            std::cerr << __FILE__ << ", " << __LINE__ << ", " << s_ << std::endl;                                      \
        }                                                                                                              \
    }
#define CUDNNASSERT(status_)                                                                                           \
    {                                                                                                                  \
        auto s_ = status_;                                                                                             \
        if (s_ != CUDNN_STATUS_SUCCESS)                                                                                \
        {                                                                                                              \
            std::cerr << __FILE__ << ", " << __LINE__ << ", " << s_ << std::endl;                                      \
        }                                                                                                              \
    }
#define CUERRORMSG(status_)                                                                                            \
    {                                                                                                                  \
        auto s_ = status_;                                                                                             \
        if (s_ != 0)                                                                                                   \
            std::cerr << __FILE__ << ", " << __LINE__ << ", " << s_ << std::endl;                                      \
    }
#define CHECK(status)                                                                                                  \
    do                                                                                                                 \
    {                                                                                                                  \
        if (status != 0)                                                                                               \
            abort();                                                                                                   \
    } while (0)

#define ASSERT_PARAM(exp)                                                                                              \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(exp))                                                                                                    \
            return STATUS_BAD_PARAM;                                                                                   \
    } while (0)

#define ASSERT_FAILURE(exp)                                                                                            \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(exp))                                                                                                    \
            return STATUS_FAILURE;                                                                                     \
    } while (0)

#define CSC(call, err)                                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        cudaError_t cudaStatus = call;                                                                                 \
        if (cudaStatus != cudaSuccess)                                                                                 \
        {                                                                                                              \
            return err;                                                                                                \
        }                                                                                                              \
    } while (0)

#define DEBUG_PRINTF(...)                                                                                              \
    do                                                                                                                 \
    {                                                                                                                  \
    } while (0)

#else
#define ASSERT(assertion)                                                                                              \
    {                                                                                                                  \
        if (!(assertion))                                                                                              \
        {                                                                                                              \
            std::cerr << "#assertion" << __FILE__ << "," << __LINE__ << std::endl;                                     \
            abort();                                                                                                   \
        }                                                                                                              \
    }
#define CUASSERT(status_)                                                                                              \
    {                                                                                                                  \
        auto s_ = status_;                                                                                             \
        if (s_ != cudaSuccess)                                                                                         \
        {                                                                                                              \
            std::cerr << __FILE__ << ", " << __LINE__ << ", " << s_ << ", " << cudaGetErrorString(s_) << std::endl;    \
        }                                                                                                              \
    }
#define CUBLASASSERT(status_)                                                                                          \
    {                                                                                                                  \
        auto s_ = status_;                                                                                             \
        if (s_ != CUBLAS_STATUS_SUCCESS)                                                                               \
        {                                                                                                              \
            std::cerr << __FILE__ << ", " << __LINE__ << ", " << s_ << std::endl;                                      \
        }                                                                                                              \
    }
#define CUDNNASSERT(status_)                                                                                           \
    {                                                                                                                  \
        auto s_ = status_;                                                                                             \
        if (s_ != CUDNN_STATUS_SUCCESS)                                                                                \
        {                                                                                                              \
            std::cerr << __FILE__ << ", " << __LINE__ << ", " << s_ << std::endl;                                      \
        }                                                                                                              \
    }
#define CUERRORMSG(status_)                                                                                            \
    {                                                                                                                  \
        auto s_ = status_;                                                                                             \
        if (s_ != 0)                                                                                                   \
            std::cerr << __FILE__ << ", " << __LINE__ << ", " << s_ << std::endl;                                      \
    }
#define ASSERT_PARAM(exp)                                                                                              \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(exp))                                                                                                    \
        {                                                                                                              \
            fprintf(stderr, "Bad param - " #exp ", %s:%d\n", __FILE__, __LINE__);                                      \
            return STATUS_BAD_PARAM;                                                                                   \
        }                                                                                                              \
    } while (0)

#define ASSERT_FAILURE(exp)                                                                                            \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(exp))                                                                                                    \
        {                                                                                                              \
            fprintf(stderr, "Failure - " #exp ", %s:%d\n", __FILE__, __LINE__);                                        \
            return STATUS_FAILURE;                                                                                     \
        }                                                                                                              \
    } while (0)

#define CSC(call, err)                                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        cudaError_t cudaStatus = call;                                                                                 \
        if (cudaStatus != cudaSuccess)                                                                                 \
        {                                                                                                              \
            printf("%s %d CUDA FAIL %s\n", __FILE__, __LINE__, cudaGetErrorString(cudaStatus));                        \
            return err;                                                                                                \
        }                                                                                                              \
    } while (0)

#define CHECK(status)                                                                                                  \
    {                                                                                                                  \
        if (status != 0)                                                                                               \
        {                                                                                                              \
            DEBUG_PRINTF("%s %d CUDA FAIL %s\n", __FILE__, __LINE__, cudaGetErrorString(status));                      \
            abort();                                                                                                   \
        }                                                                                                              \
    }

#define DEBUG_PRINTF(...)                                                                                              \
    do                                                                                                                 \
    {                                                                                                                  \
        printf(__VA_ARGS__);                                                                                           \
    } while (0)

#endif

using namespace nvinfer1;
using namespace nvinfer1::plugin;

// Enumerator for status
typedef enum
{
    STATUS_SUCCESS = 0,
    STATUS_FAILURE = 1,
    STATUS_BAD_PARAM = 2,
    STATUS_NOT_SUPPORTED = 3,
    STATUS_NOT_INITIALIZED = 4
} pluginStatus_t;

namespace arctern
{
namespace trtPlugin
{

// Write values into buffer
template <typename T>
void write(char*& buffer, const T& val)
{
    *reinterpret_cast<T*>(buffer) = val;
    buffer += sizeof(T);
}

// Read values from buffer
template <typename T>
T read(const char*& buffer)
{
    T val = *reinterpret_cast<const T*>(buffer);
    buffer += sizeof(T);
    return val;
}

template <typename T>
Weights copyToDevice(const void* hostData, size_t count)
{
    void* deviceData;
    CUASSERT(cudaMalloc(&deviceData, count * sizeof(T)))
    CUASSERT(cudaMemcpy(deviceData, hostData, count * sizeof(T), cudaMemcpyHostToDevice))
    return Weights{DataType::kFLOAT, deviceData, int64_t(count)};
}

template <typename T>
void serializeFromDevice(char*& hostBuffer, Weights deviceWeights)
{
    CUASSERT(cudaMemcpy(hostBuffer, deviceWeights.values, deviceWeights.count * sizeof(T), cudaMemcpyDeviceToHost))
    hostBuffer += deviceWeights.count * sizeof(T);
}

template <typename T>
Weights deserializeToDevice(const void*& hostBuffer, size_t& serialLength)
{
    size_t count = 0;
    deserialize_value<T>(&hostBuffer, &serialLength, &count);
    Weights w = copyToDevice<T>(hostBuffer, count);
    serialLength -= count * sizeof(T);
    return w;
}

class BasePlugin : public IPluginV2Ext
{
public:
    explicit BasePlugin(const std::string& name)
    {
        mLayerName = name;
    }

    BasePlugin(const std::string& name, const std::vector<Dims>& inputShape, const std::vector<Dims>& outputShape)
    {
        mLayerName = name;
        mInputShape = inputShape;
        mOutputShape = outputShape;
    }

    int initialize() override
    {
        return STATUS_SUCCESS;
    }

    void terminate() override {}

    size_t getWorkspaceSize(int) const override
    {
        return 0;
    }

    void destroy() override
    {
        delete this;
    }

    void setPluginNamespace(const char* libNamespace) override
    {
        mNamespace = libNamespace;
    }

    const char* getPluginNamespace() const override
    {
        return mNamespace.c_str();
    }

    bool isOutputBroadcastAcrossBatch(int outputIndex, const bool* inputIsBroadcasted, int nbInputs) const override
    {
        return false;
    }

    bool canBroadcastInputAcrossBatch(int inputIndex) const override
    {
        return false;
    };

    void attachToContext(cudnnContext* cudnn, cublasContext* cublas, IGpuAllocator* allocator) override {}

    void detachFromContext() override {}

    Dims const& getInputDims(int index) const
    {
        return mInputShape.at(index);
    }

protected:
    void deserializeBase(void const*& serialData, size_t& serialLength)
    {
        deserialize_value(&serialData, &serialLength, &mInputShape);
    }

    size_t getBaseSerializationSize() const
    {
        return (serialized_size(mInputShape));
    }

    void serializeBase(void*& buffer) const
    {
        serialize_value(&buffer, mInputShape);
    }

    std::string mLayerName;
    std::string mNamespace;

    std::vector<Dims> mInputShape;
    std::vector<Dims> mOutputShape;
};

class BaseCreator : public IPluginCreator
{
public:
    void setPluginNamespace(const char* libNamespace) override
    {
        mNamespace = libNamespace;
    }

    const char* getPluginNamespace() const override
    {
        return mNamespace.c_str();
    }

    std::string mNamespace;
};

} // namespace trtPlugin
} // namespace arctern

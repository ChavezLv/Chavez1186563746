#pragma once

#include <random>
#include <utility>
#include <fstream>
#include <iterator>

//#include "batchStream.hpp"
#include "NvInfer.h"
#include "cuda_runtime.h"

#include "common.hpp"
#include "trtUtils.hpp"

namespace arctern
{
namespace trtParser
{
//! \class EntropyCalibratorImpl
//!
//! \brief Implements common functionality for Entropy calibrators.
//!
// class EntropyCalibratorImpl
//{
// public:
//    EntropyCalibratorImpl(
//        BatchStream& stream, int firstBatch, std::string networkName, const char* inputBlobName, bool readCache =
//        true) : mStream(stream) , mCalibrationTableName("CalibrationTable" + networkName) ,
//        mInputBlobName(inputBlobName) , mReadCache(readCache)
//    {
//        nvinfer1::Dims dims = mStream.getDims();
//        mInputCount = samplesCommon::volume(dims);
//        CHECK(cudaMalloc(&mDeviceInput, mInputCount * sizeof(float)));
//        mStream.reset(firstBatch);
//    }
//
//    virtual ~EntropyCalibratorImpl()
//    {
//        CHECK(cudaFree(mDeviceInput));
//    }
//
//    int getBatchSize() const
//    {
//        return mStream.getBatchSize();
//    }
//
//    bool getBatch(void* bindings[], const char* names[], int nbBindings)
//    {
//        if (!mStream.next())
//        {
//            return false;
//        }
//        CHECK(cudaMemcpy(mDeviceInput, mStream.getBatch(), mInputCount * sizeof(float), cudaMemcpyHostToDevice));
//        assert(!strcmp(names[0], mInputBlobName));
//        bindings[0] = mDeviceInput;
//        return true;
//    }
//
//    const void* readCalibrationCache(size_t& length)
//    {
//        mCalibrationCache.clear();
//        std::ifstream input(mCalibrationTableName, std::ios::binary);
//        input >> std::noskipws;
//        if (mReadCache && input.good())
//        {
//            std::copy(std::istream_iterator<char>(input), std::istream_iterator<char>(),
//                std::back_inserter(mCalibrationCache));
//        }
//        length = mCalibrationCache.size();
//        return length ? mCalibrationCache.data() : nullptr;
//    }
//
//    void writeCalibrationCache(const void* cache, size_t length)
//    {
//        std::ofstream output(mCalibrationTableName, std::ios::binary);
//        output.write(reinterpret_cast<const char*>(cache), length);
//    }
//
// private:
//    BatchStream mStream;
//    size_t mInputCount;
//    std::string mCalibrationTableName;
//    const char* mInputBlobName;
//    bool mReadCache{true};
//    void* mDeviceInput{nullptr};
//    std::vector<char> mCalibrationCache;
//};

//! \class Int8EntropyCalibrator2
//!
//! \brief Implements Entropy calibrator 2.
//!  CalibrationAlgoType is kENTROPY_CALIBRATION_2.
//!
//class Int8EntropyCalibrator2 : public nvinfer1::IInt8EntropyCalibrator2
//{
//public:
//    Int8EntropyCalibrator2(
//        BatchStream& stream, int firstBatch, const char* networkName, const char* inputBlobName, bool readCache = true)
//        : mImpl(stream, firstBatch, networkName, inputBlobName, readCache)
//    {
//    }
//
//    int getBatchSize() const override
//    {
//        return mImpl.getBatchSize();
//    }
//
//    bool getBatch(void* bindings[], const char* names[], int nbBindings) override
//    {
//        return mImpl.getBatch(bindings, names, nbBindings);
//    }
//
//    const void* readCalibrationCache(size_t& length) override
//    {
//        return mImpl.readCalibrationCache(length);
//    }
//
//    void writeCalibrationCache(const void* cache, size_t length) override
//    {
//        mImpl.writeCalibrationCache(cache, length);
//    }
//
//private:
//    EntropyCalibratorImpl mImpl;
//};

class RndInt8Calibrator : public nvinfer1::IInt8EntropyCalibrator2
{
public:
    RndInt8Calibrator(int totalSamples, std::map<std::string, nvinfer1::Dims3> inputDimensions, std::string cacheFile)
        : mTotalSamples(totalSamples)
        , mCurrentSample(0)
        , mInputDimensions(std::move(inputDimensions))
        , mCacheFile(std::move(cacheFile))
    {
        std::default_random_engine generator;
        std::uniform_real_distribution<float> distribution(-1.0F, 1.0F);
        for (auto& elem : mInputDimensions)
        {
            int elemCount = get_shape_size(elem.second);

            std::vector<float> rnd_data(elemCount);
            for (auto& val : rnd_data)
                val = distribution(generator);

            void* data;
            CUASSERT(cudaMalloc(&data, elemCount * sizeof(float)));
            CUASSERT(cudaMemcpy(data, &rnd_data[0], elemCount * sizeof(float), cudaMemcpyHostToDevice));

            mInputDeviceBuffers.insert(std::make_pair(elem.first, data));
        }
    }

    ~RndInt8Calibrator() override
    {
        for (auto& elem : mInputDeviceBuffers)
            CUASSERT(cudaFree(elem.second));
    }

    int getBatchSize() const override
    {
        return 1;
    }

    bool getBatch(void* bindings[], const char* names[], int nbBindings) override
    {
        if (mCurrentSample >= mTotalSamples)
            return false;

        for (int i = 0; i < nbBindings; ++i)
            bindings[i] = mInputDeviceBuffers[names[i]];

        ++mCurrentSample;
        return true;
    }

    const void* readCalibrationCache(size_t& length) override
    {
//        mCalibrationCache.clear();
//        std::ifstream input(mCacheFile, std::ios::binary);
//        input >> std::noskipws;
//        if (input.good())
//            std::copy(std::istream_iterator<char>(input), std::istream_iterator<char>(),
//                std::back_inserter(mCalibrationCache));
//
//        length = mCalibrationCache.size();
//        return length ? &mCalibrationCache[0] : nullptr;
        return nullptr;
    }

    void writeCalibrationCache(const void* cache, size_t length) override
    {
        std::ofstream output(mCacheFile, std::ios::binary);
        output.write(reinterpret_cast<const char*>(cache), length);
    }

private:
    int mTotalSamples;
    int mCurrentSample;
    std::string mCacheFile;
    std::map<std::string, nvinfer1::Dims3> mInputDimensions;
    std::map<std::string, void*> mInputDeviceBuffers;
    std::vector<char> mCalibrationCache;
};

} // namespace trtParser
} // namespace arctern

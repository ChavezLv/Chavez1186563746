#pragma once

#include "shapedWeights.hpp"

#include <NvInfer.h>
#include <cassert>

namespace arctern
{
namespace trtParser
{
class TensorOrWeights
{
    union
    {
        nvinfer1::ITensor* _tensor;
        ShapedWeights _weights;
    };
    enum
    {
        NODE_TENSOR,
        NODE_WEIGHTS
    } _variant;

public:
    inline TensorOrWeights()
        : _tensor(nullptr)
        , _variant(NODE_TENSOR)
    {
    }
    inline TensorOrWeights(nvinfer1::ITensor* tensor)
        : _tensor(tensor)
        , _variant(NODE_TENSOR)
    {
    }
    inline TensorOrWeights(ShapedWeights const& weights)
        : _weights(weights)
        , _variant(NODE_WEIGHTS)
    {
    }
    inline bool is_tensor() const
    {
        return _variant == NODE_TENSOR;
    }
    inline bool is_weights() const
    {
        return _variant == NODE_WEIGHTS;
    }
    inline nvinfer1::ITensor& tensor()
    {
        assert(this->is_tensor());
        return *_tensor;
    }
    inline nvinfer1::ITensor const& tensor() const
    {
        assert(this->is_tensor());
        return *_tensor;
    }
    inline ShapedWeights& weights()
    {
        assert(this->is_weights());
        return _weights;
    }
    inline ShapedWeights const& weights() const
    {
        assert(this->is_weights());
        return _weights;
    }
    inline nvinfer1::Dims shape() const
    {
        return this->is_tensor() ? _tensor->getDimensions() : _weights.shape;
    }
    inline operator bool() const
    {
        return this->is_tensor() ? (bool) _tensor : (bool) _weights;
    }
    nvinfer1::ITensor* reset_tensor(nvinfer1::ITensor* tensor)
    {
        assert(this->is_tensor());
        return _tensor = tensor;
    }
};

} // namespace trtParser
} // namespace arctern
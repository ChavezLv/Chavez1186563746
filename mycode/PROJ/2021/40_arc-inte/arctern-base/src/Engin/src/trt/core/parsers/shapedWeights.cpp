#include "shapedWeights.hpp"
#include "trtUtils.hpp"

namespace arctern
{
namespace trtParser
{
bool convertINT64(void* weightValues, const size_t nbWeights, std::vector<int32_t>& converted_weights)
{
    int64_t* weights = static_cast<int64_t*>(weightValues);
    for (size_t i = 0; i < nbWeights; i++)
    {
        if (weights[i] > INT32_MAX || weights[i] < INT32_MIN)
        {
            return false;
        }
        else
        {
            converted_weights[i] = static_cast<int32_t>(weights[i]);
        }
    }
    return true;
}

size_t ShapedWeights::count() const
{
    if (this->shape.nbDims == 0)
    {
        return 0;
    }
    else
    {
        size_t c = 1;
        for (int i = 0; i < this->shape.nbDims; ++i)
        {
            c *= this->shape.d[i];
        }
        return c;
    }
}

ShapedWeights ShapedWeights::empty(DataType type)
{
    return ShapedWeights(type, nullptr, nvinfer1::Dims{0});
}

ShapedWeights::ShapedWeights()
    : values(nullptr)
    , shape{0}
{
}

ShapedWeights::ShapedWeights(DataType type, void* values, nvinfer1::Dims shape)
    : type(type)
    , values(values)
    , shape(shape)
{
    // Note: this->shape.type[] is not used
}

size_t ShapedWeights::size_bytes() const
{
    return this->count() * get_dtype_size(this->type);
}

ShapedWeights::operator bool() const
{
    return (bool) this->values;
}

ShapedWeights::operator nvinfer1::Weights() const
{
    nvinfer1::Weights w{};
    w.values = this->values;
    w.type = this->type;
    w.count = this->count();
    return w;
}

template <typename DType>
void transpose2DWeights(ShapedWeights const& weights, nvinfer1::Dims const& new_shape, ShapedWeights* result)
{
    DType const* src = reinterpret_cast<DType*>(weights.values);
    DType* dst = reinterpret_cast<DType*>(result->values);
    int src_stride = weights.shape.d[1];
    int dst_stride = result->shape.d[1];
    for (int i = 0; i < new_shape.d[0]; ++i)
    {
        for (int j = 0; j < new_shape.d[1]; ++j)
        {
            dst[i * dst_stride + j] = src[j * src_stride + i];
        }
    }
}

bool transposeWeights(ShapedWeights const& weights, nvinfer1::Permutation const& perm, ShapedWeights* result)
{
    nvinfer1::Dims shape = weights.shape;
    nvinfer1::Dims new_shape;
    new_shape.nbDims = shape.nbDims;
    for (int d = 0; d < shape.nbDims; ++d)
    {
        new_shape.d[d] = shape.d[perm.order[d]];
        result->shape.d[d] = new_shape.d[d];
    }
    // TODO: Need to generalize this transpose implementation
    assert(perm.order[0] == 1 && perm.order[1] == 0);
    if (shape.nbDims == 2)
    {
        if (weights.type == nvinfer1::DataType::kFLOAT)
        {
            transpose2DWeights<float>(weights, new_shape, result);
        }
        else if (weights.type == nvinfer1::DataType::kHALF)
        {
            transpose2DWeights<uint16_t>(weights, new_shape, result);
        }
        else
        {
            return false;
        }
    }
    else
    {
        // TODO: Implement general transposes and multiple data types
        // Unsupported weights transpose
        return false;
    }
    return true;
}

} // namespace trtParser
} // namespace arctern
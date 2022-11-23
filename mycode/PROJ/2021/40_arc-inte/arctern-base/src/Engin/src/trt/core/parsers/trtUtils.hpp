#pragma once

#include "importerContext.hpp"
#include "status.hpp"
#include "tensorOrWeights.hpp"

#include <NvInfer.h>
#include <cassert>
#include <cmath>

namespace arctern
{
namespace trtParser
{
/**
 * \brief Get the dtype size object
 *
 * \param trt_dtype tensorrt data type
 * \return size of data type
 */
inline int get_dtype_size(nvinfer1::DataType trt_dtype)
{
    switch (trt_dtype)
    {
    case nvinfer1::DataType::kFLOAT: return 4;
    case nvinfer1::DataType::kHALF: return 2;
    case nvinfer1::DataType::kINT8: return 1;
    case nvinfer1::DataType::kINT32: return 4;
    default: return -1;
    }
}

/**
 * \brief Get the shape size
 *
 * \param shape tensorrt dims
 */
inline int64_t get_shape_size(nvinfer1::Dims shape)
{
    // Returns total number of elements in shape
    if (shape.nbDims == 0)
    {
        return 0;
    }
    int64_t count = 1;
    for (int d = 0; d < shape.nbDims; ++d)
    {
        count *= shape.d[d];
    }
    return count;
}

/**
 * \brief insert dim value in index
 *
 * \param dims origin dims
 * \param idx insert dim index
 * \param value insert dim value
 */
inline nvinfer1::Dims insert_dim(nvinfer1::Dims const& dims, int idx, int value)
{
    assert(idx < dims.nbDims + 1);
    nvinfer1::Dims new_dims;
    new_dims.nbDims = dims.nbDims + 1;
    for (int i = 0; i < idx; ++i)
    {
        new_dims.d[i] = dims.d[i];
        new_dims.type[i] = dims.type[i];
    }
    new_dims.d[idx] = value;
    for (int i = idx + 1; i < new_dims.nbDims; ++i)
    {
        new_dims.d[i] = dims.d[i - 1];
        new_dims.type[i] = dims.type[i - 1];
    }
    return new_dims;
}

/**
 * \brief remove dim value in index
 *
 * \param dims origin dims
 * \param idx remove dim index
 */
inline nvinfer1::Dims remove_dim(nvinfer1::Dims const& dims, int idx)
{
    assert(idx < dims.nbDims);
    nvinfer1::Dims new_dims;
    new_dims.nbDims = dims.nbDims - 1;
    for (int i = 0; i < idx; ++i)
    {
        new_dims.d[i] = dims.d[i];
        new_dims.type[i] = dims.type[i];
    }
    for (int i = idx; i < new_dims.nbDims; ++i)
    {
        new_dims.d[i] = dims.d[i + 1];
        new_dims.type[i] = dims.type[i + 1];
    }
    // Special case for scalar result (i.e., there was only one dim originally)
    if (new_dims.nbDims == 0)
    {
        new_dims.nbDims = 1;
        new_dims.d[0] = 1;
        new_dims.type[0] = nvinfer1::DimensionType::kCHANNEL;
    }
    return new_dims;
}

/**
 * \brief Adds unitary dimensions on the left
 *
 * \param dims origin dims
 * \param ndim_new number of new dims
 */
inline nvinfer1::Dims expand_dims(nvinfer1::Dims const& dims, int ndim_new)
{
    assert(dims.nbDims <= ndim_new);
    nvinfer1::Dims new_dims;
    new_dims.nbDims = ndim_new;
    int j = 0;
    for (; j < ndim_new - dims.nbDims; ++j)
    {
        new_dims.d[j] = 1;
    }
    for (int i = 0; i < dims.nbDims; ++i, ++j)
    {
        new_dims.d[j] = dims.d[i];
    }
    return new_dims;
}

/**
 * \brief remove first dim
 *
 * \param perm
 * \return
 */
inline nvinfer1::Permutation remove_first_dim(nvinfer1::Permutation const& perm)
{
    assert(perm.order[0] == 0);
    nvinfer1::Permutation new_perm;
    int ndim = nvinfer1::Dims::MAX_DIMS;
    for (int i = 0; i < ndim - 1; ++i)
    {
        new_perm.order[i] = perm.order[i + 1] - 1;
    }
    return new_perm;
}

/**
 * \brief squeeze trailing dims
 *
 * \param dims
 * \return
 */
inline nvinfer1::Dims squeeze_trailing_dims(nvinfer1::Dims const& dims)
{
    nvinfer1::Dims new_dims = dims;
    // Note: TRT requires at least one dimension, so we don't squeeze [1]->[]
    while (new_dims.nbDims > 1 && new_dims.d[new_dims.nbDims - 1] == 1)
    {
        --new_dims.nbDims;
    }
    return new_dims;
}

inline nvinfer1::Dims set_dims_CHW(nvinfer1::Dims const& dims)
{
    nvinfer1::Dims new_dims = dims;
    assert(new_dims.nbDims > 0);
    new_dims.type[0] = nvinfer1::DimensionType::kCHANNEL;
    for (int i = 1; i < new_dims.nbDims; ++i)
    {
        new_dims.type[i] = nvinfer1::DimensionType::kSPATIAL;
    }
    return new_dims;
}

inline nvinfer1::DimsHW operator-(nvinfer1::DimsHW dims)
{
    return nvinfer1::DimsHW(-dims.h(), -dims.w());
}

// Note: These are used for checking beg_padding == end_padding
inline bool operator==(nvinfer1::Dims const& a, nvinfer1::Dims const& b)
{
    if (a.nbDims != b.nbDims)
    {
        return false;
    }
    for (int i = 0; i < a.nbDims; ++i)
    {
        if (a.d[i] != b.d[i])
        {
            return false;
        }
    }
    return true;
}
inline bool operator!=(nvinfer1::Dims const& a, nvinfer1::Dims const& b)
{
    return !(a == b);
}

inline nvinfer1::DimsHW get_DimsHW_from_CHW(nvinfer1::Dims dims)
{
    assert(dims.nbDims == 3);
    return nvinfer1::DimsHW(dims.d[1], dims.d[2]);
}

inline TensorOrWeights identity(ImporterContext* ctx, TensorOrWeights input)
{
    if (input.is_weights())
    {
        return input;
    }
    else
    {
        auto* layer = ctx->network()->addShuffle(input.tensor());
        if (!layer)
        {
            return nullptr;
        }
        return layer->getOutput(0);
    }
}

template <typename T, class Func>
void apply_unary_function(T const* idata, T* odata, size_t count, Func func)
{
    for (size_t i = 0; i < count; ++i)
    {
        odata[i] = func(idata[i]);
    }
}

template <typename T>
Status apply_unary_function(T const* idata, T* odata, size_t count, nvinfer1::UnaryOperation func)
{
#define TRTUTILS_APPLY_UNARY_FUNC(func) apply_unary_function(idata, odata, count, [](T x) { return func; })

    using namespace nvinfer1;
    switch (func)
    {
    case UnaryOperation::kEXP: TRTUTILS_APPLY_UNARY_FUNC(exp(x)); break;
    case UnaryOperation::kLOG: TRTUTILS_APPLY_UNARY_FUNC(log(x)); break;
    case UnaryOperation::kSQRT: TRTUTILS_APPLY_UNARY_FUNC(sqrt(x)); break;
    case UnaryOperation::kRECIP: TRTUTILS_APPLY_UNARY_FUNC(T(1) / x); break;
    case UnaryOperation::kABS: TRTUTILS_APPLY_UNARY_FUNC(fabs(x)); break;
    case UnaryOperation::kNEG: TRTUTILS_APPLY_UNARY_FUNC(-x); break;
    default: return MAKE_ERROR("Unsupported unary function", ErrorCode::kUNSUPPORTED_NODE);
    }
    return Status::success();

#undef TRTUTILS_APPLY_UNARY_FUNC
}

// TODO: This actually uses a combination of ONNX and TRT, so it may belong
//       in a different file.
inline Status apply_unary_function(
    ShapedWeights const& iweights, ShapedWeights* oweights, nvinfer1::UnaryOperation func)
{
    assert(iweights.type == oweights->type);
    assert(iweights.shape == oweights->shape);
    void const* idata = iweights.values;
    void* odata = const_cast<void*>(oweights->values);
    size_t count = iweights.count();
    switch (iweights.type)
    {
    case nvinfer1::DataType::kFLOAT: return apply_unary_function((float*) idata, (float*) odata, count, func);
    default: return MAKE_ERROR("Unsupported weights data type for unary function", ErrorCode::kUNSUPPORTED_NODE);
    }
}

ValueOrStatus<std::vector<TensorOrWeights>> inline apply_unary_function(
    ImporterContext* ctx, TensorOrWeights& input, nvinfer1::UnaryOperation func)
{
    if (input.is_weights())
    {
        ShapedWeights const& weights = input.weights();
        ShapedWeights new_weights = ctx->createTempWeights(weights.type, weights.shape);
        // TODO: This is a bit ugly (but safe because they share the same values
        //       pointer).
        TRT_CHECK(apply_unary_function(weights, &new_weights, func));
        return {{new_weights}};
    }
    else
    {
        auto* layer = ctx->network()->addUnary(input.tensor(), func);
        ASSERT(layer, ErrorCode::kUNSUPPORTED_NODE);
        return {{layer->getOutput(0)}};
    }
}

} // namespace trtParser
} // namespace arctern
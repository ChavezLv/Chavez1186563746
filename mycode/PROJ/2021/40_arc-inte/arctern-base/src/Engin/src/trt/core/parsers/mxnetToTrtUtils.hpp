#pragma once

#include <algorithm>
#include <iostream>
#include <regex>

#include <NvInfer.h>

#include "mxnetModelParser.hpp"
#include "shapedWeights.hpp"
#include "trtUtils.hpp"

using std::cerr;
using std::endl;

inline std::ostream& operator<<(std::ostream& stream, nvinfer1::Dims const& shape)
{
    if (shape.nbDims == 0)
    {
        return stream;
    }
    stream << "(" << shape.d[0];
    for (int i = 1; i < shape.nbDims; ++i)
    {
        stream << ", " << shape.d[i];
    }
    stream << ")";
    return stream;
}

inline std::ostream& operator<<(std::ostream& stream, nvinfer1::DataType const& dtype)
{
    switch (dtype)
    {
    case nvinfer1::DataType::kFLOAT: return stream << "float32";
    case nvinfer1::DataType::kHALF: return stream << "float16";
    case nvinfer1::DataType::kINT8: return stream << "int8";
    case nvinfer1::DataType::kINT32: return stream << "int32";
    default: throw std::runtime_error("Unknown dtype");
    }
}

inline std::ostream& operator<<(std::ostream& stream, nvinfer1::Permutation const& perm)
{
    int ndims = nvinfer1::Dims::MAX_DIMS;
    stream << "(" << perm.order[0];
    for (int i = 1; i < ndims; ++i)
    {
        stream << ", " << perm.order[i];
    }
    stream << ")";
    return stream;
}

namespace arctern
{
namespace trtParser
{

template <typename _Ty>
_Ty Str2Num(
    const std::string& str, _Ty _min = std::numeric_limits<_Ty>::min(), _Ty _max = std::numeric_limits<_Ty>::max())
{
    std::istringstream iss(str);
    _Ty val;
    iss >> val;
    return val;
}

template <typename _Ty>
_Ty Str2Bool(const std::string& str)
{
    return str == "True" || str == "true" || str == "1";
}

template <typename _Ty>
std::vector<_Ty> Str2Vector(std::string str)
{
    std::vector<_Ty> ret;
    if (str.empty())
    {
        return ret;
    }
    char startChar = '(';
    char endChar = ')';
    if (str.find(startChar) == std::string::npos) {
       startChar = '[';
       endChar = ']';
    }
    std::string::size_type beg = str.find(startChar);
    std::string::size_type end = str.rfind(endChar);

    if (beg >= std::string::npos || end >= std::string::npos || end - beg <= 1)
    {
        return ret;
    }

    str.erase(end, -1);
    str.erase(0, beg + 1);
    std::istringstream iss(str);
    for (std::string strVal; std::getline(iss, strVal, ',');)
    {
        std::istringstream isv(strVal);
        _Ty val;
        isv >> val;
        ret.push_back(val);
    }
    return ret;
}

// Returns the input if it is already a tensor. If it is of type ShapedWeights, adds a new
// constant layer to the TRT network and returns its output.
inline nvinfer1::ITensor& convertToTensor(TensorOrWeights& input, ImporterContext* ctx)
{
    if (input.is_tensor())
    {
        return input.tensor();
    }
    else
    {
        // Handle non-tensor indices input by adding a new constant layer to the network.
        const ShapedWeights& weights = input.weights();
        return *(ctx->network()->addConstant(weights.shape, weights)->getOutput(0));
    }
}

inline bool convert_mxnet_weights(MxnetParam const& param, ShapedWeights* weights)
{
    nvinfer1::Dims shape{};
    shape.nbDims = param.shape.size();
    for (size_t i = 0; i < shape.nbDims; ++i)
    {
        shape.d[i] = param.shape[i];
    }
    // Special case for scalars
    if (shape.nbDims == 0)
    {
        shape.nbDims = 1;
        shape.d[0] = 1;
        shape.type[0] = nvinfer1::DimensionType::kCHANNEL;
    }

    // TODO : only support FLOAT
    ShapedWeights trt_weights(nvinfer1::DataType::kFLOAT, (void*) param.data.data(), shape);
    *weights = trt_weights;
    return true;
}

inline nvinfer1::ITensor& convert_output_weight_to_tensor(TensorOrWeights& input, ImporterContext* ctx)
{
    if (input.is_tensor())
    {
        return input.tensor();
    }
    else
    {
        // Convert weight output to tensor output. Strip batch dimension here.
        const ShapedWeights& weights = input.weights();
        nvinfer1::Dims tensor_shape = weights.shape;
        tensor_shape = set_dims_CHW(remove_dim(tensor_shape, 0));
        return *(ctx->network()->addConstant(tensor_shape, weights)->getOutput(0));
    }
}

inline int div_ceil(int n, int d)
{
    return (n - 1) / d + 1;
}

// Convert an Mxnet axis into a TRT axis
inline Status convert_axis(int& axis, int nbDims)
{
    // Support negative indexing
    if (axis < 0)
    {
        axis += nbDims;
    }
    ASSERT(axis >= 0 && axis < nbDims, ErrorCode::kUNSUPPORTED_NODE);
    return Status::success();
}

inline int get_conv_output_size(int input_size, int filter_size, int stride, int dilation_rate, int total_padding)
{
    // This is based on the CUDNN formula
    int effective_input_size = input_size + total_padding;
    int effective_filter_size = (filter_size - 1) * dilation_rate + 1;
    return div_ceil(effective_input_size - (effective_filter_size - 1), stride);
}

inline nvinfer1::ScaleMode get_scale_mode(nvinfer1::Dims const& weights_shape)
{
    if (weights_shape.nbDims == 1)
    {
        if (weights_shape.d[0] == 1)
        {
            return nvinfer1::ScaleMode::kUNIFORM;
        }
        else
        {
            return nvinfer1::ScaleMode::kCHANNEL;
        }
    }
    else
    {
        return nvinfer1::ScaleMode::kELEMENTWISE;
    }
}

inline Status get_infer_dim(const nvinfer1::Dims& shape, nvinfer1::Dims& new_shape, bool reverse)
{
    std::vector<int> param_shape_vec(new_shape.nbDims);
    for (size_t i = 0; i < new_shape.nbDims; ++i)
    {
        param_shape_vec[i] = new_shape.d[i];
    }

    std::vector<int> dshape_vec(shape.nbDims);
    for (size_t i = 0; i < shape.nbDims; ++i)
    {
        dshape_vec[i] = shape.d[i];
    }

    std::vector<int> tmp;
    size_t src_idx = 0;
    int inf_idx = -1;
    if (reverse)
    {
        std::reverse(dshape_vec.begin(), dshape_vec.end());
        std::reverse(param_shape_vec.begin(), param_shape_vec.end());
    }
    auto dshape_len = dshape_vec.size();
    auto params_len = param_shape_vec.size();
    for (size_t i = 0; i < params_len; ++i)
    {
        auto proposed_dim = param_shape_vec[i];
        if (proposed_dim == 0)
        {
            // keep same
            ASSERT(src_idx < dshape_len, ErrorCode::kINVALID_VALUE);
            tmp.push_back(dshape_vec[src_idx++]);
        }
        else if (proposed_dim == -1)
        {
            // infer
            ASSERT_INPUT(inf_idx < 0, ErrorCode::kINVALID_VALUE, "One and only one dim can be inferred");
            inf_idx = i;
            tmp.push_back(1);
            src_idx++;
        }
        else if (proposed_dim == -2)
        {
            // copy all remaining dims from source
            while (src_idx < dshape_len)
            {
                size_t dn = dshape_vec[src_idx++];
                tmp.push_back(dn);
            }
        }
        else if (proposed_dim == -3)
        {
            // merge two dims from source
            ASSERT(src_idx < dshape_len - 1, ErrorCode::kINVALID_VALUE);
            size_t d1 = dshape_vec[src_idx++];
            size_t d2 = dshape_vec[src_idx++];
            size_t dn = d1 * d2;
            tmp.push_back(dn);
        }
        else if (proposed_dim == -4)
        {
            // split the source dim s into two dims
            // read the left dim and then the right dim (either can be -1)
            ASSERT(i + 2 < params_len, ErrorCode::kINVALID_VALUE);
            ASSERT(src_idx < dshape_len, ErrorCode::kINVALID_VALUE);
            size_t d0 = dshape_vec[src_idx++];
            auto d1 = param_shape_vec[++i];
            auto d2 = param_shape_vec[++i];
            ASSERT_INPUT(d1 != -1 || d2 != -1, ErrorCode::kINVALID_VALUE, "Split dims cannot both be -1.");
            if (d1 == -1)
                d1 = d0 / d2;
            if (d2 == -1)
                d2 = d0 / d1;
            ASSERT(d1 * d2 == static_cast<int>(d0) || static_cast<int>(d0) == int(0), ErrorCode::kINVALID_VALUE);
            tmp.push_back(d1);
            tmp.push_back(d2);
        }
        else
        {
            // greater than 0, new shape
            tmp.push_back(proposed_dim);
            src_idx++;
        }
    }

    if (inf_idx >= 0)
    {
        if (get_shape_size(shape) > 0)
        {
            int new_size = 1;
            for (auto x : tmp)
                new_size *= x;
            tmp[inf_idx] = get_shape_size(shape) / new_size;
        }
        else
        {
            tmp[inf_idx] = 0;
        }
    }
    if (reverse)
    {
        std::reverse(param_shape_vec.begin(), param_shape_vec.end());
        std::reverse(dshape_vec.begin(), dshape_vec.end());
        std::reverse(tmp.begin(), tmp.end());
    }
    for (size_t i = 0; i < new_shape.nbDims; ++i)
    {
        new_shape.d[i] = tmp[i];
    }
    return Status::success();
}

} // namespace trtParser
} // namespace arctern

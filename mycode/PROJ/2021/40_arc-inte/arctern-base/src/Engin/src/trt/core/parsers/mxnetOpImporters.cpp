#include <functional>

#include "attributes.hpp"
#include "mxnetModelParser.hpp"
#include "mxnetOpImporters.hpp"
#include "mxnetToTrtUtils.hpp"
#include "../plugin/inferPlugin.h"
#include <NvInferPlugin.h>
#include <set>
namespace arctern
{
namespace trtParser
{

using AttrProc = std::function<void(std::string)>;
using AttrProcMap = std::map<std::string, AttrProc>;

void ProcAttrs(const MxnetNode& mxnet_node, const AttrProcMap& procMap, bool bRequired)
{
    for (const auto& attrProc : procMap)
    {
        std::string strVal = mxnet_node.attrs.GetValue(attrProc.first, bRequired);
        if (!strVal.empty() && attrProc.second != nullptr)
        {
            attrProc.second(strVal);
        }
    }
}

string_map<MxnetNodeImporter>& getMxnetOpImporterMap()
{
    static string_map<MxnetNodeImporter> mxnet_op_importers;
    return mxnet_op_importers;
}

enum
{
    BATCH_DIM = 0,
    CHANNEL_DIM = 1
};

// Returns false if the transpose does not require any data movement (i.e., it's equivalent to a reshape)
bool is_transpose_required(nvinfer1::Dims const& shape, nvinfer1::Permutation const& perm)
{
    int ndim = shape.nbDims;
    int prev_significant_dim = 0;
    for (int dst_i = 0; dst_i < ndim; ++dst_i)
    {
        int src_i = perm.order[dst_i];
        if (shape.d[src_i] != 1)
        {
            if (src_i < prev_significant_dim)
            {
                return false;
            }
            prev_significant_dim = src_i;
        }
    }
    return true;
}

// Note: perm should not include the batch dim
nvinfer1::ITensor* transpose_tensor(
    ImporterContext* ctx, nvinfer1::ITensor& tensor, nvinfer1::Permutation const& perm, bool permute_dim_types = true)
{
    nvinfer1::IShuffleLayer* layer = ctx->network()->addShuffle(tensor);
    if (!layer)
    {
        return nullptr;
    }
    nvinfer1::Dims shape = tensor.getDimensions();
    // Check if we need to transpose the data
    if (!is_transpose_required(shape, perm))
    {
        layer->setFirstTranspose(perm);
    }
    // Transpose can be simplified to be a reshape if no data re-ordering is required.
    else
    {
        nvinfer1::Dims new_shape{};
        new_shape.nbDims = shape.nbDims;
        for (int i = 0; i < new_shape.nbDims; i++)
        {
            new_shape.d[i] = shape.d[perm.order[i]];
        }
        layer->setReshapeDimensions(new_shape);
    }
    return layer->getOutput(0);
}

nvinfer1::ITensor* move_tensor_dimension(ImporterContext* ctx, nvinfer1::ITensor& tensor, int from, int to)
{
    int ndim = tensor.getDimensions().nbDims;
    if (!(0 <= from && from < ndim))
    {
        return nullptr;
    }
    if (!(0 <= to && to < ndim))
    {
        return nullptr;
    }
    std::vector<int> vperm;
    vperm.reserve(ndim);
    for (int i = 0; i < ndim; ++i)
    {
        vperm.push_back(i);
    }
    vperm.erase(vperm.begin() + from);
    vperm.insert(vperm.begin() + to, from);
    nvinfer1::Permutation perm;
    std::copy(vperm.begin(), vperm.end(), perm.order);
    return transpose_tensor(ctx, tensor, perm, false);
}

nvinfer1::ITensor* reshape_tensor(ImporterContext* ctx, nvinfer1::ITensor& tensor, nvinfer1::Dims shape)
{
    if (shape == tensor.getDimensions())
    {
        return &tensor;
    }
    nvinfer1::IShuffleLayer* layer = ctx->network()->addShuffle(tensor);
    if (!layer)
    {
        return nullptr;
    }
    layer->setReshapeDimensions(shape);
    return layer->getOutput(0);
}

nvinfer1::ITensor* flatten_tensor(ImporterContext* ctx, nvinfer1::ITensor& tensor, int axis = 0)
{
    nvinfer1::Dims shape = tensor.getDimensions();
    nvinfer1::Dims new_shape = shape;
    for (int i = axis + 1; i < shape.nbDims; ++i)
    {
        new_shape.d[axis] *= shape.d[i];
        new_shape.d[i] = 1;
    }
    return reshape_tensor(ctx, tensor, new_shape);
}

MxnetNodeImportResult unaryHelper(
    ImporterContext* ctx, MxnetNode const& node, std::vector<TensorOrWeights>& inputs, nvinfer1::UnaryOperation op)
{
    nvinfer1::ITensor& input = convertToTensor(inputs.at(0), ctx);
    nvinfer1::IUnaryLayer* layer = ctx->network()->addUnary(input, op);
    return {{layer->getOutput(0)}};
}

MxnetNodeImportResult activationHelper(ImporterContext* ctx, const MxnetNode& node,
    std::vector<TensorOrWeights>& inputs, nvinfer1::ActivationType op, const float* alpha = nullptr,
    const float* beta = nullptr)
{
    nvinfer1::ITensor& input = convertToTensor(inputs.at(0), ctx);
    nvinfer1::IActivationLayer* layer = ctx->network()->addActivation(input, op);
    if (alpha)
    {
        layer->setAlpha(*alpha);
    }
    if (beta)
    {
        layer->setBeta(*beta);
    }

    return {{layer->getOutput(0)}};
}

// Adds a constant scalar to the network in the form of a constant layer.
template <typename ScalarType>
nvinfer1::IConstantLayer* addConstantScalar(ImporterContext* ctx, ScalarType scalar, ShapedWeights::DataType type)
{
    ShapedWeights scalarWeights = ctx->createTempWeights(type, nvinfer1::Dims{0});
    static_cast<ScalarType*>(scalarWeights.values)[0] = scalar;
    return ctx->network()->addConstant(scalarWeights.shape, scalarWeights);
}

MxnetNodeImportResult addScale(ImporterContext* ctx, nvinfer1::ITensor& tensor_, nvinfer1::ScaleMode mode,
    nvinfer1::Weights shift, nvinfer1::Weights scale, nvinfer1::Weights power)
{
    nvinfer1::ITensor* tensor_ptr = &tensor_;
    nvinfer1::Dims dims = tensor_ptr->getDimensions();

    bool need_to_expand_dims = (dims.nbDims != 4);
    nvinfer1::Dims orig_shape = dims;
    if (need_to_expand_dims)
    {
        // Expand or squash dims to 3D
        nvinfer1::Dims new_shape = dims;
        while (new_shape.nbDims < 4)
        {
            new_shape.d[new_shape.nbDims++] = 1;
        }
        while (new_shape.nbDims > 4)
        {
            new_shape.d[3] *= new_shape.d[--new_shape.nbDims];
        }
        tensor_ptr = reshape_tensor(ctx, *tensor_ptr, new_shape);
        ASSERT(tensor_ptr, ErrorCode::kUNSUPPORTED_NODE);
        dims = tensor_ptr->getDimensions();
    }

    ASSERT(dims.nbDims == 4, ErrorCode::kUNSUPPORTED_NODE);
    // Fill in dtype for any unused (dummy) weights
    nvinfer1::DataType* dtype_ptr = nullptr;
    if (shift.count)
    {
        dtype_ptr = &shift.type;
    }
    if (scale.count)
    {
        ASSERT(!dtype_ptr || *dtype_ptr == scale.type, ErrorCode::kUNSUPPORTED_NODE);
        dtype_ptr = &scale.type;
    }
    if (power.count)
    {
        ASSERT(!dtype_ptr || *dtype_ptr == power.type, ErrorCode::kUNSUPPORTED_NODE);
        dtype_ptr = &power.type;
    }
    ASSERT(dtype_ptr, ErrorCode::kINTERNAL_ERROR);
    shift.type = *dtype_ptr;
    scale.type = *dtype_ptr;
    power.type = *dtype_ptr;
    auto* layer = ctx->network()->addScale(*tensor_ptr, mode, shift, scale, power);
    ASSERT(layer, ErrorCode::kUNSUPPORTED_NODE);
    tensor_ptr = layer->getOutput(0);

    if (need_to_expand_dims)
    {
        // Un-expand spatial dims back to 1D
        tensor_ptr = reshape_tensor(ctx, *tensor_ptr, orig_shape);
        ASSERT(tensor_ptr, ErrorCode::kUNSUPPORTED_NODE);
    }
    return {{tensor_ptr}};
}

MxnetNodeImportResult combineTensorsElementwise(ImporterContext* ctx, MxnetNode const& node,
    std::vector<TensorOrWeights>& inputs, nvinfer1::ElementWiseOperation binary_op,
    bool legacy_binary_op_broadcasting = false)
{
    ASSERT(!inputs.empty(), ErrorCode::kINVALID_NODE);
    std::vector<nvinfer1::ITensor*> input_tensors;
    int ndim_max = -1;
    int tensors_ndim_max = -1;
    for (auto input : inputs)
    {
        ndim_max = std::max(ndim_max, input.shape().nbDims);
        // Note: Tensor dims always exclude the batch dim, but weights may not
        if (input.is_tensor())
        {
            tensors_ndim_max = std::max(tensors_ndim_max, input.shape().nbDims);
        }
    }
    for (auto input : inputs)
    {
        nvinfer1::ITensor* tensor_ptr;
        if (input.is_weights())
        {
            auto weights = input.weights();
            // Note: TRT supports broadcasting, but ranks must match
            if (input.shape().nbDims < ndim_max)
            {
                weights.shape = expand_dims(weights.shape, ndim_max);
            }
            if (weights.shape.nbDims == tensors_ndim_max + 1)
            {
                // The weights contain a batch dim, which must be removed
                // Note: TRT Constant layer has implicit batch dim of 1
                ASSERT(weights.shape.d[BATCH_DIM] == 1, ErrorCode::kUNSUPPORTED_NODE);
                weights.shape = remove_dim(weights.shape, BATCH_DIM);
            }
            auto* layer = ctx->network()->addConstant(weights.shape, weights);
            ASSERT(layer, ErrorCode::kUNSUPPORTED_NODE);
            tensor_ptr = layer->getOutput(0);
        }
        else
        {
            tensor_ptr = &input.tensor();
            // Support broadcasting for tensor inputs by expanding dimensions.
            if (tensor_ptr->getDimensions().nbDims != tensors_ndim_max)
            {
                nvinfer1::Dims new_dims = expand_dims(tensor_ptr->getDimensions(), tensors_ndim_max);
                tensor_ptr = reshape_tensor(ctx, *tensor_ptr, new_dims);
            }
            ASSERT(tensor_ptr->getDimensions().nbDims == tensors_ndim_max, ErrorCode::kUNSUPPORTED_NODE);
        }
        input_tensors.push_back(tensor_ptr);
    }
    nvinfer1::ITensor* combined = input_tensors.at(0);
    if (input_tensors.size() == 1)
    {
        // Note: Single input must be wrapped in identity to avoid messing up network outputs
        return {{identity(ctx, combined)}};
    }
    for (size_t i = 1; i < input_tensors.size(); ++i)
    {
        nvinfer1::ITensor* tensor = input_tensors.at(i);
        ASSERT(tensor->getDimensions().nbDims == combined->getDimensions().nbDims, ErrorCode::kUNSUPPORTED_NODE);
        auto* layer = ctx->network()->addElementWise(*combined, *tensor, binary_op);
        ASSERT(layer, ErrorCode::kUNSUPPORTED_NODE);
        combined = layer->getOutput(0);
    }
    return {{combined}};
}

bool registerBuiltinOpImporter(const std::string& op, MxnetNodeImporter const& importer)
{
    bool inserted = getMxnetOpImporterMap().insert({op, importer}).second;
    assert(inserted);
    return inserted;
}

#define IGNORE_UNUSED_GLOBAL(x)                                                                                        \
    static void _ignore_unused2_##x();                                                                                 \
    static void _ignore_unused1_##x()                                                                                  \
    {                                                                                                                  \
        (void) _ignore_unused2_##x;                                                                                    \
        (void) x;                                                                                                      \
    }                                                                                                                  \
    static void _ignore_unused2_##x()                                                                                  \
    {                                                                                                                  \
        (void) _ignore_unused1_##x;                                                                                    \
    }                                                                                                                  \
    struct SwallowSemicolon##x                                                                                         \
    {                                                                                                                  \
    }

#define DEFINE_BUILTIN_OP_IMPORTER(op)                                                                                 \
    MxnetNodeImportResult import##op(                                                                                  \
        ImporterContext* ctx, MxnetNode const& node, std::vector<TensorOrWeights>& inputs);                            \
    static const bool op##_registered_builtin_op = registerBuiltinOpImporter(#op, import##op);                         \
    IGNORE_UNUSED_GLOBAL(op##_registered_builtin_op);                                                                  \
    MxnetNodeImportResult import##op(ImporterContext* ctx, MxnetNode const& node, std::vector<TensorOrWeights>& inputs)

#define RETURN_FIRST_OUTPUT(layer)                                                                                     \
    do                                                                                                                 \
    {                                                                                                                  \
        nvinfer1::ILayer* layer_ptr = layer;                                                                           \
        ASSERT(layer_ptr, ErrorCode::kUNSUPPORTED_NODE);                                                               \
        return {{layer_ptr->getOutput(0)}};                                                                            \
    } while (0)

#define RETURN_SECOND_OUTPUT(layer)                                                                                    \
    do                                                                                                                 \
    {                                                                                                                  \
        nvinfer1::ILayer* layer_ptr = layer;                                                                           \
        ASSERT(layer_ptr, ErrorCode::kUNSUPPORTED_NODE);                                                               \
        return {{layer_ptr->getOutput(1)}};                                                                            \
    } while (0)

#define RETURN_IDENTITY(input)                                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        TensorOrWeights output = identity(ctx, input);                                                                 \
        ASSERT(output, ErrorCode::kUNSUPPORTED_NODE);                                                                  \
        return {{output}};                                                                                             \
    } while (0)

MxnetNodeImportResult reduceTensor(
    ImporterContext* ctx, MxnetNode const& node, TensorOrWeights input, nvinfer1::ReduceOperation operation)
{
    ASSERT(input.is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    nvinfer1::ITensor& tensor = input.tensor();
    int ndim = tensor.getDimensions().nbDims;

    std::vector<int> axis;
    bool keepdims = false;
    bool exclude = false;

    AttrProcMap optAttrProcs;
    optAttrProcs["axis"] = [&](const std::string& strVal) {
        auto mxnet_axis = Str2Num<int>(strVal);
        axis.emplace_back(mxnet_axis);
    };
    optAttrProcs["keepdims"] = [&](const std::string& strVal) {
        auto mxnet_keepdims = Str2Bool<bool>(strVal);
        keepdims = mxnet_keepdims;
    };
    optAttrProcs["exclude"] = [&](const std::string& strVal) {
        auto mxnet_exclude = Str2Bool<bool>(strVal);
        exclude = mxnet_exclude;
    };
    ProcAttrs(node, optAttrProcs, false);

    ASSERT(axis.size() == 1, ErrorCode::kUNSUPPORTED_NODE);
    uint32_t axis_mask = 1 << (axis[0] >= 0 ? axis[0] : axis[0] + ndim);
    RETURN_FIRST_OUTPUT(ctx->network()->addReduce(tensor, operation, axis_mask, keepdims));
}

// Helper for ArgMax/ArgMin
MxnetNodeImportResult argMinMaxHelper(
    ImporterContext* ctx, const MxnetNode& node, std::vector<TensorOrWeights>& inputs, nvinfer1::TopKOperation op)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    nvinfer1::ITensor& tensor = inputs.at(0).tensor();
    ASSERT(tensor.getType() != nvinfer1::DataType::kINT32, ErrorCode::kUNSUPPORTED_NODE);
    int nbDims = tensor.getDimensions().nbDims;

    // Get attributes.
    int axis;
    bool keepdims = false;

    AttrProcMap optAttrProcs;
    optAttrProcs["axis"] = [&](const std::string& strVal) {
        auto mxnet_axis = Str2Num<int>(strVal);
        axis = mxnet_axis;
    };
    optAttrProcs["keepdims"] = [&](const std::string& strVal) {
        auto mxnet_keepdims = Str2Bool<bool>(strVal);
        keepdims = mxnet_keepdims;
    };

    ProcAttrs(node, optAttrProcs, false);

    // Adjust axis to TensorRT format
    TRT_CHECK(convert_axis(axis, nbDims));

    uint32_t axisMask = 1 << axis;
    // Insert a TopK layer with k set to 1.
    nvinfer1::ITopKLayer* layer = ctx->network()->addTopK(tensor, op, 1, axisMask);
    ASSERT(layer, ErrorCode::kUNSUPPORTED_NODE);
    // We don't care about the TopK values, just the indices.
    nvinfer1::ITensor* indices = layer->getOutput(1);
    indices->setType(nvinfer1::DataType::kINT32);
    if (keepdims)
    {
        // The default behavior of the TopK layer is to keepdims.
        return {{indices}};
    }
    else
    {
        // Otherwise, we need to squeeze the axis dimension - we achieve this by reshaping.
        // The new dimensions are just the old dimensions with all values after axis shifted over.
        nvinfer1::Dims reshapeDims = indices->getDimensions();
        --reshapeDims.nbDims;
        // The axis dimension should be reduced to size 1 after performing the reduction.
        ASSERT(reshapeDims.d[axis] == 1, ErrorCode::kINVALID_VALUE);
        for (int i = axis; i < reshapeDims.nbDims; ++i)
        {
            reshapeDims.d[i] = reshapeDims.d[i + 1];
        }
        nvinfer1::IShuffleLayer* squeezeLayer = ctx->network()->addShuffle(*indices);
        squeezeLayer->setReshapeDimensions(reshapeDims);
        return {{squeezeLayer->getOutput(0)}};
    }
}

// TIPS : NO TEST
DEFINE_BUILTIN_OP_IMPORTER(abs)
{
    return apply_unary_function(ctx, inputs.at(0), nvinfer1::UnaryOperation::kABS);
}

// TIPS : NO TEST
DEFINE_BUILTIN_OP_IMPORTER(arccos)
{
    return unaryHelper(ctx, node, inputs, nvinfer1::UnaryOperation::kACOS);
}

// TIPS : NO TEST
DEFINE_BUILTIN_OP_IMPORTER(arccosh)
{
    return unaryHelper(ctx, node, inputs, nvinfer1::UnaryOperation::kACOSH);
}

DEFINE_BUILTIN_OP_IMPORTER(elemwise_add)
{
    ASSERT(inputs.size() == 2, ErrorCode::kINVALID_NODE);
    return combineTensorsElementwise(ctx, node, inputs, nvinfer1::ElementWiseOperation::kSUM, true);
}

DEFINE_BUILTIN_OP_IMPORTER(broadcast_add)
{
    ASSERT(inputs.size() == 2, ErrorCode::kINVALID_NODE);
    return combineTensorsElementwise(ctx, node, inputs, nvinfer1::ElementWiseOperation::kSUM, true);
}

DEFINE_BUILTIN_OP_IMPORTER(elemwise_sub)
{
    ASSERT(inputs.size() == 2, ErrorCode::kINVALID_NODE);
    return combineTensorsElementwise(ctx, node, inputs, nvinfer1::ElementWiseOperation::kSUB, true);
}

DEFINE_BUILTIN_OP_IMPORTER(broadcast_sub)
{
    ASSERT(inputs.size() == 2, ErrorCode::kINVALID_NODE);
    return combineTensorsElementwise(ctx, node, inputs, nvinfer1::ElementWiseOperation::kSUB, true);
}

DEFINE_BUILTIN_OP_IMPORTER(broadcast_div)
{
  ASSERT(inputs.size() == 2, ErrorCode::kINVALID_NODE);
  return combineTensorsElementwise(ctx, node, inputs, nvinfer1::ElementWiseOperation::kDIV, true);
}

// TIPS : NO TEST
DEFINE_BUILTIN_OP_IMPORTER(argmax)
{
    return argMinMaxHelper(ctx, node, inputs, nvinfer1::TopKOperation::kMAX);
}

// TIPS : NO TEST
DEFINE_BUILTIN_OP_IMPORTER(argmin)
{
    return argMinMaxHelper(ctx, node, inputs, nvinfer1::TopKOperation::kMIN);
}

// TIPS : NO TEST
DEFINE_BUILTIN_OP_IMPORTER(arcsin)
{
    return unaryHelper(ctx, node, inputs, nvinfer1::UnaryOperation::kASIN);
}

// TIPS : NO TEST
DEFINE_BUILTIN_OP_IMPORTER(arcsinh)
{
    return unaryHelper(ctx, node, inputs, nvinfer1::UnaryOperation::kASINH);
}

// TIPS : NO TEST
DEFINE_BUILTIN_OP_IMPORTER(arctan)
{
    return unaryHelper(ctx, node, inputs, nvinfer1::UnaryOperation::kATAN);
}

// TIPS : NO TEST
DEFINE_BUILTIN_OP_IMPORTER(arctanh)
{
    return unaryHelper(ctx, node, inputs, nvinfer1::UnaryOperation::kATANH);
}

DEFINE_BUILTIN_OP_IMPORTER(BatchNorm)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    ASSERT(inputs.at(1).is_weights(), ErrorCode::kUNSUPPORTED_NODE);
    ASSERT(inputs.at(2).is_weights(), ErrorCode::kUNSUPPORTED_NODE);
    ASSERT(inputs.at(3).is_weights(), ErrorCode::kUNSUPPORTED_NODE);
    ASSERT(inputs.at(4).is_weights(), ErrorCode::kUNSUPPORTED_NODE);
    nvinfer1::ITensor& tensor = inputs.at(0).tensor();
    auto scale_weights = inputs.at(1).weights();
    auto bias_weights = inputs.at(2).weights();
    auto mean_weights = inputs.at(3).weights();
    auto variance_weights = inputs.at(4).weights();
    float eps = 1e-3f;
    bool fix_gamma = true;

    AttrProcMap optAttrProcs;
    optAttrProcs["eps"] = [&](const std::string& strVal) {
        auto mxnet_eps = Str2Num<float>(strVal);
        eps = mxnet_eps;
    };
    optAttrProcs["fix_gamma"] = [&](const std::string& strVal) {
        auto mxnet_fix_gamma = Str2Bool<bool>(strVal);
        fix_gamma = mxnet_fix_gamma;
    };
    ProcAttrs(node, optAttrProcs, false);

    ASSERT(scale_weights.type == nvinfer1::DataType::kFLOAT && bias_weights.type == nvinfer1::DataType::kFLOAT
            && mean_weights.type == nvinfer1::DataType::kFLOAT && variance_weights.type == nvinfer1::DataType::kFLOAT,
        ErrorCode::kUNSUPPORTED_NODE);
    nvinfer1::Dims dims = tensor.getDimensions();
    int nchan = dims.d[CHANNEL_DIM];
    nvinfer1::Dims weights_shape{1, {nchan}};
    ASSERT(scale_weights.shape == weights_shape, ErrorCode::kINVALID_NODE);
    ASSERT(bias_weights.shape == weights_shape, ErrorCode::kINVALID_NODE);
    ASSERT(mean_weights.shape == weights_shape, ErrorCode::kINVALID_NODE);
    ASSERT(variance_weights.shape == weights_shape, ErrorCode::kINVALID_NODE);
    auto combined_scale_weights = ctx->createTempWeights(scale_weights.type, scale_weights.shape);
    auto combined_bias_weights = ctx->createTempWeights(bias_weights.type, bias_weights.shape);
    size_t nweight = nchan;
    // Fold the weights together into a single bias and scale
    for (size_t i = 0; i < nweight; ++i)
    {
        float scale = fix_gamma ? 1.0f : (static_cast<float const*>(scale_weights.values))[i];
        float bias = (static_cast<float const*>(bias_weights.values))[i];
        float mean = (static_cast<float const*>(mean_weights.values))[i];
        float variance = (static_cast<float const*>(variance_weights.values))[i];
        float& combined_scale_ref = const_cast<float*>(static_cast<float const*>(combined_scale_weights.values))[i];
        float& combined_bias_ref = const_cast<float*>(static_cast<float const*>(combined_bias_weights.values))[i];
        combined_scale_ref = scale / sqrtf(variance + eps);
        combined_bias_ref = bias - mean * combined_scale_ref;
    }
    return addScale(ctx, tensor, nvinfer1::ScaleMode::kCHANNEL, combined_bias_weights, combined_scale_weights, {});
}

DEFINE_BUILTIN_OP_IMPORTER(_contrib_SyncBatchNorm)
{
  ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
  ASSERT(inputs.at(1).is_weights(), ErrorCode::kUNSUPPORTED_NODE);
  ASSERT(inputs.at(2).is_weights(), ErrorCode::kUNSUPPORTED_NODE);
  ASSERT(inputs.at(3).is_weights(), ErrorCode::kUNSUPPORTED_NODE);
  ASSERT(inputs.at(4).is_weights(), ErrorCode::kUNSUPPORTED_NODE);
  nvinfer1::ITensor& tensor = inputs.at(0).tensor();
  auto scale_weights = inputs.at(1).weights();
  auto bias_weights = inputs.at(2).weights();
  auto mean_weights = inputs.at(3).weights();
  auto variance_weights = inputs.at(4).weights();
  float eps = 1e-3f;
  bool fix_gamma = true;

  AttrProcMap optAttrProcs;
  optAttrProcs["eps"] = [&](const std::string& strVal) {
      auto mxnet_eps = Str2Num<float>(strVal);
      eps = mxnet_eps;
  };
  optAttrProcs["fix_gamma"] = [&](const std::string& strVal) {
      auto mxnet_fix_gamma = Str2Bool<bool>(strVal);
      fix_gamma = mxnet_fix_gamma;
  };
  ProcAttrs(node, optAttrProcs, false);

  ASSERT(scale_weights.type == nvinfer1::DataType::kFLOAT && bias_weights.type == nvinfer1::DataType::kFLOAT
         && mean_weights.type == nvinfer1::DataType::kFLOAT && variance_weights.type == nvinfer1::DataType::kFLOAT,
         ErrorCode::kUNSUPPORTED_NODE);
  nvinfer1::Dims dims = tensor.getDimensions();
  int nchan = dims.d[CHANNEL_DIM];
  nvinfer1::Dims weights_shape{1, {nchan}};
  ASSERT(scale_weights.shape == weights_shape, ErrorCode::kINVALID_NODE);
  ASSERT(bias_weights.shape == weights_shape, ErrorCode::kINVALID_NODE);
  ASSERT(mean_weights.shape == weights_shape, ErrorCode::kINVALID_NODE);
  ASSERT(variance_weights.shape == weights_shape, ErrorCode::kINVALID_NODE);
  auto combined_scale_weights = ctx->createTempWeights(scale_weights.type, scale_weights.shape);
  auto combined_bias_weights = ctx->createTempWeights(bias_weights.type, bias_weights.shape);
  size_t nweight = nchan;
  // Fold the weights together into a single bias and scale
  for (size_t i = 0; i < nweight; ++i)
  {
    float scale = fix_gamma ? 1.0f : (static_cast<float const*>(scale_weights.values))[i];
    float bias = (static_cast<float const*>(bias_weights.values))[i];
    float mean = (static_cast<float const*>(mean_weights.values))[i];
    float variance = (static_cast<float const*>(variance_weights.values))[i];
    float& combined_scale_ref = const_cast<float*>(static_cast<float const*>(combined_scale_weights.values))[i];
    float& combined_bias_ref = const_cast<float*>(static_cast<float const*>(combined_bias_weights.values))[i];
    combined_scale_ref = scale / sqrtf(variance + eps);
    combined_bias_ref = bias - mean * combined_scale_ref;
  }
  return addScale(ctx, tensor, nvinfer1::ScaleMode::kCHANNEL, combined_bias_weights, combined_scale_weights, {});
}

// TIPS : NO TEST
DEFINE_BUILTIN_OP_IMPORTER(ceil)
{
    return unaryHelper(ctx, node, inputs, nvinfer1::UnaryOperation::kCEIL);
}

// TODO : CAST
// DEFINE_BUILTIN_OP_IMPORTER(Cast)
//{
//    // Get input node.
//    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
//    OnnxAttrs attrs(node);
//    // Check if datatype casted to is valid.
//    nvinfer1::DataType dtype = nvinfer1::DataType::kFLOAT;
//    ASSERT(convert_dtype(attrs.get<int32_t>("to"), &dtype), ErrorCode::kUNSUPPORTED_NODE);
//    // Add the layer.
//    nvinfer1::IIdentityLayer* layer = ctx->network()->addIdentity(inputs.at(0).tensor());
//    layer->setPrecision(dtype);
//    RETURN_FIRST_OUTPUT(layer);
//}

DEFINE_BUILTIN_OP_IMPORTER(clip)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    float a_min;
    float a_max;

    AttrProcMap reqAttrProcs;

    reqAttrProcs["a_min"] = [&](const std::string& strVal) {
        auto mxnet_a_min = Str2Num<float>(strVal);
        a_min = mxnet_a_min;
    };
    reqAttrProcs["a_max"] = [&](const std::string& strVal) {
        auto mxnet_a_max = Str2Num<float>(strVal);
        a_max = mxnet_a_max;
    };

    ProcAttrs(node, reqAttrProcs, true);

    auto plug = createClipPlugin(node.strName.c_str(), a_min, a_max);
    std::vector<nvinfer1::ITensor*> const& plugin_inputs = {&inputs.at(0).tensor()};
    RETURN_FIRST_OUTPUT(ctx->network()->addPluginV2(plugin_inputs.data(), plugin_inputs.size(), *plug));

//    return activationHelper(ctx, node, inputs, nvinfer1::ActivationType::kCLIP, &a_min, &a_max);
}

DEFINE_BUILTIN_OP_IMPORTER(Concat)
{
    std::vector<nvinfer1::ITensor*> tensors;
    for (auto& input : inputs)
    {
        ASSERT(input.is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
        ASSERT(input.tensor().getType() != nvinfer1::DataType::kINT32, ErrorCode::kUNSUPPORTED_NODE);
        tensors.push_back(&input.tensor());
    }

    // parser attrs
    int dim = 1;

    AttrProcMap reqAttrProcs;

    reqAttrProcs["dim"] = [&](const std::string& strVal) {
        auto mxnet_dim = Str2Num<int>(strVal);
        dim = mxnet_dim;
    };

    ProcAttrs(node, reqAttrProcs, true);

    int nbDims = inputs.at(0).shape().nbDims;
    TRT_CHECK(convert_axis(dim, nbDims));
    auto* layer = ctx->network()->addConcatenation(tensors.data(), tensors.size());
    ASSERT(layer, ErrorCode::kUNSUPPORTED_NODE);
    layer->setAxis(dim);
    RETURN_FIRST_OUTPUT(layer);
}

DEFINE_BUILTIN_OP_IMPORTER(Convolution)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    ASSERT(inputs.at(1).is_weights(), ErrorCode::kUNSUPPORTED_NODE);

    nvinfer1::ITensor* tensor_ptr = &inputs.at(0).tensor();
    auto kernel_weights = inputs.at(1).weights();
    nvinfer1::Dims dims = tensor_ptr->getDimensions();
    bool need_to_expand_dims = (dims.nbDims == 2);
    if (need_to_expand_dims)
    {
        // Expand spatial dims from 1D to 2D
        nvinfer1::DimsCHW new_shape(dims.d[0], dims.d[1], 1);
        tensor_ptr = reshape_tensor(ctx, *tensor_ptr, new_shape);
        ASSERT(tensor_ptr, ErrorCode::kUNSUPPORTED_NODE);
        dims = tensor_ptr->getDimensions();
    }
    if (kernel_weights.shape.nbDims == 3)
    {
        kernel_weights.shape.nbDims = 4;
        kernel_weights.shape.d[3] = 1;
    }
    ASSERT(dims.nbDims >= 3, ErrorCode::kUNSUPPORTED_NODE);
    ASSERT(kernel_weights.shape.nbDims == 4, ErrorCode::kUNSUPPORTED_NODE);
    nvinfer1::Weights bias_weights{};
    if (inputs.size() == 3)
    {
        ASSERT(inputs.at(2).is_weights(), ErrorCode::kUNSUPPORTED_NODE);
        auto shaped_bias_weights = inputs.at(2).weights();
        ASSERT(shaped_bias_weights.shape.nbDims == 1, ErrorCode::kINVALID_NODE);
        ASSERT(shaped_bias_weights.shape.d[0] == kernel_weights.shape.d[0], ErrorCode::kINVALID_NODE);
        bias_weights = shaped_bias_weights;
    }
    else
    {
        bias_weights = ShapedWeights::empty(kernel_weights.type);
    }
    nvinfer1::DimsHW kernel_size;
    kernel_size.h() = kernel_weights.shape.d[2];
    kernel_size.w() = kernel_weights.shape.d[3];
    nvinfer1::DimsHW strides(1, 1);
    nvinfer1::DimsHW beg_padding(0, 0), end_padding(0, 0);
    nvinfer1::DimsHW dilations(1, 1);
    nvinfer1::PaddingMode paddingMode{nvinfer1::PaddingMode::kEXPLICIT_ROUND_DOWN};
    int nchan = dims.d[CHANNEL_DIM];
    int noutput; // Note: Weights order is KCRS
    int ngroup = 1;
    bool no_bias = false;

    // parser attrs
    AttrProcMap reqAttrProcs;
    AttrProcMap optAttrProcs;
    reqAttrProcs["kernel"] = [&](const std::string& strVal) {
        auto mxnet_kernel = Str2Vector<int>(strVal);
        int ndim = mxnet_kernel.size();
        kernel_size.h() = mxnet_kernel[0];
        kernel_size.w() = ndim > 1 ? mxnet_kernel[1] : 1;
    };
    optAttrProcs["stride"] = [&](const std::string& strVal) {
        auto mxnet_stride = Str2Vector<int>(strVal);
        int ndim = mxnet_stride.size();
        strides.h() = mxnet_stride[0];
        strides.w() = ndim > 1 ? mxnet_stride[1] : 1;
    };
    optAttrProcs["dilate"] = [&](const std::string& strVal) {
        auto mxnet_dilate = Str2Vector<int>(strVal);
        int ndim = mxnet_dilate.size();
        dilations.h() = mxnet_dilate[0];
        dilations.w() = ndim > 1 ? mxnet_dilate[1] : 1;
    };
    optAttrProcs["pad"] = [&](const std::string& strVal) {
        auto mxnet_pad = Str2Vector<int>(strVal);
        int ndim = mxnet_pad.size();
        beg_padding.h() = mxnet_pad[0];
        beg_padding.w() = ndim > 1 ? mxnet_pad[1] : 1;
        end_padding.h() = mxnet_pad[0];
        end_padding.w() = ndim > 1 ? mxnet_pad[1] : 1;
    };
    reqAttrProcs["num_filter"] = [&](const std::string& strVal) {
        int mxnet_num_filter = Str2Num<int>(strVal);
        noutput = mxnet_num_filter;
    };
    optAttrProcs["num_group"] = [&](const std::string& strVal) {
        auto mxnet_num_group = Str2Num<int>(strVal);
        ngroup = mxnet_num_group;
    };
    optAttrProcs["no_bias"] = [&](const std::string& strVal) {
        auto mxnet_no_bias = Str2Bool<bool>(strVal);
        no_bias = mxnet_no_bias;
    };

    ProcAttrs(node, reqAttrProcs, true);
    ProcAttrs(node, optAttrProcs, false);

    ASSERT(kernel_weights.shape.d[1] * ngroup == nchan, ErrorCode::kINVALID_NODE);
    ASSERT((inputs.size() == 3 && !no_bias) || no_bias, ErrorCode::kINVALID_NODE);
    ASSERT(noutput == kernel_weights.shape.d[0], ErrorCode::kINVALID_NODE);
    ASSERT(kernel_size.h() == kernel_weights.shape.d[2], ErrorCode::kINVALID_NODE);
    ASSERT(kernel_size.w() == kernel_weights.shape.d[3], ErrorCode::kINVALID_NODE);
    nvinfer1::IConvolutionLayer* layer
        = ctx->network()->addConvolution(*tensor_ptr, noutput, kernel_size, kernel_weights, bias_weights);

    ASSERT(layer, ErrorCode::kUNSUPPORTED_NODE);
    layer->setStride(strides);
    layer->setPaddingMode(paddingMode);
    layer->setPrePadding(beg_padding);
    layer->setPostPadding(end_padding);
    layer->setDilation(dilations);
    layer->setNbGroups(ngroup);
    tensor_ptr = layer->getOutput(0);
    dims = tensor_ptr->getDimensions();
    if (need_to_expand_dims)
    {
        // Un-expand spatial dims back to 1D
        nvinfer1::Dims new_shape{2, {dims.d[0], dims.d[1]}};
        tensor_ptr = reshape_tensor(ctx, *tensor_ptr, new_shape);
        ASSERT(tensor_ptr, ErrorCode::kUNSUPPORTED_NODE);
    }
    return {{tensor_ptr}};
}

// TIPS : NO TEST
DEFINE_BUILTIN_OP_IMPORTER(cos)
{
    return unaryHelper(ctx, node, inputs, nvinfer1::UnaryOperation::kCOS);
}

// TIPS : NO TEST
DEFINE_BUILTIN_OP_IMPORTER(cosh)
{
    return unaryHelper(ctx, node, inputs, nvinfer1::UnaryOperation::kCOSH);
}

// TIPS : NO TEST
DEFINE_BUILTIN_OP_IMPORTER(depth_to_space)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    nvinfer1::ITensor* tensor_ptr = &inputs.at(0).tensor();
    nvinfer1::IShuffleLayer* layer = ctx->network()->addShuffle(*tensor_ptr);
    ASSERT(layer, ErrorCode::kUNSUPPORTED_NODE);
    // parser attrs
    int block_size = 0;

    AttrProcMap reqAttrProcs;
    reqAttrProcs["block_size"] = [&](const std::string& strVal) {
        auto mxnet_block_size = Str2Num<int>(strVal);
        block_size = mxnet_block_size;
    };

    ProcAttrs(node, reqAttrProcs, true);

    nvinfer1::Dims dims = tensor_ptr->getDimensions();
    int ndim_spatial = dims.nbDims - 1;
    nvinfer1::Dims new_shape1;
    new_shape1.nbDims = dims.nbDims + ndim_spatial;
    new_shape1.d[ndim_spatial] = dims.d[0];
    for (int i = 0; i < ndim_spatial; ++i)
    {
        ASSERT(new_shape1.d[ndim_spatial] % block_size == 0, ErrorCode::kINVALID_NODE);
        new_shape1.d[ndim_spatial] /= block_size;
        new_shape1.d[i] = block_size;
        new_shape1.d[ndim_spatial + 1 + i] = dims.d[1 + i];
    }
    layer->setReshapeDimensions(new_shape1);
    nvinfer1::Permutation perm;
    perm.order[0] = ndim_spatial;
    for (int i = 0; i < ndim_spatial; ++i)
    {
        perm.order[1 + 2 * i + 0] = ndim_spatial + 1 + i;
        perm.order[1 + 2 * i + 1] = i;
    }
    layer->setSecondTranspose(perm);
    tensor_ptr = layer->getOutput(0);
    dims = tensor_ptr->getDimensions();
    nvinfer1::Dims new_shape2;
    new_shape2.nbDims = dims.nbDims - ndim_spatial;
    new_shape2.d[0] = dims.d[0];
    for (int i = 0; i < ndim_spatial; ++i)
    {
        new_shape2.d[1 + i] = dims.d[1 + 2 * i + 0] * dims.d[1 + 2 * i + 1];
    }
    tensor_ptr = reshape_tensor(ctx, *tensor_ptr, new_shape2);
    ASSERT(tensor_ptr, ErrorCode::kUNSUPPORTED_NODE);
    return {{tensor_ptr}};
}

DEFINE_BUILTIN_OP_IMPORTER(broadcast_mul)
{
    ASSERT(inputs.size() == 2, ErrorCode::kINVALID_NODE);
    return combineTensorsElementwise(ctx, node, inputs, nvinfer1::ElementWiseOperation::kPROD, true);
}

DEFINE_BUILTIN_OP_IMPORTER(elemwise_mul)
{
    ASSERT(inputs.size() == 2, ErrorCode::kINVALID_NODE);
    return combineTensorsElementwise(ctx, node, inputs, nvinfer1::ElementWiseOperation::kPROD, true);
}

DEFINE_BUILTIN_OP_IMPORTER(_copy)
{
  RETURN_IDENTITY(inputs.at(0));
}

DEFINE_BUILTIN_OP_IMPORTER(Dropout)
{
    RETURN_IDENTITY(inputs.at(0));
}

DEFINE_BUILTIN_OP_IMPORTER(MAERegressionOutput)
{
    RETURN_IDENTITY(inputs.at(0));
}

DEFINE_BUILTIN_OP_IMPORTER(Activation)
{
    std::string strVal = node.attrs.GetValue("act_type", true);
    nvinfer1::ActivationType act_type;
    if (strVal == "relu")
    {
        act_type = nvinfer1::ActivationType::kRELU;
    }
    else if (strVal == "sigmoid")
    {
        act_type = nvinfer1::ActivationType::kSIGMOID;
    }
    else if (strVal == "softrelu")
    {
        act_type = nvinfer1::ActivationType::kSOFTPLUS;
    }
    else if (strVal == "softsign")
    {
        act_type = nvinfer1::ActivationType::kSOFTSIGN;
    }
    else if (strVal == "tanh")
    {
        act_type = nvinfer1::ActivationType::kTANH;
    }
    else
    {
        return MAKE_ERROR("Assertion failed: Not impelemnet yet.", ErrorCode::kINVALID_NODE);
    }
    return activationHelper(ctx, node, inputs, act_type);
}

DEFINE_BUILTIN_OP_IMPORTER(Embedding)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);

    nvinfer1::ITensor& tensor = inputs.at(0).tensor();
    nvinfer1::ITensor& weight = convertToTensor(inputs.at(1), ctx);

    int input_dim = 0;
    int output_dim = 0;
    bool sparse_grad = false;

    // parser attrs
    AttrProcMap reqAttrProcs;

    reqAttrProcs["input_dim"] = [&](const std::string& strVal) {
        auto mxnet_input_dim = Str2Num<int>(strVal);
        input_dim = mxnet_input_dim;
    };
    reqAttrProcs["output_dim"] = [&](const std::string& strVal) {
        auto mxnet_output_dim = Str2Num<int>(strVal);
        output_dim = mxnet_output_dim;
    };
    reqAttrProcs["sparse_grad"] = [&](const std::string& strVal) {
        auto mxnet_sparse_grad = Str2Bool<bool>(strVal);
        sparse_grad = mxnet_sparse_grad;
    };

    ProcAttrs(node, reqAttrProcs, true);

    ASSERT(!sparse_grad, ErrorCode::kUNSUPPORTED_NODE);

    auto plug = arctern::trtPlugin::createEmbeddingPlugin(node.strName.c_str(), input_dim, output_dim, sparse_grad);
    std::vector<nvinfer1::ITensor*> const& plugin_inputs = {&inputs.at(0).tensor(), &weight};
    RETURN_FIRST_OUTPUT(ctx->network()->addPluginV2(plugin_inputs.data(), plugin_inputs.size(), *plug));
}

DEFINE_BUILTIN_OP_IMPORTER(exp)
{
    return apply_unary_function(ctx, inputs.at(0), nvinfer1::UnaryOperation::kEXP);
}

DEFINE_BUILTIN_OP_IMPORTER(Flatten)
{
    // Note: Flattening to shape=[batch, n] is currently the only sensible
    //       operation, because we can't remove or merge into the batch dim.
    nvinfer1::Dims dims = inputs.at(0).shape();
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    nvinfer1::ITensor* tensor_ptr;
    nvinfer1::Dims new_shape{2, {dims.d[0], (int) get_shape_size(remove_dim(dims, 0))}};
    tensor_ptr = reshape_tensor(ctx, inputs.at(0).tensor(), new_shape);
    ASSERT(tensor_ptr, ErrorCode::kUNSUPPORTED_NODE);
    return {{tensor_ptr}};
}

// TODO : gather_nd
// DEFINE_BUILTIN_OP_IMPORTER(gather_nd)
//{
//    nvinfer1::ITensor& data = convertToTensor(inputs.at(0), ctx);
//    nvinfer1::ITensor& indices = convertToTensor(inputs.at(1), ctx);
//    int axis = 0;
//    int nbDims = inputs.at(0).shape().nbDims;
//    TRT_CHECK(convert_axis(axis, nbDims));
//    RETURN_FIRST_OUTPUT(ctx->network()->addGather(data, indices, axis));
//}

DEFINE_BUILTIN_OP_IMPORTER(floor)
{
    return unaryHelper(ctx, node, inputs, nvinfer1::UnaryOperation::kFLOOR);
}

DEFINE_BUILTIN_OP_IMPORTER(FullyConnected)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    ASSERT(inputs.at(1).is_weights(), ErrorCode::kUNSUPPORTED_NODE);

    int num_hidden;
    bool no_bias = false;
    bool flatten = true;

    // parser attrs
    AttrProcMap reqAttrProcs;
    AttrProcMap optAttrProcs;
    reqAttrProcs["num_hidden"] = [&](const std::string& strVal) {
        auto mxnet_num_hidden = Str2Num<int>(strVal);
        num_hidden = mxnet_num_hidden;
    };
    optAttrProcs["no_bias"] = [&](const std::string& strVal) {
        auto mxnet_no_bias = Str2Bool<bool>(strVal);
        no_bias = mxnet_no_bias;
    };
    optAttrProcs["flatten"] = [&](const std::string& strVal) {
        auto mxnet_flatten = Str2Bool<bool>(strVal);
        flatten = mxnet_flatten;
    };

    ProcAttrs(node, reqAttrProcs, true);
    ProcAttrs(node, optAttrProcs, false);

    nvinfer1::ITensor* tensor_ptr = &inputs.at(0).tensor();
    nvinfer1::Dims dims = tensor_ptr->getDimensions();
    static int cnt = 0;
    if (flatten)
    {
        bool need_to_expand_dims = (tensor_ptr->getDimensions().nbDims < 3);
        if (need_to_expand_dims)
        {
            nvinfer1::Dims new_shape{3, {(int) get_shape_size(dims), 1, 1}};
            tensor_ptr = reshape_tensor(ctx, *tensor_ptr, new_shape);
            ASSERT(tensor_ptr, ErrorCode::kUNSUPPORTED_NODE);
            /// set reshpae_tensor name
            std::string name = "stn_loc_reshape" + std::to_string(cnt++); /// <zy>:solved face_attributes model node name conflict!
            tensor_ptr->setName(name.c_str());
        }
    }
    else
    {
        ASSERT(dims.nbDims <= 6, ErrorCode::kUNSUPPORTED_NODE);
        nvinfer1::Dims new_shape = dims;
        while (new_shape.nbDims < dims.nbDims + 2)
        {
            new_shape.d[new_shape.nbDims++] = 1;
        }
        tensor_ptr = reshape_tensor(ctx, *tensor_ptr, new_shape);
        ASSERT(tensor_ptr, ErrorCode::kUNSUPPORTED_NODE);
        /// set reshpae_tensor name
        std::string name = "stn_loc_reshape" + std::to_string(cnt++);
        tensor_ptr->setName(name.c_str());
    }

    ShapedWeights weights = inputs.at(1).weights();
    ASSERT(weights.shape.d[0] == num_hidden, ErrorCode::kUNSUPPORTED_NODE);
    ShapedWeights biases;
    if (no_bias)
    {
        biases = ShapedWeights::empty(tensor_ptr->getType());
    }
    else
    {
        ASSERT(inputs.at(2).is_weights(), ErrorCode::kUNSUPPORTED_NODE);
        auto shaped_bias_weights = inputs.at(2).weights();
        ASSERT(shaped_bias_weights.shape.nbDims == 1, ErrorCode::kINVALID_NODE);
        ASSERT(shaped_bias_weights.shape.d[0] == weights.shape.d[0], ErrorCode::kINVALID_NODE);
        biases = shaped_bias_weights;
    }
    nvinfer1::IFullyConnectedLayer* layer = ctx->network()->addFullyConnected(*tensor_ptr, num_hidden, weights, biases);
    tensor_ptr = layer->getOutput(0);
    tensor_ptr->setName(std::string(node.strName + "_fc").c_str());

    if (flatten)
    {
        nvinfer1::Dims new_shape{2, {1, num_hidden}};
        tensor_ptr = reshape_tensor(ctx, *tensor_ptr, new_shape);
        ASSERT(tensor_ptr, ErrorCode::kUNSUPPORTED_NODE);
    }
    else
    {
        nvinfer1::Dims new_shape = tensor_ptr->getDimensions();
        new_shape.nbDims -= 2;
        tensor_ptr = reshape_tensor(ctx, *tensor_ptr, new_shape);
        ASSERT(tensor_ptr, ErrorCode::kUNSUPPORTED_NODE);
    }
    return {{tensor_ptr}};
}

///std::string split implementation by using delimeter as an another string
std::vector<std::string> split(std::string stringToBeSplitted, std::string delimeter)
{
    std::vector<std::string> splittedString;
    int startIndex = 0;
    int  endIndex = 0;
    while( (endIndex = stringToBeSplitted.find(delimeter, startIndex)) < stringToBeSplitted.size() )
    {
        std::string val = stringToBeSplitted.substr(startIndex, endIndex - startIndex);
        splittedString.push_back(val);
        startIndex = endIndex + delimeter.size();
    }
    if(startIndex < stringToBeSplitted.size())
    {
        std::string val = stringToBeSplitted.substr(startIndex);
        splittedString.push_back(val);
    }
    return splittedString;
}

DEFINE_BUILTIN_OP_IMPORTER(FullyConnectedReLUFusion)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    ASSERT(inputs.at(1).is_weights(), ErrorCode::kUNSUPPORTED_NODE);

    int num_hidden;
    bool no_bias = false;
    bool flatten = true;

    // parser attrs
    AttrProcMap reqAttrProcs;
    AttrProcMap optAttrProcs;
    reqAttrProcs["num_hidden"] = [&](const std::string& strVal) {
        auto mxnet_num_hidden = Str2Num<int>(strVal);
        num_hidden = mxnet_num_hidden;
    };
    optAttrProcs["no_bias"] = [&](const std::string& strVal) {
        auto mxnet_no_bias = Str2Bool<bool>(strVal);
        no_bias = mxnet_no_bias;
    };
    optAttrProcs["flatten"] = [&](const std::string& strVal) {
        auto mxnet_flatten = Str2Bool<bool>(strVal);
        flatten = mxnet_flatten;
    };

    ProcAttrs(node, reqAttrProcs, true);
    ProcAttrs(node, optAttrProcs, false);
    std::vector<std::string> vSplitedStr = split(node.strName, "_fuse_");

    ASSERT(flatten, ErrorCode::kUNSUPPORTED_NODE);
    nvinfer1::ITensor* tensor_ptr = &inputs.at(0).tensor();
    nvinfer1::Dims dims = tensor_ptr->getDimensions();
    bool need_to_expand_dims = (tensor_ptr->getDimensions().nbDims < 3);
    if (need_to_expand_dims)
    {
        nvinfer1::Dims new_shape{3, {(int) get_shape_size(dims), 1, 1}};
        tensor_ptr = reshape_tensor(ctx, *tensor_ptr, new_shape);
        ASSERT(tensor_ptr, ErrorCode::kUNSUPPORTED_NODE);
    }

    ShapedWeights weights = inputs.at(1).weights();
    ASSERT(weights.shape.d[0] == num_hidden, ErrorCode::kUNSUPPORTED_NODE);
    ShapedWeights biases;
    if (no_bias)
    {
        biases = ShapedWeights::empty(tensor_ptr->getType());
    }
    else
    {
        ASSERT(inputs.at(2).is_weights(), ErrorCode::kUNSUPPORTED_NODE);
        auto shaped_bias_weights = inputs.at(2).weights();
        ASSERT(shaped_bias_weights.shape.nbDims == 1, ErrorCode::kINVALID_NODE);
        ASSERT(shaped_bias_weights.shape.d[0] == weights.shape.d[0], ErrorCode::kINVALID_NODE);
        biases = shaped_bias_weights;
    }
    nvinfer1::IFullyConnectedLayer* fc_layer
        = ctx->network()->addFullyConnected(*tensor_ptr, num_hidden, weights, biases);
    ASSERT(fc_layer, ErrorCode::kUNSUPPORTED_NODE);
    tensor_ptr = fc_layer->getOutput(0);
    tensor_ptr->setName(vSplitedStr[0].c_str());

    nvinfer1::IActivationLayer* relu_layer
        = ctx->network()->addActivation(*tensor_ptr, nvinfer1::ActivationType::kRELU);
    ASSERT(relu_layer, ErrorCode::kUNSUPPORTED_NODE);
    tensor_ptr = relu_layer->getOutput(0);
    tensor_ptr->setName(vSplitedStr[1].c_str());

    nvinfer1::Dims new_shape{2, {1, num_hidden}};
    tensor_ptr = reshape_tensor(ctx, *tensor_ptr, new_shape);
    ASSERT(tensor_ptr, ErrorCode::kUNSUPPORTED_NODE);
    return {{tensor_ptr}};
}

DEFINE_BUILTIN_OP_IMPORTER(Pooling)
{
    nvinfer1::DimsHW kernel_size;
    nvinfer1::DimsHW stride;
    nvinfer1::DimsHW beg_padding;
    nvinfer1::DimsHW end_padding;
    nvinfer1::PaddingMode paddingMode{nvinfer1::PaddingMode::kEXPLICIT_ROUND_DOWN};
    nvinfer1::PoolingType pool_type{nvinfer1::PoolingType::kMAX};
    bool global_pool = false;
    std::string pooling_convention{"valid"};
    std::string pooling_type{"max"};
    int p_value = 0;
    bool count_include_pad = true;

    // parser attrs
    AttrProcMap optAttrProcs;
    optAttrProcs["kernel"] = [&](const std::string& strVal) {
        auto mxnet_kernel = Str2Vector<int>(strVal);
        int ndim = mxnet_kernel.size();
        kernel_size.h() = mxnet_kernel[0];
        kernel_size.w() = ndim > 1 ? mxnet_kernel[1] : 1;
    };
    optAttrProcs["pool_type"] = [&](const std::string& strVal) { pooling_type = strVal; };
    optAttrProcs["pooling_convention"] = [&](const std::string& strVal) { pooling_convention = strVal; };
    optAttrProcs["global_pool"] = [&](const std::string& strVal) {
        auto mxnet_global_pool = Str2Bool<bool>(strVal);
        global_pool = mxnet_global_pool;
    };
    optAttrProcs["stride"] = [&](const std::string& strVal) {
        auto mxnet_stride = Str2Vector<int>(strVal);
        int ndim = mxnet_stride.size();
        stride.h() = mxnet_stride[0];
        stride.w() = ndim > 1 ? mxnet_stride[1] : 1;
    };
    optAttrProcs["pad"] = [&](const std::string& strVal) {
        auto mxnet_pad = Str2Vector<int>(strVal);
        int ndim = mxnet_pad.size();
        beg_padding.h() = mxnet_pad[0];
        beg_padding.w() = ndim > 1 ? mxnet_pad[1] : 1;
        end_padding.h() = mxnet_pad[0];
        end_padding.w() = ndim > 1 ? mxnet_pad[1] : 1;
    };
    optAttrProcs["p_value"] = [&](const std::string& strVal) {
        auto mxnet_p_value = Str2Num<int>(strVal);
        p_value = mxnet_p_value;
    };
    optAttrProcs["count_include_pad"] = [&](const std::string& strVal) {
        auto mxnet_count_include_pad = Str2Bool<bool>(strVal);
        count_include_pad = mxnet_count_include_pad;
    };
    ProcAttrs(node, optAttrProcs, false);

    if (pooling_type == "max")
    {
        pool_type = nvinfer1::PoolingType::kMAX;
    }
    else if (pooling_type == "avg")
    {
        pool_type = nvinfer1::PoolingType::kAVERAGE;
    }
    else
    {
        return MAKE_ERROR("Assertion failed: Not impelemnet yet.", ErrorCode::kINVALID_NODE);
    }

    if (pooling_convention == "valid")
    {
        paddingMode = nvinfer1::PaddingMode::kEXPLICIT_ROUND_DOWN;
    }
    else if (pooling_convention == "full")
    {
        paddingMode = nvinfer1::PaddingMode::kCAFFE_ROUND_UP;
    }
    else
    {
        return MAKE_ERROR("Assertion failed: Not impelemnet yet.", ErrorCode::kINVALID_NODE);
    }

    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    nvinfer1::ITensor* tensor_ptr = &inputs.at(0).tensor();
    nvinfer1::Dims dims = tensor_ptr->getDimensions();

    if (global_pool)
    {
        ASSERT(dims.nbDims == 4, ErrorCode::kUNSUPPORTED_NODE);
        kernel_size.h() = dims.d[2];
        kernel_size.w() = dims.d[3];
        RETURN_FIRST_OUTPUT(ctx->network()->addPooling(*tensor_ptr, pool_type, kernel_size));
    }
    else
    {
        // max pooling
        ASSERT(dims.nbDims >= 3, ErrorCode::kINVALID_NODE);
        bool need_to_expand_dims = (dims.nbDims == 3);
        if (need_to_expand_dims)
        {
            // Expand spatial dims from 1D to 2D
            nvinfer1::DimsNCHW new_shape(dims.d[0], dims.d[1], dims.d[2], 1);
            tensor_ptr = reshape_tensor(ctx, *tensor_ptr, new_shape);
            ASSERT(tensor_ptr, ErrorCode::kUNSUPPORTED_NODE);
            dims = tensor_ptr->getDimensions();
        }
        ASSERT(dims.nbDims == 4, ErrorCode::kUNSUPPORTED_NODE);
        nvinfer1::IPoolingLayer* layer = ctx->network()->addPooling(*tensor_ptr, pool_type, kernel_size);
        ASSERT(layer, ErrorCode::kUNSUPPORTED_NODE);
        layer->setStride(stride);
        layer->setAverageCountExcludesPadding(!count_include_pad);
        layer->setPaddingMode(paddingMode);
        layer->setPrePadding(beg_padding);
        layer->setPostPadding(end_padding);
        tensor_ptr = layer->getOutput(0);
        dims = tensor_ptr->getDimensions();
        if (need_to_expand_dims)
        {
            // Un-expand spatial dims back to 1D
            nvinfer1::Dims new_shape{3, {dims.d[0], dims.d[1], dims.d[2]}};
            tensor_ptr = reshape_tensor(ctx, *tensor_ptr, new_shape);
            ASSERT(tensor_ptr, ErrorCode::kUNSUPPORTED_NODE);
        }
        return {{tensor_ptr}};
    }
}

DEFINE_BUILTIN_OP_IMPORTER(hard_sigmoid)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    float alpha = 0.2f;
    float beta = 0.5f;

    AttrProcMap optAttrProcs;
    optAttrProcs["alpha"] = [&](const std::string& strVal) {
        auto mxnet_alpha = Str2Num<float>(strVal);
        alpha = mxnet_alpha;
    };
    optAttrProcs["beta"] = [&](const std::string& strVal) {
        auto mxnet_beta = Str2Num<float>(strVal);
        beta = mxnet_beta;
    };
    ProcAttrs(node, optAttrProcs, false);

    return activationHelper(ctx, node, inputs, nvinfer1::ActivationType::kHARD_SIGMOID, &alpha, &beta);
}

DEFINE_BUILTIN_OP_IMPORTER(identity)
{
    RETURN_IDENTITY(inputs.at(0));
}

DEFINE_BUILTIN_OP_IMPORTER(L2Normalization)
{
    float eps = 1e-10;
    std::string mode{"instance"};

    AttrProcMap reqAttrProcs;
    AttrProcMap optAttrProcs;
    reqAttrProcs["eps"] = [&](const std::string& strVal) {
        auto mxnet_eps = Str2Num<float>(strVal);
        eps = mxnet_eps;
    };
    optAttrProcs["mode"] = [&](const std::string& strVal) { mode = strVal; };

    ProcAttrs(node, reqAttrProcs, true);
    ProcAttrs(node, optAttrProcs, false);

    ASSERT(mode == "instance", ErrorCode::kUNSUPPORTED_NODE);

    if (inputs.at(0).is_tensor())
    {
        auto plug = arctern::trtPlugin::createL2NormalizationPlugin(node.strName.c_str(), eps);
        std::vector<nvinfer1::ITensor*> const& plugin_inputs = {&inputs.at(0).tensor()};
        RETURN_FIRST_OUTPUT(ctx->network()->addPluginV2(plugin_inputs.data(), plugin_inputs.size(), *plug));
    }
    else
    {
        ShapedWeights& weight = inputs.at(0).weights();
        nvinfer1::Dims shape = weight.shape;
        int ndim = weight.shape.nbDims;
        int nData = 1;
        for (int j = 1; j < ndim; j++)
        {
            nData *= shape.d[j];
        }

        auto* data = (float*) weight.values;
        for (int i = 0; i < shape.d[0]; i++)
        {
            float sum = 0.0f;
            for (int j = 0; j < nData; j++)
            {
                sum += (data[j] * data[j]);
            }
            sum = sqrtf(sum);

            for (int j = 0; j < nData; j++)
            {
                data[j] /= sum;
            }
            data += nData;
        }
        return {{weight}};
    }
}

DEFINE_BUILTIN_OP_IMPORTER(LeakyReLU)
{
    std::string act_type = "leaky"; //{'elu', 'leaky', 'prelu', 'rrelu', 'selu'}
    float slope = 0.25;
    float lower_bound = 0.125;
    float upper_bound = 0.334;

    // parser attrs
    AttrProcMap optAttrProcs;
    optAttrProcs["act_type"] = [&](const std::string& strVal) { act_type = strVal; };
    optAttrProcs["slope"] = [&](const std::string& strVal) {
        auto mxnet_slope = Str2Num<float>(strVal);
        slope = mxnet_slope;
    };
    optAttrProcs["lower_bound"] = [&](const std::string& strVal) {
        auto mxnet_lower_bound = Str2Num<float>(strVal);
        lower_bound = mxnet_lower_bound;
    };
    optAttrProcs["upper_bound"] = [&](const std::string& strVal) {
        auto mxnet_upper_bound = Str2Num<float>(strVal);
        upper_bound = mxnet_upper_bound;
    };

    ProcAttrs(node, optAttrProcs, false);

    if (act_type == "leaky")
    {
        auto plug = createLReLUPlugin(slope);
        std::vector<nvinfer1::ITensor*> const& plugin_inputs = {&inputs.at(0).tensor()};
        RETURN_FIRST_OUTPUT(ctx->network()->addPluginV2(plugin_inputs.data(), plugin_inputs.size(), *plug));
    }
    else if (act_type == "elu")
    {
        return activationHelper(ctx, node, inputs, nvinfer1::ActivationType::kELU, &slope);
    }
    else if (act_type == "selu")
    {
        float alpha = 1.6732f;
        float beta = 1.0507f;
        return activationHelper(ctx, node, inputs, nvinfer1::ActivationType::kSELU, &alpha, &beta);
    }
    else if (act_type == "prelu")
    {
        ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
        ASSERT(inputs.at(1).is_weights(), ErrorCode::kUNSUPPORTED_NODE);
        ShapedWeights weights = inputs.at(1).weights();
        ASSERT(weights.type == nvinfer1::DataType::kFLOAT, ErrorCode::kUNSUPPORTED_NODE);

        std::vector<float> alpha;
        alpha.assign((float*) weights.values, (float*) (weights.values) + weights.count());

        auto plug = arctern::trtPlugin::createPReLUPlugin(node.strName.c_str(), alpha);
        std::vector<nvinfer1::ITensor*> const& plugin_inputs = {&inputs.at(0).tensor()};
        RETURN_FIRST_OUTPUT(ctx->network()->addPluginV2(plugin_inputs.data(), plugin_inputs.size(), *plug));
    }
    else
    {
        return MAKE_ERROR("Assertion failed: Not impelemnet yet.", ErrorCode::kINVALID_NODE);
    }
}

DEFINE_BUILTIN_OP_IMPORTER(log)
{
    return apply_unary_function(ctx, inputs.at(0), nvinfer1::UnaryOperation::kLOG);
}

// TODO : log_softmax
// DEFINE_BUILTIN_OP_IMPORTER(log_softmax)
//{
//    auto result = importSoftmax(ctx, node, inputs);
//    if (result.is_error())
//    {
//        return result;
//    }
//    else
//    {
//        auto& input = result.value().at(0);
//        return apply_unary_function(ctx, input, nvinfer1::UnaryOperation::kLOG);
//    }
//}

// TODO : LRN
// DEFINE_BUILTIN_OP_IMPORTER(LRN) {
//  ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
//  nvinfer1::ITensor& tensor = inputs.at(0).tensor();
//  OnnxAttrs attrs(node);
//  int   size  = attrs.get<int>("size");
//  float alpha = attrs.get<float>("alpha", 0.0001f);
//  float beta  = attrs.get<float>("beta", 0.75f);
//  float bias  = attrs.get<float>("bias", 1.0f);
//  RETURN_FIRST_OUTPUT(
//    ctx->network()->addLRN(tensor, size, alpha, beta, bias));
//}

DEFINE_BUILTIN_OP_IMPORTER(max)
{
    return reduceTensor(ctx, node, inputs.at(0), nvinfer1::ReduceOperation::kMAX);
}

// TIPS : NO TEST
DEFINE_BUILTIN_OP_IMPORTER(Mean)
{
    auto sum_result = combineTensorsElementwise(ctx, node, inputs, nvinfer1::ElementWiseOperation::kSUM);
    if (sum_result.is_error())
    {
        return sum_result;
    }
    auto& sum_input = sum_result.value().at(0);
    ASSERT(sum_input.is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    nvinfer1::ITensor& sum_tensor = sum_input.tensor();

    int ndim = sum_tensor.getDimensions().nbDims;
    float scale_value = 1.f / inputs.size();
    auto scale_dtype = nvinfer1::DataType::kFLOAT;
    auto scale_shape = nvinfer1::Dims{ndim, {1, 1, 1, 1, 1, 1, 1, 1}};
    auto scale_weights = ctx->createTempWeights(scale_dtype, scale_shape);
    static_cast<float*>(scale_weights.values)[0] = scale_value;
    auto* constant_layer = ctx->network()->addConstant(scale_weights.shape, scale_weights);
    ASSERT(constant_layer, ErrorCode::kUNSUPPORTED_NODE);
    nvinfer1::ITensor& scale_constant = *constant_layer->getOutput(0);
    RETURN_FIRST_OUTPUT(
        ctx->network()->addElementWise(sum_tensor, scale_constant, nvinfer1::ElementWiseOperation::kPROD));
}

DEFINE_BUILTIN_OP_IMPORTER(min)
{
    return reduceTensor(ctx, node, inputs.at(0), nvinfer1::ReduceOperation::kMIN);
}

DEFINE_BUILTIN_OP_IMPORTER(_full)
{
    std::vector<int> shape;
    float val;

    AttrProcMap reqAttrProcs;
    reqAttrProcs["shape"] = [&](const std::string& strVal) {
        auto mxnet_shape = Str2Vector<int>(strVal);
        shape = mxnet_shape;
    };
    reqAttrProcs["val"] = [&](const std::string& strVal) {
        auto mxnet_val = Str2Num<float>(strVal);
        val = mxnet_val;
    };
    ProcAttrs(node, reqAttrProcs, true);

    nvinfer1::Dims dims{};
    dims.nbDims = shape.size();
    for (size_t i = 0; i < shape.size(); i++)
    {
        dims.d[i] = shape[i];
    }

    ShapedWeights scaleWeights = ctx->createTempWeights(nvinfer1::DataType::kFLOAT, dims);
    std::fill(
        static_cast<float*>(scaleWeights.values), static_cast<float*>(scaleWeights.values) + scaleWeights.count(), val);
    RETURN_FIRST_OUTPUT(ctx->network()->addConstant(scaleWeights.shape, scaleWeights));
}

DEFINE_BUILTIN_OP_IMPORTER(_mul_scalar)
{
  auto input = inputs.at(0);
  if(input.is_tensor()){
    nvinfer1::ITensor& tensor = inputs.at(0).tensor();
    float scale = 1.0f;
    AttrProcMap reqAttrProcs;
    reqAttrProcs["scalar"] = [&](const std::string& strVal) {
      auto mxnet_scale = Str2Num<float>(strVal);
      scale = mxnet_scale;
    };
    ProcAttrs(node, reqAttrProcs, true);

    nvinfer1::Dims shape{1, {1}};
    auto scale_weight = ctx->createTempWeights(nvinfer1::DataType::kFLOAT, shape);
    float& scale_weight_ref = const_cast<float*>(static_cast<float const*>(scale_weight.values))[0];
    scale_weight_ref = scale;
    return addScale(ctx, tensor, nvinfer1::ScaleMode::kUNIFORM, {}, scale_weight, {});
  }else{
    ASSERT(input.is_weights(), ErrorCode::kINVALID_VALUE);
    ShapedWeights& weight = input.weights();
    nvinfer1::ILayer* layer_ptr = ctx->network()->addConstant(weight.shape, weight);
    ASSERT(layer_ptr, ErrorCode::kUNSUPPORTED_NODE);
    nvinfer1::ITensor *tensor = layer_ptr->getOutput(0);
    float scale = 1.0f;
    AttrProcMap reqAttrProcs;
    reqAttrProcs["scalar"] = [&](const std::string& strVal) {
      auto mxnet_scale = Str2Num<float>(strVal);
      scale = mxnet_scale;
    };
    ProcAttrs(node, reqAttrProcs, true);

    nvinfer1::Dims shape{1, {1}};
    auto scale_weight = ctx->createTempWeights(nvinfer1::DataType::kFLOAT, shape);
    float& scale_weight_ref = const_cast<float*>(static_cast<float const*>(scale_weight.values))[0];
    scale_weight_ref = scale;
    return addScale(ctx, *tensor, nvinfer1::ScaleMode::kUNIFORM, {}, scale_weight, {});
  }

}

DEFINE_BUILTIN_OP_IMPORTER(broadcast_power)
{
  ASSERT(inputs.size() == 2, ErrorCode::kINVALID_NODE);
  return combineTensorsElementwise(ctx, node, inputs, nvinfer1::ElementWiseOperation::kPOW, true);
}

DEFINE_BUILTIN_OP_IMPORTER(mean)
{
  auto input = inputs.at(0);
  auto operation = nvinfer1::ReduceOperation::kAVG;
  ASSERT(input.is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
  nvinfer1::ITensor& tensor = input.tensor();
  int ndim = tensor.getDimensions().nbDims;

  std::vector<int> axis;
  bool keepdims = false;

  AttrProcMap optAttrProcs;
  optAttrProcs["axis"] = [&](const std::string& strVal) {
    auto pos = strVal.find_first_of(',');
    auto substr = strVal.substr(1, pos-1);
    auto mxnet_axis = Str2Num<int>(substr);
    axis.emplace_back(mxnet_axis);
  };
  optAttrProcs["keepdims"] = [&](const std::string& strVal) {
    auto mxnet_keepdims = Str2Bool<bool>(strVal);
    keepdims = mxnet_keepdims;
  };

  ProcAttrs(node, optAttrProcs, false);

  ASSERT(axis.size() == 1, ErrorCode::kUNSUPPORTED_NODE);
  uint32_t axis_mask = 1 << (axis[0] >= 0 ? axis[0] : axis[0] + ndim);
  RETURN_FIRST_OUTPUT(ctx->network()->addReduce(tensor, operation, axis_mask, keepdims));
}
DEFINE_BUILTIN_OP_IMPORTER(_linalg_gemm2){
  auto input0 = inputs.at(0);
  ASSERT(input0.is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
  nvinfer1::ITensor& tensor0 = input0.tensor();

  auto input1 = inputs.at(1);
  nvinfer1::ITensor *tensor1 = nullptr;
  if(input1.is_tensor()){
    tensor1 = &input1.tensor();
  }else{
    ASSERT(input1.is_weights(), ErrorCode::kUNSUPPORTED_NODE);
    ShapedWeights& weight = input1.weights();
    nvinfer1::ILayer* layer_ptr = ctx->network()->addConstant(weight.shape, weight);
    ASSERT(layer_ptr, ErrorCode::kUNSUPPORTED_NODE);
    tensor1 = layer_ptr->getOutput(0);
  }
  ASSERT(tensor1 != nullptr, ErrorCode::kINVALID_VALUE);


  bool transpose_0, transpose_1;
  float alpha;
  int axis;
  //set default value
  transpose_0 = transpose_1 = false;
  alpha = 1.0f;
  axis = -2;

  AttrProcMap optAttrProcs;
  optAttrProcs["alpha"] = [&](const std::string& strVal){
    alpha = Str2Num<float>(strVal);
  };
  optAttrProcs["transpose_a"] = [&](const std::string& strVal){
    transpose_0 = Str2Bool<bool>(strVal);
  };
  optAttrProcs["transpose_b"] = [&](const std::string& strVal){
    transpose_1 = Str2Bool<bool>(strVal);
  };
  optAttrProcs["axis"] = [&](const std::string& strVal){
    axis = Str2Num<int>(strVal);
  };

  ProcAttrs(node, optAttrProcs, false);

  ASSERT(axis == -2, ErrorCode::kINVALID_VALUE);
  ASSERT(alpha == 1.0f, ErrorCode::kINVALID_VALUE);

  auto op0 = (transpose_0) ? nvinfer1::MatrixOperation::kTRANSPOSE : nvinfer1::MatrixOperation::kNONE;
  auto op1 = (transpose_1) ? nvinfer1::MatrixOperation::kTRANSPOSE : nvinfer1::MatrixOperation::kNONE;

  RETURN_FIRST_OUTPUT(ctx->network()->addMatrixMultiply(tensor0, op0, *tensor1, op1));
}

DEFINE_BUILTIN_OP_IMPORTER(erf) {
  ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);

  auto plug = arctern::trtPlugin::createErfPlugin(node.strName.c_str());
  std::vector<nvinfer1::ITensor *> const &plugin_inputs = {&inputs.at(0).tensor()};
  RETURN_FIRST_OUTPUT(ctx->network()->addPluginV2(plugin_inputs.data(), plugin_inputs.size(), *plug));
}


DEFINE_BUILTIN_OP_IMPORTER(Pad)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    nvinfer1::ITensor& tensor = inputs.at(0).tensor();
    int ndim = tensor.getDimensions().nbDims;
    nvinfer1::DimsHW beg_padding, end_padding;
    std::string mode;
    std::vector<int> pad_width;
    float value = 0.0f;

    // parser attrs
    AttrProcMap reqAttrProcs;
    AttrProcMap optAttrProcs;

    reqAttrProcs["mode"] = [&](const std::string& strVal) { mode = strVal; };
    optAttrProcs["pad_width"] = [&](const std::string& strVal) {
        auto mxnet_pad_width = Str2Vector<int>(strVal);
        pad_width = mxnet_pad_width;
    };
    optAttrProcs["constant_value"] = [&](const std::string& strVal) {
        auto mxnet_value = Str2Num<float>(strVal);
        value = mxnet_value;
    };

    ProcAttrs(node, reqAttrProcs, true);
    ProcAttrs(node, optAttrProcs, false);

    ASSERT(mode == "constant" || mode == "edge", ErrorCode::kUNSUPPORTED_NODE);

    ASSERT(pad_width.size() == ndim * 2, ErrorCode::kUNSUPPORTED_NODE);
    beg_padding.h() = pad_width[ndim * 2 - 4];
    end_padding.h() = pad_width[ndim * 2 - 3];
    beg_padding.w() = pad_width[ndim * 2 - 2];
    end_padding.w() = pad_width[ndim * 2 - 1];

    /*if (mode == "constant" && value == 0)
    {
        RETURN_FIRST_OUTPUT(ctx->network()->addPadding(tensor, beg_padding, end_padding));
    }
    else*/
    {   int enumMode;
        if(mode == "constant")enumMode = 0;
        if(mode == "edge")enumMode = 1;
        auto plug = arctern::trtPlugin::createPadPlugin(node.strName.c_str(), enumMode, pad_width, value);
        std::vector<nvinfer1::ITensor*> const& plugin_inputs = {&inputs.at(0).tensor()};
        RETURN_FIRST_OUTPUT(ctx->network()->addPluginV2(plugin_inputs.data(), plugin_inputs.size(), *plug));
    }
}

DEFINE_BUILTIN_OP_IMPORTER(repeat)
{
  ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
  nvinfer1::ITensor& tensor = inputs.at(0).tensor();
  int ndim = tensor.getDimensions().nbDims;
  ASSERT(ndim == 4, ErrorCode::kINVALID_VALUE);

  int repeats, axis;
  AttrProcMap optAttrProcs;
  optAttrProcs["repeats"] = [&](const std::string& strVal) {
      auto mxnet_repeat = Str2Num<int>(strVal);
      repeats = mxnet_repeat;
  };
  optAttrProcs["axis"] = [&](const std::string& strVal) {
      auto mxnet_axis = Str2Num<int>(strVal);
      if(mxnet_axis < 0){
        axis = ndim + mxnet_axis;
      }
  };

  ProcAttrs(node, optAttrProcs, true);

  if(repeats == 1){
    return {{&inputs.at(0).tensor()}};
  }

  auto plug = arctern::trtPlugin::createRepeatPlugin(node.strName.c_str(), repeats, axis);
  std::vector<nvinfer1::ITensor*> const& plugin_inputs = {&inputs.at(0).tensor()};
  RETURN_FIRST_OUTPUT(ctx->network()->addPluginV2(plugin_inputs.data(), plugin_inputs.size(), *plug));
}

DEFINE_BUILTIN_OP_IMPORTER(pow)
{
    ASSERT(inputs.size() == 2, ErrorCode::kINVALID_NODE);
    return combineTensorsElementwise(ctx, node, inputs, nvinfer1::ElementWiseOperation::kPOW, true);
}

DEFINE_BUILTIN_OP_IMPORTER(reciprocal)
{
    return apply_unary_function(ctx, inputs.at(0), nvinfer1::UnaryOperation::kRECIP);
}

DEFINE_BUILTIN_OP_IMPORTER(sum)
{
    return reduceTensor(ctx, node, inputs.at(0), nvinfer1::ReduceOperation::kSUM);
}

DEFINE_BUILTIN_OP_IMPORTER(relu)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    RETURN_FIRST_OUTPUT(ctx->network()->addActivation(inputs.at(0).tensor(), nvinfer1::ActivationType::kRELU));
}

DEFINE_BUILTIN_OP_IMPORTER(Reshape)
{
    auto input = inputs.at(0);
    nvinfer1::Dims new_shape{};
    bool reverse = false;

    AttrProcMap optAttrProcs;
    optAttrProcs["shape"] = [&](const std::string& strVal) {
        auto mxnet_shape = Str2Vector<int>(strVal);
        new_shape.nbDims = mxnet_shape.size();
        for (int i = 0; i < new_shape.nbDims; i++)
        {
            new_shape.d[i] = mxnet_shape[i];
        }
    };
    optAttrProcs["reverse"] = [&](const std::string& strVal) {
        auto mxnet_reverse = Str2Bool<bool>(strVal);
        reverse = mxnet_reverse;
    };
    ProcAttrs(node, optAttrProcs, false);

    // Check for -1 dimension in new shape
    TRT_CHECK(get_infer_dim(input.shape(), new_shape, reverse));

    if (input.is_weights())
    {
        auto weights = input.weights();
        weights.shape = new_shape;
        return {{weights}};
    }
    else
    {
        nvinfer1::ITensor& tensor = input.tensor();
        nvinfer1::IShuffleLayer* layer = ctx->network()->addShuffle(tensor);
        ASSERT(layer, ErrorCode::kUNSUPPORTED_NODE);
        layer->setReshapeDimensions(new_shape);
        ASSERT(get_shape_size(layer->getOutput(0)->getDimensions()) == get_shape_size(input.shape()),
            ErrorCode::kUNSUPPORTED_NODE);
        RETURN_FIRST_OUTPUT(layer);
    }
}

DEFINE_BUILTIN_OP_IMPORTER(transpose)
{
    auto input = inputs.at(0);
    std::vector<int> axes(input.shape().nbDims);
    for (size_t i = 0; i < axes.size(); ++i)
    {
        axes[i] = axes.size() - 1 - i;
    }

    AttrProcMap optAttrProcs;
    optAttrProcs["axes"] = [&](const std::string& strVal) {
        auto mxnet_axes = Str2Vector<int>(strVal);
        axes = mxnet_axes;
    };

    ProcAttrs(node, optAttrProcs, false);

    nvinfer1::ITensor& tensor = input.tensor();
    nvinfer1::Permutation perm{};
    std::copy(axes.begin(), axes.end(), perm.order);
    nvinfer1::ITensor* tensor_ptr;
    ASSERT(tensor_ptr = transpose_tensor(ctx, tensor, perm, false), ErrorCode::kUNSUPPORTED_NODE);
    return {{tensor_ptr}};
}

// TODO : RNN
// DEFINE_BUILTIN_OP_IMPORTER(RNN) {
//  OnnxAttrs attrs(node);
//  std::string direction_str = attrs.get("direction", "forward");
//  ASSERT(direction_str == "forward" || direction_str == "bidirectional",
//         ErrorCode::kUNSUPPORTED_NODE);
//  nvinfer1::RNNDirection direction = (direction_str == "forward" ?
//                                      nvinfer1::RNNDirection::kUNIDIRECTION :
//                                      nvinfer1::RNNDirection::kBIDIRECTION);
//  int hidden_size = attrs.get<int>("hidden_size");
//  std::vector<std::string> default_activation_strs = {"TanH", "TanH"};
//  auto activation_strs = attrs.get("activations", default_activation_strs);
//  ASSERT(activation_strs.size() == 1 || activation_strs.size() == 2,
//         ErrorCode::kINVALID_NODE);
//  if( activation_strs.size() == 2 ) {
//    ASSERT(activation_strs.at(1) == activation_strs.at(0),
//           ErrorCode::kUNSUPPORTED_NODE);
//  }
//  std::string activation_str = activation_strs.at(0);
//  ASSERT(activation_str == "TanH" || activation_str == "Relu",
//         ErrorCode::kUNSUPPORTED_NODE);
//  nvinfer1::RNNOperation op = (activation_str == "TanH" ?
//                                nvinfer1::RNNOperation::kTANH :
//                                nvinfer1::RNNOperation::kRELU);
//  nvinfer1::RNNMode mode = nvinfer1::RnnMode::kLINEAR;
//  int do_output_sequence = attrs.get("output_sequence", 0);
//  ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
//  int layer_count = 1;
//  int max_sequence_length = 64; // TODO: How to specify this?
//
//  // TODO: weights = concatenate inputs.at(1).weights() and inputs.at(2).weights() over slowest dim
//  //       biases  = inputs.at(3).weights(); [OPTIONAL, default 0]
//
//  auto* layer = ctx->network()->addRNN(
//    inputs.at(0).tensor(), layer_count, hidden_size, max_seq_length,
//    op, mode, direction, weights, biases);
//
//  // TODO: Looks like we need to transpose the outputs from [1, T, N, dir, C] to [1, T, dir, N, C]
//  //       Return {{output 0, output 1}}, but take care of outputs being optional (i.e., check how many outputs
// there are, as well as output_sequence)
//}

DEFINE_BUILTIN_OP_IMPORTER(sigmoid)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    RETURN_FIRST_OUTPUT(ctx->network()->addActivation(inputs.at(0).tensor(), nvinfer1::ActivationType::kSIGMOID));
}

DEFINE_BUILTIN_OP_IMPORTER(sin)
{
    return unaryHelper(ctx, node, inputs, nvinfer1::UnaryOperation::kSIN);
}

DEFINE_BUILTIN_OP_IMPORTER(sinh)
{
    return unaryHelper(ctx, node, inputs, nvinfer1::UnaryOperation::kSINH);
}

DEFINE_BUILTIN_OP_IMPORTER(slice)
{
  ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
  nvinfer1::ITensor& tensor = inputs.at(0).tensor();

  std::vector<std::string> begin_str;
  std::vector<std::string> end_str;
  std::vector<std::string> step_str;

  auto trim = [](std::string str){
    if(str.empty()){
      return str;
    }
    str.erase(0, str.find_first_not_of(" "));
    str.erase(str.find_last_not_of(" ") + 1);
    return str;
  };

  // parser attrs
  AttrProcMap reqAttrProcs;
  reqAttrProcs["begin"] = [&](const std::string& strVal) {
    auto subStrVal = strVal.substr(strVal.find("(") + 1, strVal.find(")")-strVal.find("(")-1);
    std::stringstream ss(subStrVal);
    std::string tmp;
    while(getline(ss, tmp, ',')){
      begin_str.push_back(trim(tmp));
    }
  };
  reqAttrProcs["end"] = [&](const std::string& strVal) {
    auto subStrVal = strVal.substr(strVal.find("(") + 1, strVal.find(")")-strVal.find("(")-1);
    std::stringstream ss(subStrVal);
    std::string tmp;
    while(getline(ss, tmp, ',')){
      end_str.push_back(trim(tmp));
    }
  };
  reqAttrProcs["step"] = [&](const std::string& strVal) {
    auto subStrVal = strVal.substr(strVal.find("(") + 1, strVal.find(")")-strVal.find("(")-1);
    std::stringstream ss(subStrVal);
    std::string tmp;
    while(getline(ss, tmp, ',')){
      step_str.push_back(trim(tmp));
    }
  };
  ProcAttrs(node, reqAttrProcs, true);

  /// rule
  const nvinfer1::Dims dims = tensor.getDimensions();
  std::vector<int> begin;
  std::vector<int> end;
  std::vector<int> step;
  if(step_str.empty()){
    step.resize(begin_str.size(), 1);
  }else{
    for(int i = 0; i < step_str.size(); ++i){
      if(step_str[i] == "None"){
        step.push_back(1);
      }else{
        step.push_back(Str2Num<int>(step_str[i]));
      }
    }
  }

  for(int i = 0; i < begin_str.size(); ++i){
    if(begin_str[i] == "None"){
      if(step[i] > 0){
        begin.push_back(0);
      }else{
        begin.push_back(dims.d[i] - 1);
      }
    }else{
      begin.push_back(Str2Num<int>(begin_str[i]));
    }
  }
  for(int i = 0; i < end_str.size(); ++i){
    if(end_str[i] == "None"){
      if(step[i] > 0){
        end.push_back(dims.d[i]);
      }else{
        end.push_back(-1);
      }
    }else{
      end.push_back(Str2Num<int>(end_str[i]));
    }
  }

  const int nbDims = dims.nbDims;
  auto makeDims = [nbDims](int initVal) -> nvinfer1::Dims {
    nvinfer1::Dims result{nbDims, {}, {}};
    std::fill_n(&result.d[0], nbDims, initVal);
    return result;
  };
  nvinfer1::Dims sliceStart = makeDims(0);
  nvinfer1::Dims sliceSize = dims;
  const nvinfer1::Dims sliceStride = makeDims(1); // ONNX has no support for strides in Slice
  for(int i = 0; i < begin.size(); ++i){
    sliceStart.d[i] = begin[i];
    ASSERT(step[i] > 0, ErrorCode::kINVALID_VALUE); /// Notice: Currently support step > 0 only
    ASSERT(begin[i] >=0, ErrorCode::kINVALID_VALUE);/// Notice: Currently support begine > 0 only
    ASSERT(end[i] > 0, ErrorCode::kINVALID_VALUE);/// Notice: Currently support end > 0 only
    sliceSize.d[i] = end[i] - begin[i];
  }


  RETURN_FIRST_OUTPUT(ctx->network()->addSlice(tensor, sliceStart, sliceSize, sliceStride));
}


DEFINE_BUILTIN_OP_IMPORTER(slice_axis)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    nvinfer1::ITensor& tensor = inputs.at(0).tensor();
    int axis;
    int begin;
    int end = 0;

    // parser attrs
    AttrProcMap reqAttrProcs;

    reqAttrProcs["axis"] = [&](const std::string& strVal) {
        auto mxnet_axis = Str2Num<int>(strVal);
        axis = mxnet_axis >= 0 ? mxnet_axis : mxnet_axis + tensor.getDimensions().nbDims;
    };
    reqAttrProcs["begin"] = [&](const std::string& strVal) {
        auto mxnet_begin = Str2Num<int>(strVal);
        begin = mxnet_begin;
    };
    reqAttrProcs["end"] = [&](const std::string& strVal) {
        if (strVal != "None")
        {
            auto mxnet_end = Str2Num<int>(strVal);
            end = mxnet_end;
        }
    };

    ProcAttrs(node, reqAttrProcs, true);

    const nvinfer1::Dims dims = tensor.getDimensions();
    int dim = dims.d[axis];
    begin = begin >= 0 ? begin : dim + begin;
    end = end > 0 ? end : dim + end;

    const int nbDims = dims.nbDims;
    auto makeDims = [nbDims](int initVal) -> nvinfer1::Dims {
        nvinfer1::Dims result{nbDims, {}, {}};
        std::fill_n(&result.d[0], nbDims, initVal);
        return result;
    };
    nvinfer1::Dims sliceStart = makeDims(0);
    nvinfer1::Dims sliceSize = dims;
    const nvinfer1::Dims sliceStride = makeDims(1); // ONNX has no support for strides in Slice
    sliceStart.d[axis] = begin;
    sliceSize.d[axis] = end < dim ? end - begin : dim - begin;

    // If entire slice op was a no-op, simply return the input tensor
    if (sliceStart == makeDims(0) && sliceSize == dims)
    {
        return {{&tensor}};
    }

    /// when the slice's axis is one of the last 2 dims, we using addPadding to replace addSlice
    if(axis == nbDims-1 || axis == nbDims-2){
      nvinfer1::Dims new_begin = makeDims(0);
      nvinfer1::Dims new_end = makeDims(0);
      for(int i = 0; i < nbDims; ++i){
        if(axis != i){
          new_end.d[i] = dims.d[i];
        }else{
          new_begin.d[i] = begin;
          new_end.d[i] = end;
        }
      }

      nvinfer1::DimsHW pre_padding;
      nvinfer1::DimsHW post_padding;
      for(int i = 0; i < 2; ++i){
        pre_padding.d[i] = 0 - new_begin.d[nbDims-2+i];
        post_padding.d[i] = new_end.d[nbDims-2+i] - dims.d[nbDims-2+i];
      }
      RETURN_FIRST_OUTPUT(ctx->network()->addPadding(tensor, pre_padding, post_padding));
    }else{
      RETURN_FIRST_OUTPUT(ctx->network()->addSlice(tensor, sliceStart, sliceSize, sliceStride));
    }
}

DEFINE_BUILTIN_OP_IMPORTER(slice_like)
{
  ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
  ASSERT(inputs.at(1).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
  nvinfer1::ITensor& tensor = inputs.at(0).tensor();
  nvinfer1::ITensor& tensor_t = inputs.at(1).tensor();

  std::vector<int> vaxis;
  // parser attrs
  AttrProcMap reqAttrProcs;
  reqAttrProcs["axes"] = [&](const std::string& strVal) {
      auto subStrVal = strVal.substr(strVal.find("(") + 1, strVal.find(")")-strVal.find("(")-1);
      std::stringstream ss(subStrVal);
      std::string tmp;
      while(getline(ss, tmp, ',')){
        vaxis.push_back(Str2Num<int>(tmp));
      }
  };

  ProcAttrs(node, reqAttrProcs, true);
  const nvinfer1::Dims dims = tensor.getDimensions();
  const nvinfer1::Dims dims_t = tensor_t.getDimensions();
  const int nbDims = dims.nbDims;
  auto makeDims = [nbDims](int initVal) -> nvinfer1::Dims {
      nvinfer1::Dims result{nbDims, {}, {}};
      std::fill_n(&result.d[0], nbDims, initVal);
      return result;
  };

  std::vector<int> begin, end, step;
  if(vaxis.size() ==0){
    for(int i = 0; i < nbDims; ++i){
      begin.push_back(0);
      end.push_back(dims_t.d[i]);
      step.push_back(1);
    }
  }else{
    for(int i = 0; i < vaxis.size(); ++i){
      int axis = vaxis[i];
      if(axis < 0)axis += nbDims;
      ASSERT(axis >= 0, ErrorCode::kINVALID_VALUE);
      ASSERT(axis < nbDims, ErrorCode::kINVALID_VALUE);
      ASSERT(axis < dims_t.nbDims, ErrorCode::kINVALID_VALUE);
      begin.push_back(0);
      end.push_back(dims_t.d[axis]);
      step.push_back(1);
    }
  }

  nvinfer1::Dims sliceStart = makeDims(0);
  nvinfer1::Dims sliceSize = dims;
  const nvinfer1::Dims sliceStride = makeDims(1);
  for(int i = 0; i < vaxis.size(); ++i){
    sliceStart.d[vaxis[i]] = begin[i];
    sliceSize.d[vaxis[i]] = end[i] - begin[i];
  }

  RETURN_FIRST_OUTPUT(ctx->network()->addSlice(tensor, sliceStart, sliceSize, sliceStride));
}

DEFINE_BUILTIN_OP_IMPORTER(SliceChannel)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    int num_outputs;
    int axis = 1;
    bool squeeze_axis = false;

    // parser attrs
    AttrProcMap reqAttrProcs;
    AttrProcMap optAttrProcs;

    reqAttrProcs["num_outputs"] = [&](const std::string& strVal) {
        auto mxnet_num_outputs = Str2Num<int>(strVal);
        num_outputs = mxnet_num_outputs;
    };
    optAttrProcs["axis"] = [&](const std::string& strVal) {
        auto mxnet_axis = Str2Num<int>(strVal);
        axis = mxnet_axis;
    };
    optAttrProcs["squeeze_axis"] = [&](const std::string& strVal) {
        auto mxnet_squeeze_axis = Str2Bool<bool>(strVal);
        squeeze_axis = mxnet_squeeze_axis;
    };

    ProcAttrs(node, reqAttrProcs, true);
    ProcAttrs(node, optAttrProcs, false);
    ASSERT(!squeeze_axis, ErrorCode::kUNSUPPORTED_NODE);

    nvinfer1::ITensor& tensor = inputs.at(0).tensor();
    const nvinfer1::Dims dims = tensor.getDimensions();
    const int nbDims = dims.nbDims;
    /// constraint: the dims of input must be more than 2
    ASSERT(nbDims > 2, ErrorCode::kUNSUPPORTED_NODE);
    /// constraint: when the slice's axis is one of the last 2 dims,
    /// we using addPadding to replace addSlice, so the axis of slice must be [nbDims-2 or nbDims-1]
    ASSERT(axis >= nbDims - 2, ErrorCode::kUNSUPPORTED_NODE);

    int step = dims.d[axis] / num_outputs;
    std::vector<TensorOrWeights> outputs(num_outputs);
    for(int i = 0; i < num_outputs; ++i){
      nvinfer1::DimsHW pre_padding;
      nvinfer1::DimsHW post_padding;
      for(int j = nbDims-2; j < nbDims; ++j){
        if(j == axis){
          pre_padding.d[j-nbDims+2] = 0 - (i*step);
          post_padding.d[j-nbDims+2] = step - dims.d[axis] + i*step;
        }else
        {
          pre_padding.d[j-nbDims+2] = 0;
          post_padding.d[j-nbDims+2] = 0;
        }
      }
      auto layer_ptr = ctx->network()->addPadding(tensor, pre_padding, post_padding);
      outputs[i] = layer_ptr->getOutput(0);
    }

    return {outputs};
}

DEFINE_BUILTIN_OP_IMPORTER(softmax)
{
    nvinfer1::ITensor* tensor_ptr = &inputs.at(0).tensor();
    int ndim = inputs.at(0).shape().nbDims;
    int axis = ndim-1;

    // parser attrs
    AttrProcMap optAttrProcs;
    optAttrProcs["axis"] = [&](const std::string& strVal) {
        auto mxnet_axis = Str2Num<int>(strVal);
        axis = mxnet_axis >= 0 ? mxnet_axis : mxnet_axis + ndim;
    };
    ProcAttrs(node, optAttrProcs, false);

    auto* layer = ctx->network()->addSoftMax(*tensor_ptr);
    ASSERT(layer, ErrorCode::kUNSUPPORTED_NODE);
    layer->setAxes(1 << axis);
    tensor_ptr = layer->getOutput(0);
    return {{tensor_ptr}};
}

DEFINE_BUILTIN_OP_IMPORTER(SoftmaxActivation)
{
    nvinfer1::ITensor* tensor_ptr = &inputs.at(0).tensor();
    int ndim = inputs.at(0).shape().nbDims;
    std::string mode("instance");

    // parser attrs
    AttrProcMap optAttrProcs;
    optAttrProcs["mode"] = [&](const std::string& strVal) { mode = strVal; };
    ProcAttrs(node, optAttrProcs, false);

    if (mode == "channel" || (mode == "instance" && ndim == 2))
    {
        int axis = 1;
        auto* layer = ctx->network()->addSoftMax(*tensor_ptr);
        ASSERT(layer, ErrorCode::kUNSUPPORTED_NODE);
        layer->setAxes(1 << axis);
        tensor_ptr = layer->getOutput(0);
        return {{tensor_ptr}};
    }
    else
    {
        return MAKE_ERROR("Assertion failed: Not impelemnet yet.", ErrorCode::kINVALID_NODE);
    }
}

DEFINE_BUILTIN_OP_IMPORTER(SoftmaxOutput)
{
    bool preserve_shape = false;

    // parser attrs
    AttrProcMap optAttrProcs;
    optAttrProcs["preserve_shape"] = [&](const std::string& strVal) {
        auto mxnet_preserve_shape = Str2Bool<bool>(strVal);
        preserve_shape = mxnet_preserve_shape;
    };
    ProcAttrs(node, optAttrProcs, false);

    ASSERT(!preserve_shape, ErrorCode::kUNSUPPORTED_NODE);
    nvinfer1::ITensor* tensor_ptr = &inputs.at(0).tensor();
    int ndim = inputs.at(0).shape().nbDims;
    auto* layer = ctx->network()->addSoftMax(*tensor_ptr);
    ASSERT(layer, ErrorCode::kUNSUPPORTED_NODE);
    layer->setAxes(1 << (ndim - 1));
    tensor_ptr = layer->getOutput(0);
    return {{tensor_ptr}};
}

DEFINE_BUILTIN_OP_IMPORTER(softsign)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    return activationHelper(ctx, node, inputs, nvinfer1::ActivationType::kSOFTSIGN);
}

DEFINE_BUILTIN_OP_IMPORTER(space_to_depth)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    nvinfer1::ITensor* tensor_ptr = &inputs.at(0).tensor();
    nvinfer1::IShuffleLayer* layer = ctx->network()->addShuffle(*tensor_ptr);
    ASSERT(layer, ErrorCode::kUNSUPPORTED_NODE);

    // parser attrs
    int block_size = 0;

    AttrProcMap reqAttrProcs;
    reqAttrProcs["block_size"] = [&](const std::string& strVal) {
        auto mxnet_block_size = Str2Num<int>(strVal);
        block_size = mxnet_block_size;
    };

    ProcAttrs(node, reqAttrProcs, true);

    nvinfer1::Dims dims = tensor_ptr->getDimensions();
    int ndim_spatial = dims.nbDims - 1;
    nvinfer1::Dims new_shape1;
    new_shape1.nbDims = dims.nbDims + ndim_spatial;
    new_shape1.d[0] = dims.d[0];
    for (int i = 0; i < ndim_spatial; ++i)
    {
        ASSERT(dims.d[1 + i] % block_size == 0, ErrorCode::kINVALID_NODE);
        new_shape1.d[1 + 2 * i + 0] = dims.d[1 + i] / block_size;
        new_shape1.d[1 + 2 * i + 1] = block_size;
    }
    layer->setReshapeDimensions(new_shape1);
    nvinfer1::Permutation perm;
    perm.order[ndim_spatial] = 0;
    for (int i = 0; i < ndim_spatial; ++i)
    {
        perm.order[ndim_spatial + 1 + i] = 1 + 2 * i + 0;
        perm.order[i] = 1 + 2 * i + 1;
    }
    layer->setSecondTranspose(perm);
    tensor_ptr = layer->getOutput(0);
    dims = tensor_ptr->getDimensions();
    nvinfer1::Dims new_shape2;
    new_shape2.nbDims = dims.nbDims - ndim_spatial;
    new_shape2.d[0] = dims.d[ndim_spatial];
    for (int i = 0; i < ndim_spatial; ++i)
    {
        new_shape2.d[0] *= dims.d[i];
        new_shape2.d[1 + i] = dims.d[ndim_spatial + 1 + i];
    }
    tensor_ptr = reshape_tensor(ctx, *tensor_ptr, new_shape2);
    ASSERT(tensor_ptr, ErrorCode::kUNSUPPORTED_NODE);
    dims = tensor_ptr->getDimensions();
    return {{tensor_ptr}};
}

// TODO : Split
// DEFINE_BUILTIN_OP_IMPORTER(Split) {
//  ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
//  ASSERT(inputs.size() == 1, ErrorCode::kUNSUPPORTED_NODE);
//  nvinfer1::Dims dims = inputs.at(0).shape();
//  int nbDims = dims.nbDims;
//  OnnxAttrs attrs(node);
//  int axis = attrs.get<int>("axis", 0);
//  TRT_CHECK(convert_axis(axis, nbDims));
//  std::vector<int> output_lengths;
//  int noutput = node.output().size();
//  if( attrs.count("split") ) {
//    output_lengths = attrs.get<std::vector<int>>("split");
//    ASSERT((int)output_lengths.size() == noutput, ErrorCode::kINVALID_NODE);
//  } else {
//    ASSERT(dims.d[axis] % noutput == 0, ErrorCode::kINVALID_NODE);
//    output_lengths.assign(noutput, dims.d[axis] / noutput);
//  }
//  nvinfer1::IPluginV2Layer* layer =
//      ctx->addPluginV2(new SplitPlugin(axis, output_lengths),
//                     {&inputs.at(0).tensor()});
//  ASSERT(layer, ErrorCode::kUNSUPPORTED_NODE);
//  ASSERT(layer->getNbOutputs() == noutput, ErrorCode::kINTERNAL_ERROR);
//  std::vector<TensorOrWeights> outputs;
//  for( int i=0; i<noutput; ++i ) {
//    outputs.push_back(layer->getOutput(i));
//  }
//  return outputs;
//}

DEFINE_BUILTIN_OP_IMPORTER(GridGenerator)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    nvinfer1::ITensor& tensor = inputs.at(0).tensor();
    std::vector<int> target_shape;
    int transferType = -1;

    // parser attrs
    AttrProcMap reqAttrProcs;
    AttrProcMap optAttrProcs;

    optAttrProcs["target_shape"] = [&](const std::string& strVal) {
        auto mxnet_pad_width = Str2Vector<int>(strVal);
        target_shape = mxnet_pad_width;
    };
    reqAttrProcs["transform_type"] = [&](const std::string& strVal) {
        if (strVal == "affine")
        {
            transferType = 0;
        }
    };

    ProcAttrs(node, reqAttrProcs, true);
    ProcAttrs(node, optAttrProcs, false);

    ASSERT(transferType == 0, ErrorCode::kUNSUPPORTED_NODE);

    auto plug = arctern::trtPlugin::createGridGeneratorPlugin(node.strName.c_str(), target_shape, transferType);
    std::vector<nvinfer1::ITensor*> const& plugin_inputs = {&inputs.at(0).tensor()};
    RETURN_FIRST_OUTPUT(ctx->network()->addPluginV2(plugin_inputs.data(), plugin_inputs.size(), *plug));
}

DEFINE_BUILTIN_OP_IMPORTER(BilinearSampler)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    ASSERT(inputs.at(1).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);

    auto plug = arctern::trtPlugin::createBilinearSamplerPlugin(node.strName.c_str());
    std::vector<nvinfer1::ITensor*> const& plugin_inputs = {&inputs.at(0).tensor(), &inputs.at(1).tensor()};
    RETURN_FIRST_OUTPUT(ctx->network()->addPluginV2(plugin_inputs.data(), plugin_inputs.size(), *plug));
}

DEFINE_BUILTIN_OP_IMPORTER(SpatialTransformer)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    nvinfer1::ITensor& tensor = inputs.at(0).tensor();
    std::vector<int> target_shape;
    int transferType = -1;
    int samplerType = -1;

    // parser attrs
    AttrProcMap reqAttrProcs;
    AttrProcMap optAttrProcs;

    optAttrProcs["target_shape"] = [&](const std::string& strVal) {
        auto mxnet_pad_width = Str2Vector<int>(strVal);
        target_shape = mxnet_pad_width;
    };
    reqAttrProcs["transform_type"] = [&](const std::string& strVal) {
        if (strVal == "affine")
        {
            transferType = 0;
        }
    };
    reqAttrProcs["sampler_type"] = [&](const std::string& strVal) {
        if (strVal == "bilinear")
        {
            samplerType = 0;
        }
    };

    ProcAttrs(node, reqAttrProcs, true);
    ProcAttrs(node, optAttrProcs, false);

    ASSERT(transferType == 0, ErrorCode::kUNSUPPORTED_NODE);
    ASSERT(samplerType == 0, ErrorCode::kUNSUPPORTED_NODE);

    auto plug = arctern::trtPlugin::createSpatialTransformerPlugin(
        node.strName.c_str(), target_shape, transferType, samplerType);
    std::vector<nvinfer1::ITensor*> const& plugin_inputs = {&inputs.at(0).tensor(), &inputs.at(1).tensor()};
    RETURN_FIRST_OUTPUT(ctx->network()->addPluginV2(plugin_inputs.data(), plugin_inputs.size(), *plug));
}

DEFINE_BUILTIN_OP_IMPORTER(sqrt)
{
    return apply_unary_function(ctx, inputs.at(0), nvinfer1::UnaryOperation::kSQRT);
}

DEFINE_BUILTIN_OP_IMPORTER(squeeze)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    nvinfer1::ITensor& tensor = inputs.at(0).tensor();
    nvinfer1::Dims old_shape = tensor.getDimensions();
    int ndim_in = old_shape.nbDims;
    std::vector<int> axis;

    AttrProcMap optAttrProcs;

    optAttrProcs["axis"] = [&](const std::string& strVal) {
        auto mxnet_axis = Str2Vector<int>(strVal);
        axis = mxnet_axis;
    };
    ProcAttrs(node, optAttrProcs, false);

    nvinfer1::Dims new_shape;
    if (axis.empty())
    {
        int j = 0;
        for (int i = 0; i < old_shape.nbDims; ++i)
        {
            if (old_shape.d[i] != 1)
            {
                new_shape.d[j++] = old_shape.d[i];
            }
            else
            {
                ASSERT(old_shape.d[i] == 1, ErrorCode::kINVALID_NODE);
            }
        }
        new_shape.nbDims = j;
    }
    else
    {
        std::set<int> axes_set(axis.begin(), axis.end());
        int ndim_out = ndim_in - axes_set.size();
        ASSERT(ndim_out <= nvinfer1::Dims::MAX_DIMS, ErrorCode::kUNSUPPORTED_NODE);
        new_shape.nbDims = ndim_out;
        for (int i = 0, j = 0; i < old_shape.nbDims; ++i)
        {
            if (!axes_set.count(i))
            {
                new_shape.d[j++] = old_shape.d[i];
            }
            else
            {
                ASSERT(old_shape.d[i] == 1, ErrorCode::kINVALID_NODE);
            }
        }
    }
    nvinfer1::IShuffleLayer* layer = ctx->network()->addShuffle(tensor);
    ASSERT(layer, ErrorCode::kUNSUPPORTED_NODE);
    layer->setReshapeDimensions(new_shape);
    ASSERT(get_shape_size(layer->getOutput(0)->getDimensions()) == get_shape_size(old_shape),
     ErrorCode::kUNSUPPORTED_NODE);
    RETURN_FIRST_OUTPUT(layer);
}

DEFINE_BUILTIN_OP_IMPORTER(tan)
{
    return unaryHelper(ctx, node, inputs, nvinfer1::UnaryOperation::kTAN);
}

DEFINE_BUILTIN_OP_IMPORTER(tanh)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    RETURN_FIRST_OUTPUT(ctx->network()->addActivation(inputs.at(0).tensor(), nvinfer1::ActivationType::kTANH));
}

DEFINE_BUILTIN_OP_IMPORTER(topk)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    nvinfer1::ITensor& tensor = inputs.at(0).tensor();
    int ndim = tensor.getDimensions().nbDims;
    ASSERT(tensor.getType() != nvinfer1::DataType::kINT32, ErrorCode::kUNSUPPORTED_NODE);

    int axis = -1;
    int k = 1;
    std::string ret_typ = "indices"; // 'both', 'indices', 'mask', 'value'
    bool is_ascend = false;

    AttrProcMap optAttrProcs;

    optAttrProcs["axis"] = [&](const std::string& strVal) {
        auto mxnet_axis = Str2Num<int>(strVal);
        axis = mxnet_axis >= 0 ? mxnet_axis : mxnet_axis + ndim;
    };
    optAttrProcs["k"] = [&](const std::string& strVal) {
        auto mxnet_k = Str2Num<int>(strVal);
        k = mxnet_k;
    };
    optAttrProcs["ret_typ"] = [&](const std::string& strVal) { ret_typ = strVal; };
    optAttrProcs["is_ascend"] = [&](const std::string& strVal) {
        auto mxnet_is_ascend = Str2Bool<bool>(strVal);
        is_ascend = mxnet_is_ascend;
    };

    ProcAttrs(node, optAttrProcs, false);

    ASSERT(ret_typ == "value" || ret_typ == "indices" || ret_typ == "both", ErrorCode::kUNSUPPORTED_NODE);
    if (ret_typ == "value")
    {
        uint32_t axis_mask = 1 << axis;
        auto* layer = ctx->network()->addTopK(tensor, nvinfer1::TopKOperation::kMAX, k, axis_mask);
        ASSERT(layer, ErrorCode::kUNSUPPORTED_NODE);
        RETURN_FIRST_OUTPUT(layer);
    }
    else if (ret_typ == "indices")
    {
        ASSERT(axis == ndim - 1, ErrorCode::kUNSUPPORTED_NODE);
        auto plug = arctern::trtPlugin::createLastDimTopKPlugin(node.strName.c_str(), is_ascend, 0, k);
        std::vector<nvinfer1::ITensor*> const& plugin_inputs = {&inputs.at(0).tensor()};
        RETURN_SECOND_OUTPUT(ctx->network()->addPluginV2(plugin_inputs.data(), plugin_inputs.size(), *plug));
    }else if (ret_typ == "both"){
        ASSERT(axis == ndim - 1, ErrorCode::kUNSUPPORTED_NODE);
        auto plug = arctern::trtPlugin::createLastDimTopKPlugin(node.strName.c_str(), is_ascend, 0, k);
        std::vector<nvinfer1::ITensor*> const& plugin_inputs = {&inputs.at(0).tensor()};

        nvinfer1::ILayer* layer_ptr = ctx->network()->addPluginV2(plugin_inputs.data(), plugin_inputs.size(), *plug);
        ASSERT(layer_ptr, ErrorCode::kUNSUPPORTED_NODE);
        return {{layer_ptr->getOutput(0),layer_ptr->getOutput(1)}};
    }
}

// TODO : transpose
// DEFINE_BUILTIN_OP_IMPORTER(transpose) {
//  TensorOrWeights input = inputs.at(0);
//  OnnxAttrs attrs(node);
//  int ndim = input.shape().nbDims;
//  ASSERT(ndim <= nvinfer1::Dims::MAX_DIMS, ErrorCode::kUNSUPPORTED_NODE);
//  nvinfer1::Permutation default_perm; // Default is to reverse dims
//  for( int i=0; i<ndim; ++i ) {
//    default_perm.order[i] = ndim - 1 - i;
//  }
//  nvinfer1::Permutation perm = attrs.get("perm", default_perm);
//  if( input.is_tensor() ) {
//    // TRT doesn't support moving the batch dim
//    ASSERT(perm.order[BATCH_DIM] == BATCH_DIM, ErrorCode::kUNSUPPORTED_NODE);
//    perm = remove_first_dim(perm);
//    // Note: Dimension types kept unchanged in order to avoid TRT complaining about CHW order
//    nvinfer1::ITensor* output_tensor =
//        transpose_tensor(ctx, input.tensor(), perm, false);
//    ASSERT(output_tensor, ErrorCode::kUNSUPPORTED_NODE);
//    return {{output_tensor}};
//  } else {
//    auto weights = input.weights();
//    auto new_weights = ctx->createTempWeights(weights.type, weights.shape);
//    ASSERT(transposeWeights(weights, perm, &new_weights),
//           ErrorCode::kUNSUPPORTED_NODE);
//    weights = new_weights;
//    return {{weights}};
//  }
//}

DEFINE_BUILTIN_OP_IMPORTER(expand_dims)
{
    nvinfer1::ITensor& tensor = convertToTensor(inputs.at(0), ctx);
    nvinfer1::Dims old_shape = tensor.getDimensions();
    int ndim_in = old_shape.nbDims;
    int axis;

    AttrProcMap reqAttrProcs;

    reqAttrProcs["axis"] = [&](const std::string& strVal) {
        auto mxnet_axis = Str2Num<int>(strVal);
        axis = mxnet_axis >= 0 ? mxnet_axis : mxnet_axis + ndim_in;
    };

    ProcAttrs(node, reqAttrProcs, true);

    int ndim_out = ndim_in + 1;
    ASSERT(axis <= ndim_in, ErrorCode::kUNSUPPORTED_NODE);
    ASSERT(ndim_out <= nvinfer1::Dims::MAX_DIMS, ErrorCode::kUNSUPPORTED_NODE);
    nvinfer1::Dims new_shape{};
    new_shape.nbDims = ndim_out;

    for (int i = 0, j = 0; j < new_shape.nbDims; ++j)
    {
        if (j != axis)
        {
            new_shape.d[j] = old_shape.d[i++];
        }
        else
        {
            new_shape.d[j] = 1;
        }
    }
    nvinfer1::IShuffleLayer* layer = ctx->network()->addShuffle(tensor);
    ASSERT(layer, ErrorCode::kUNSUPPORTED_NODE);
    layer->setReshapeDimensions(new_shape);
    ASSERT(get_shape_size(layer->getOutput(0)->getDimensions()) == get_shape_size(old_shape),
        ErrorCode::kUNSUPPORTED_NODE);
    RETURN_FIRST_OUTPUT(layer);
}

DEFINE_BUILTIN_OP_IMPORTER(UpSampling)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);

    int scale = 0;
    int num_filter = 0;
    int sample_type = 0;
    int multi_input_mode = 0;
    // parser attrs
    AttrProcMap reqAttrProcs;
    AttrProcMap optAttrProcs;

    reqAttrProcs["scale"] = [&](const std::string& strVal) {
        auto mxnet_scale = Str2Num<int>(strVal);
        scale = mxnet_scale;
    };
    optAttrProcs["num_filter"] = [&](const std::string& strVal) {
        auto mxnet_num_filter = Str2Num<int>(strVal);
        num_filter = mxnet_num_filter;
    };
    reqAttrProcs["sample_type"] = [&](const std::string& strVal) {
        if (strVal == "nearest")
        {
            sample_type = 0;
        }
        else if (strVal == "bilinear")
        {
            sample_type = 1;
        }
        else
        {
            sample_type = -1;
        }
    };
    optAttrProcs["multi_input_mode"] = [&](const std::string& strVal) {
        if (strVal == "concat")
        {
            multi_input_mode = 0;
        }
        else if (strVal == "sum")
        {
            multi_input_mode = 1;
        }
        else
        {
            multi_input_mode = -1;
        }
    };

    ProcAttrs(node, reqAttrProcs, true);
    ProcAttrs(node, optAttrProcs, false);

    ASSERT(sample_type == 0, ErrorCode::kUNSUPPORTED_NODE);

    auto plug = arctern::trtPlugin::createUpSamplingPlugin(
        node.strName.c_str(), scale, num_filter, sample_type, multi_input_mode);
    std::vector<nvinfer1::ITensor*> const& plugin_inputs = {&inputs.at(0).tensor()};
    RETURN_FIRST_OUTPUT(ctx->network()->addPluginV2(plugin_inputs.data(), plugin_inputs.size(), *plug));
}

DEFINE_BUILTIN_OP_IMPORTER(Deconvolution)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    ASSERT(inputs.at(1).is_weights(), ErrorCode::kUNSUPPORTED_NODE);

    nvinfer1::ITensor* tensor_ptr = &inputs.at(0).tensor();
    auto kernel_weights = inputs.at(1).weights();
    nvinfer1::Dims dims = tensor_ptr->getDimensions();
    bool need_to_expand_dims = (dims.nbDims == 2);
    if (need_to_expand_dims)
    {
        // Expand spatial dims from 1D to 2D
        nvinfer1::DimsCHW new_shape(dims.d[0], dims.d[1], 1);
        tensor_ptr = reshape_tensor(ctx, *tensor_ptr, new_shape);
        ASSERT(tensor_ptr, ErrorCode::kUNSUPPORTED_NODE);
        dims = tensor_ptr->getDimensions();
    }
    if (kernel_weights.shape.nbDims == 3)
    {
        kernel_weights.shape.nbDims = 4;
        kernel_weights.shape.d[3] = 1;
    }
    ASSERT(dims.nbDims >= 3, ErrorCode::kUNSUPPORTED_NODE);
    ASSERT(kernel_weights.shape.nbDims == 4, ErrorCode::kUNSUPPORTED_NODE);
    nvinfer1::Weights bias_weights{};
    if (inputs.size() == 3)
    {
        ASSERT(inputs.at(2).is_weights(), ErrorCode::kUNSUPPORTED_NODE);
        auto shaped_bias_weights = inputs.at(2).weights();
        ASSERT(shaped_bias_weights.shape.nbDims == 1, ErrorCode::kINVALID_NODE);
//        ASSERT(shaped_bias_weights.shape.d[0] == kernel_weights.shape.d[0], ErrorCode::kINVALID_NODE);
        bias_weights = shaped_bias_weights;
    }
    else
    {
        bias_weights = ShapedWeights::empty(kernel_weights.type);
    }
    nvinfer1::DimsHW kernel_size;
    kernel_size.h() = kernel_weights.shape.d[2];
    kernel_size.w() = kernel_weights.shape.d[3];
    nvinfer1::DimsHW strides(1, 1);
    nvinfer1::DimsHW beg_padding(0, 0), end_padding(0, 0);
    nvinfer1::DimsHW dilations(1, 1);
    nvinfer1::PaddingMode paddingMode{nvinfer1::PaddingMode::kEXPLICIT_ROUND_DOWN};
    int nchan = dims.d[CHANNEL_DIM];
    int noutput; // Note: Weights order is KCRS
    int ngroup = 1;
    bool no_bias = false;

    // parser attrs
    AttrProcMap reqAttrProcs;
    AttrProcMap optAttrProcs;
    reqAttrProcs["kernel"] = [&](const std::string& strVal) {
        auto mxnet_kernel = Str2Vector<int>(strVal);
        int ndim = mxnet_kernel.size();
        kernel_size.h() = mxnet_kernel[0];
        kernel_size.w() = ndim > 1 ? mxnet_kernel[1] : 1;
    };
    optAttrProcs["stride"] = [&](const std::string& strVal) {
        auto mxnet_stride = Str2Vector<int>(strVal);
        int ndim = mxnet_stride.size();
        strides.h() = mxnet_stride[0];
        strides.w() = ndim > 1 ? mxnet_stride[1] : 1;
    };
    optAttrProcs["dilate"] = [&](const std::string& strVal) {
        auto mxnet_dilate = Str2Vector<int>(strVal);
        int ndim = mxnet_dilate.size();
        dilations.h() = mxnet_dilate[0];
        dilations.w() = ndim > 1 ? mxnet_dilate[1] : 1;
    };
    optAttrProcs["pad"] = [&](const std::string& strVal) {
        auto mxnet_pad = Str2Vector<int>(strVal);
        int ndim = mxnet_pad.size();
        beg_padding.h() = mxnet_pad[0];
        beg_padding.w() = ndim > 1 ? mxnet_pad[1] : 1;
        end_padding.h() = mxnet_pad[0];
        end_padding.w() = ndim > 1 ? mxnet_pad[1] : 1;
    };
    reqAttrProcs["num_filter"] = [&](const std::string& strVal) {
        int mxnet_num_filter = Str2Num<int>(strVal);
        noutput = mxnet_num_filter;
    };
    optAttrProcs["num_group"] = [&](const std::string& strVal) {
        auto mxnet_num_group = Str2Num<int>(strVal);
        ngroup = mxnet_num_group;
    };
    optAttrProcs["no_bias"] = [&](const std::string& strVal) {
        auto mxnet_no_bias = Str2Bool<bool>(strVal);
        no_bias = mxnet_no_bias;
    };

    ProcAttrs(node, reqAttrProcs, true);
    ProcAttrs(node, optAttrProcs, false);

//    ASSERT(kernel_weights.shape.d[1] * ngroup == nchan, ErrorCode::kINVALID_NODE);
    ASSERT((inputs.size() == 3 && !no_bias) || no_bias, ErrorCode::kINVALID_NODE);
//    ASSERT(noutput == kernel_weights.shape.d[0], ErrorCode::kINVALID_NODE);
    ASSERT(kernel_size.h() == kernel_weights.shape.d[2], ErrorCode::kINVALID_NODE);
    ASSERT(kernel_size.w() == kernel_weights.shape.d[3], ErrorCode::kINVALID_NODE);
    nvinfer1::IDeconvolutionLayer* layer
            = ctx->network()->addDeconvolution(*tensor_ptr, noutput, kernel_size, kernel_weights, bias_weights);

    ASSERT(layer, ErrorCode::kUNSUPPORTED_NODE);
    layer->setStride(strides);
    layer->setPaddingMode(paddingMode);
    layer->setPrePadding(beg_padding);
    layer->setPostPadding(end_padding);
    layer->setNbGroups(ngroup);
    tensor_ptr = layer->getOutput(0);
    dims = tensor_ptr->getDimensions();
    if (need_to_expand_dims)
    {
        // Un-expand spatial dims back to 1D
        nvinfer1::Dims new_shape{2, {dims.d[0], dims.d[1]}};
        tensor_ptr = reshape_tensor(ctx, *tensor_ptr, new_shape);
        ASSERT(tensor_ptr, ErrorCode::kUNSUPPORTED_NODE);
    }
    return {{tensor_ptr}};
}

DEFINE_BUILTIN_OP_IMPORTER(_equal) {
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);
    ASSERT(inputs.at(1).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);

    auto plug = arctern::trtPlugin::createEqualPlugin(node.strName.c_str());
    std::vector<nvinfer1::ITensor *> const &plugin_inputs = {&inputs.at(0).tensor(), &inputs.at(1).tensor()};
    RETURN_FIRST_OUTPUT(ctx->network()->addPluginV2(plugin_inputs.data(), plugin_inputs.size(), *plug));
}

DEFINE_BUILTIN_OP_IMPORTER(_mod_scalar)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);

    int scalar = 0;
    // parser attrs
    AttrProcMap optAttrProcs;

    optAttrProcs["scalar"] = [&](const std::string& strVal) {
        auto scalarVal = Str2Num<int>(strVal);
        scalar = scalarVal;
    };

    ProcAttrs(node, optAttrProcs, true);

    int mode = 0;
    auto plug = arctern::trtPlugin::createModScalarPlugin(node.strName.c_str(), scalar, mode);
    std::vector<nvinfer1::ITensor*> const& plugin_inputs = {&inputs.at(0).tensor()};
    RETURN_FIRST_OUTPUT(ctx->network()->addPluginV2(plugin_inputs.data(), plugin_inputs.size(), *plug));
}

DEFINE_BUILTIN_OP_IMPORTER(_div_scalar)
{
    ASSERT(inputs.at(0).is_tensor(), ErrorCode::kUNSUPPORTED_NODE);

    int scalar = 0;
    // parser attrs
    AttrProcMap optAttrProcs;

    optAttrProcs["scalar"] = [&](const std::string& strVal) {
        auto scalarVal = Str2Num<int>(strVal);
        scalar = scalarVal;
    };

    ProcAttrs(node, optAttrProcs, true);

    int mode = 1;
    auto plug = arctern::trtPlugin::createModScalarPlugin(node.strName.c_str(), scalar, mode);
    std::vector<nvinfer1::ITensor*> const& plugin_inputs = {&inputs.at(0).tensor()};
    RETURN_FIRST_OUTPUT(ctx->network()->addPluginV2(plugin_inputs.data(), plugin_inputs.size(), *plug));
}

DEFINE_BUILTIN_OP_IMPORTER(add_n){
    return combineTensorsElementwise(ctx, node, inputs, nvinfer1::ElementWiseOperation::kSUM, true);
}

} // namespace trtParser

} // namespace arctern

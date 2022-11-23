#include <sstream>

#include "mxnetParser.hpp"
#include "mxnetToTrtUtils.hpp"
#include "mxnetModelParser.hpp"

namespace arctern
{
namespace trtParser
{
bool strategyFlattenFullyConnectedReLU(MxnetSymbol& symbol, MxnetNode const& node, size_t current_node_idx)
{
    bool skip = false;
    if (node.strOp == "Flatten")
    {
        skip = true;
        /// 检查本层输出是否为FullyConnected层的输入
        std::vector<size_t> output_idx;
        for (size_t i = 0; i < node.outputs.size(); ++i)
        {
            auto check_node = symbol.first.at(node.outputs[i].first);
            if (check_node.strOp == "FullyConnected")
            {
                output_idx.push_back(i);
            }
        }

        if (output_idx.size() < node.outputs.size() || 0 == node.outputs.size())
        {
            skip = false;
        }

        /// 如果FullyConnected层的flatten属性为true，直接修改FullyConnected的输入为Flatten层的输入
        for (unsigned long idx : output_idx)
        {
            auto& check_node = symbol.first.at(node.outputs[idx].first);
            bool is_flatten = true;
            std::string strVal = check_node.attrs.GetValue("flatten", false);
            if (!strVal.empty())
            {
                is_flatten = Str2Bool<bool>(strVal);
            }

            if (is_flatten)
            {
                check_node.inputs[node.outputs[idx].second] = node.inputs[0];
            }
            else
            {
                skip = false;
            }
        }
    }
    else if (node.strOp == "FullyConnected")
    {
        skip = true;
        /// 检查本层输出是否为Activation层的输入
        std::vector<size_t> output_idx;
        for (size_t i = 0; i < node.outputs.size(); ++i)
        {
            auto check_node = symbol.first.at(node.outputs[i].first);
            if (check_node.strOp == "Activation")
            {
                output_idx.push_back(i);
            }
        }

        if (output_idx.empty() || (output_idx.size() < node.outputs.size()))
        {
            skip = false;
        }

        for (unsigned long idx : output_idx)
        {
            auto check_node = symbol.first.at(node.outputs[idx].first);
            std::string strVal = check_node.attrs.GetValue("act_type", true);

            if (strVal != "relu")
            {
                skip = false;
            }
        }
    }
    else if (node.strOp == "Activation")
    {
        std::string strVal = node.attrs.GetValue("act_type", true);
        if (strVal == "relu")
        {
            MxnetNode input_node = symbol.first.at(node.inputs[0].first);
            if (input_node.strOp == "FullyConnected")
            {
                symbol.first.at(current_node_idx).strOp = std::string("FullyConnectedReLUFusion");
                symbol.first.at(current_node_idx).strName = input_node.strName + "_fuse_" + node.strName;
                symbol.first.at(current_node_idx).inputs = input_node.inputs;
                symbol.first.at(current_node_idx).attrs = input_node.attrs;
            }
        }
    }

    return skip;
}

bool strategyGridGeneratorBilinearSampler(MxnetSymbol& symbol, MxnetNode const& node, size_t current_node_idx)
{
    bool skip = false;
    if (node.strOp == "GridGenerator")
    {
        skip = true;
        /// 检查本层输出是否为BilinearSampler层的输入
        std::vector<size_t> output_idx;
        for (size_t i = 0; i < node.outputs.size(); ++i)
        {
            auto check_node = symbol.first.at(node.outputs[i].first);
            if (check_node.strOp == "BilinearSampler")
            {
                output_idx.push_back(i);
            }
        }

        if (output_idx.empty() || (output_idx.size() < node.outputs.size()))
        {
            skip = false;
        }
    }
    else if (node.strOp == "BilinearSampler")
    {
        MxnetNode input_node = symbol.first.at(node.inputs[1].first);
        /// {GridGenerator, BilinearSampler}组合为SpatialTransformer
        if (input_node.strOp == "GridGenerator")
        {
            symbol.first.at(current_node_idx).strOp = std::string("SpatialTransformer");
            symbol.first.at(current_node_idx).strName = input_node.strName + "_fuse_" + node.strName;
            symbol.first.at(current_node_idx).inputs[1] = input_node.inputs[0];
            symbol.first.at(current_node_idx).attrs = input_node.attrs;
            symbol.first.at(current_node_idx).attrs.push_back(std::make_pair("sampler_type", "bilinear"));
        }
    }

    return skip;
}

MxnetNodeImportResult MxnetParser::importNode(MxnetNode const& node, std::vector<TensorOrWeights>& inputs)
{
    if (!_op_importers.count(node.strOp))
    {
        return MAKE_ERROR("No importer registered for op: " + node.strOp, ErrorCode::kUNSUPPORTED_NODE);
    }
    MxnetNodeImporter const& node_importer = _op_importers.at(node.strOp);

    std::vector<TensorOrWeights> outputs;
    GET_VALUE(node_importer(&_importer_ctx, node, inputs), &outputs);

    for (size_t i = 0; i < outputs.size(); ++i)
    {
        std::string node_output_name = (outputs.size() == 1)? node.strName : node.strName + std::to_string(i);
        TensorOrWeights& output = outputs.at(i);
        if (output)
        {
            if (output.is_tensor())
            {
                output.tensor().setName(node_output_name.c_str());
            }
        }
    }
    return outputs;
}

Status MxnetParser::importModel(MxnetSymbol& symbol, std::vector<MxnetParam>& params, nvinfer1::Dims inputDims)
{
    string_map<std::vector<TensorOrWeights>> tensors;
    for (size_t node_idx = 0; node_idx < symbol.first.size(); node_idx++)
    {
        _current_node = node_idx;
        auto const& node = symbol.first.at(node_idx);

        // handle op "null"
        if (node.strOp == "null")
        {
            // TODO : only support input node's name is "data"
            std::vector<TensorOrWeights> tensor(1);
            if (node.strName == "data")
            {
                // TODO : covert data_type
                nvinfer1::DataType trt_dtype = nvinfer1::DataType::kFLOAT;
                nvinfer1::ITensor* layer = nullptr;
                ASSERT_INPUT(layer = _importer_ctx.network()->addInput(node.strName.c_str(), trt_dtype, inputDims),
                    ErrorCode::kUNSUPPORTED_NODE, node.strName);
                tensor[0] = layer;
            }
            else
            {
                auto param = std::find_if(params.begin(), params.end(),
                    [&](const MxnetParam& param) { return param.strName == node.strName; });
                if (param != params.end())
                {
                    ShapedWeights weights;
                    ASSERT_INPUT(convert_mxnet_weights(*param, &weights), ErrorCode::kUNSUPPORTED_NODE, node.strName);
                    tensor[0] = weights;
                }
                else
                {
                    tensor[0] = ShapedWeights::empty(nvinfer1::DataType::kFLOAT);
                }
            }
            tensors.insert({node.strName, tensor});

            continue;
        }

        if (strategyFlattenFullyConnectedReLU(symbol, node, _current_node))
        {
            continue;
        }

        if (strategyGridGeneratorBilinearSampler(symbol, node, _current_node))
        {
            continue;
        }

        std::vector<TensorOrWeights> inputs;
        for (auto const& input_indices : node.inputs)
        {
            auto input_name = symbol.first.at(input_indices.first).strName;
            ASSERT(tensors.count(input_name), ErrorCode::kINVALID_GRAPH);
            inputs.push_back(tensors.at(input_name).at(input_indices.second));
        }
        std::vector<TensorOrWeights> outputs;
        GET_VALUE(this->importNode(node, inputs), &outputs);
        std::string node_output_name = node.strName;
        ASSERT(!tensors.count(node_output_name), ErrorCode::kINVALID_GRAPH);
        tensors.insert({node_output_name, outputs});
        if (!outputs.empty())
        {
            std::stringstream ss;
            ss << node.strName << ":" << node.strOp << " -> ";
            for (auto output : outputs)
            {
                ss << output.shape();
            }
            _importer_ctx.logger().log(nvinfer1::ILogger::Severity::kINFO, ss.str().c_str());
        }
    }

    ///import output nodes
    for (size_t node_idx = 0; node_idx < symbol.second.size(); node_idx++)
    {
        auto const& node = symbol.second.at(node_idx);
        auto output_name = symbol.first.at(node.first).strName;
        ASSERT(tensors.count(output_name), ErrorCode::kINVALID_GRAPH);
        nvinfer1::ITensor* output_tensor_ptr = &tensors.at(output_name).at(node.second).tensor();
        _importer_ctx.network()->markOutput(*output_tensor_ptr);
    }

    return Status::success();
}

bool MxnetParser::parse(MxnetSymbol& symbol, std::vector<MxnetParam>& params, nvinfer1::Dims inputDims)
{
    Status status = this->importModel(symbol, params, inputDims);
    if (status.is_error())
    {
        status.setNode(_current_node);
        _errors.push_back(status);
        return false;
    }
    return true;
}

bool MxnetParser::parseFromFile(const char* symbol, const void* params, nvinfer1::Dims& inputDims)
{
    mxnetSymbol_ = ParseMxnetJson(symbol);
    mxnetParams_ = LoadMxnetParam(params);
    return parse(mxnetSymbol_, mxnetParams_, inputDims);
}

} // namespace trtParser
} // namespace arctern
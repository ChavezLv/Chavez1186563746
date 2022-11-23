#include "mxnetModelParser.hpp"
#include <sstream>

#include "jsonHelper.hpp"

namespace arctern
{
namespace trtParser
{
MxnetNode ParseMxnetNode(const Json::iterator& jNode)
{
    MxnetNode node;
    for (Json::iterator jField = jNode->begin(); jField != jNode->end(); ++jField)
    {
        if (jField.key() == "op")
        {
            node.strOp = jField.value();
        }
        else if (jField.key() == "name")
        {
            node.strName = jField.value();
        }
        else if (jField.key() == "attr" || jField.key() == "attrs" || jField.key() == "param")
        {
            node.attrs = ParseArray<StringPair>(
                jField, [](Json::iterator jAttr) { return std::make_pair(jAttr.key(), jAttr.value()); });
        }
        else if (jField.key() == "inputs")
        {
            node.inputs = ParseArray<MxnetInput>(jField, [](Json::iterator jInput) {
                assert(jInput->is_array());
                auto inputIndices = ParseArray<size_t>(jInput);
                assert(inputIndices.size() == 3U || inputIndices.size() == 2U);
                return std::make_pair(inputIndices[0], inputIndices[1]);
            });
        }
    }
    return std::move(node);
}

MxnetSymbol ParseMxnetJson(const char* symbol)
{
    Json jModel;
    std::stringstream(symbol) >> jModel;

    std::vector<MxnetNode> nodes;
    std::vector<MxnetInput> headIndices;
    std::vector<size_t> argIndices;
    for (Json::iterator jField = jModel.begin(); jField != jModel.end(); ++jField)
    {
        if (jField.key() == "nodes")
        {
            nodes = ParseArray<MxnetNode>(jField, ParseMxnetNode);
        }
        else if (jField.key() == "heads")
        {
            headIndices = ParseArray<MxnetInput>(jField, [](Json::iterator jInput) {
                assert(jInput->is_array());
                auto inputIndices = ParseArray<size_t>(jInput);
                assert(inputIndices.size() == 3U || inputIndices.size() == 2U);
                return std::make_pair(inputIndices[0], inputIndices[1]);
            });
        }
        else if (jField.key() == "arg_nodes")
        {
            argIndices = ParseArray<size_t>(jField);
        }
        else if (jField.key() == "attrs")
        {
        }
        else if (jField.key() == "node_row_ptr")
        {
        }
    }
    for (auto iArgIdx : argIndices)
    {
        assert(nodes[iArgIdx].strOp == "null");
    }

    for (size_t node_idx = 0; node_idx < nodes.size(); ++node_idx)
    {
        auto node = nodes[node_idx];
        for (size_t input_idx = 0; input_idx < node.inputs.size(); ++input_idx)
        {
            auto indices = node.inputs[input_idx].first;
            nodes[indices].outputs.emplace_back(node_idx, input_idx);
        }
    }

    return std::make_pair(std::move(nodes), std::move(headIndices));
}

std::vector<MxnetParam> LoadMxnetParam(const void* params)
{
    std::vector<MxnetParam> vParams;
    uint64_t offset = 0;

    uint64_t *header = (uint64_t*)((uint8_t*)params + offset);
    offset += sizeof(uint64_t);
    uint64_t *reserved = (uint64_t*)((uint8_t*)params + offset);
    offset += sizeof(uint64_t);

    uint64_t *data_count = (uint64_t*)((uint8_t*)params + offset);
    offset += sizeof(uint64_t);
    for (int i = 0; i < (int)(*data_count); i++)
    {
        uint32_t *magic = (uint32_t*)((uint8_t*)params + offset); /// 0xF993FAC9
        offset += sizeof(uint32_t);
        /// shape
        uint32_t *ndim = nullptr;
        Shape shape;
        if (*magic == 0xF993FAC9)
        {
            int32_t *stype = (int32_t*)((uint8_t*)params + offset);
            offset += sizeof(int32_t);
            ndim = (uint32_t*)((uint8_t*)params + offset);
            offset += sizeof(uint32_t);
            shape.resize(*ndim);

            for(int j = 0; j < (int)(*ndim); ++j){
                int64_t *val = (int64_t*)((uint8_t*)params + offset);
                offset += sizeof(int64_t);
                shape[j] = *val;
            }
        }
        else if (*magic == 0xF993FAC8)
        {
            ndim = (uint32_t*)((uint8_t*)params + offset);
            offset += sizeof(uint32_t);
            shape.resize(*ndim);

            for(int j = 0; j < (int)(*ndim); ++j){
                int64_t *val = (int64_t*)((uint8_t*)params + offset);
                offset += sizeof(int64_t);
                shape[j] = *val;
            }
        }
        else
        {
            ndim = magic;
            shape.resize(*ndim);

            for(int j = 0; j < (int)(*ndim); ++j){
                uint32_t *val = (uint32_t*)((uint8_t*)params + offset);
                offset += sizeof(uint32_t);
                shape[j] = *val;
            }
        }

        // context
        int32_t *dev_type = (int32_t*)((uint8_t*)params + offset);
        offset += sizeof(int32_t);
        int32_t *dev_id = (int32_t*)((uint8_t*)params + offset);
        offset += sizeof(int32_t);

        int32_t *type_flag = (int32_t*)((uint8_t*)params + offset);
        offset += sizeof(int32_t);

        // data
        size_t len = 0;
        if (shape.size() == 1)
            len = shape[0];
        if (shape.size() == 2)
            len = shape[0] * shape[1];
        if (shape.size() == 3)
            len = shape[0] * shape[1] * shape[2];
        if (shape.size() == 4)
            len = shape[0] * shape[1] * shape[2] * shape[3];

        MxnetParam p;
        p.shape = shape;
        p.data.resize(len);

        for(int j = 0; j < len; ++j){
            float *val = (float*)((uint8_t*)params + offset);
            offset += sizeof(float);
            p.data[j] = *val;
        }
        vParams.emplace_back(std::move(p));
    }

    uint64_t *name_count = (uint64_t*)((uint8_t*)params + offset);
    offset += sizeof(uint64_t);
    for (int i = 0; i < (int)(*name_count); i++)
    {
        uint64_t *len = (uint64_t*)((uint8_t*)params + offset);
        offset += sizeof(uint64_t);
        MxnetParam& p = vParams[i];
        p.strName.resize(*len);
        for(int j = 0; j < (int)(*len); ++j){
            char* c = (char*)((uint8_t*)params + offset);
            offset += sizeof(char);
            p.strName[j] = *c;
        }

        if (memcmp(p.strName.c_str(), "arg:", 4) == 0)
        {
            p.strName = std::string(p.strName.c_str() + 4);
        }
        if (memcmp(p.strName.c_str(), "aux:", 4) == 0)
        {
            p.strName = std::string(p.strName.c_str() + 4);
        }
    }

    return std::move(vParams);
}

} // namespace trtParser
} // namespace arctern
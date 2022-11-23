#pragma once

#include "attributes.hpp"
#include <map>
#include <string>
#include <vector>

namespace arctern
{
namespace trtParser
{

using MxnetInput = std::pair<size_t, size_t>;

/** \struct MxnetNode
 *
 * \brief Mxnet node struct
 */
struct MxnetNode
{
    std::string strName;             // node namee
    std::string strOp;               // node op
    std::vector<MxnetInput> inputs;  // node input indexes
    std::vector<MxnetInput> outputs; // node output indexes
    Attributes attrs;                // node attributes
};

/** \struct MxnetParam
 *
 * \brief Mxnet params
 */
struct MxnetParam
{
    std::string strName; // param name
    Shape shape;         // param shape
    // TODO: to support other data type
    std::vector<float> data; // param data
};

using MxnetSymbol = std::pair<std::vector<MxnetNode>, std::vector<MxnetInput>>;

MxnetSymbol ParseMxnetJson(const char* symbol);

std::vector<MxnetParam> LoadMxnetParam(const void* params);

} // namespace trtParser
} // namespace arctern

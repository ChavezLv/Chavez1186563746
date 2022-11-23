#pragma once

#include "src/common/json.hpp"

using Json = nlohmann::json;

namespace arctern
{
namespace trtParser
{
template <typename _Ty>
std::vector<_Ty> ParseArray(Json::iterator& jAry)
{
    std::vector<_Ty> ary;
    for (Json::iterator i = jAry->begin(); i != jAry->end(); ++i)
    {
        ary.push_back(i->get<_Ty>());
    }
    return std::move(ary);
}

template <typename _Ty>
std::vector<_Ty> ParseArray(const Json::iterator& jAry, std::function<_Ty(Json::iterator)> elemProc)
{
    std::vector<_Ty> ary;
    for (Json::iterator i = jAry->begin(); i != jAry->end(); ++i)
    {
        ary.push_back(elemProc(i));
    }
    return std::move(ary);
}
} // namespace trtParser
} // namespace arctern

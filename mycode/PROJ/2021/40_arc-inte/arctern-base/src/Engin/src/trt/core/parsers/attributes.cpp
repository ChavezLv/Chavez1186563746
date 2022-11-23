#include "attributes.hpp"
#include <algorithm>

namespace arctern
{
namespace trtParser
{

Attributes::Attributes(const std::vector<StringPair>& baseObj)
    : std::vector<StringPair>(baseObj)
{
}

std::string Attributes::GetValue(const std::string& strKey, bool bRequired) const
{
    auto iAttr = _Find(strKey);
    if (iAttr == end())
    {
        if (bRequired)
        {
            //LOG(FATAL) << "Key " << strKey << " not found";
        }
        return std::string();
    }
    return iAttr->second;
}

bool Attributes::HasValue(const std::string& strKey) const
{
    return (_Find(strKey) != end());
}

bool Attributes::RemoveValue(const std::string& strKey)
{
    auto iAttr = _Find(strKey);
    if (iAttr == end())
    {
        return false;
    }
    erase(iAttr);
    return true;
}

std::vector<StringPair>::iterator Attributes::_Find(const std::string& strKey)
{
    return std::find_if(begin(), end(), [&](const StringPair& attr) { return attr.first == strKey; });
}

std::vector<StringPair>::const_iterator Attributes::_Find(const std::string& strKey) const
{
    return std::find_if(cbegin(), cend(), [&](const StringPair& attr) { return attr.first == strKey; });
}

} // namespace trtParser
} // namespace arctern
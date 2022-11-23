#pragma once

#include "common.hpp"

namespace arctern
{
namespace trtParser
{

/** \class Attributes
 * 
 * \brief mxnet node attributes object
 */
class Attributes : public std::vector<StringPair>
{
public:
    Attributes() = default;
    Attributes(const std::vector<StringPair>& baseObj);
    std::string GetValue(const std::string& strKey, bool bRequired) const;
    bool HasValue(const std::string& strKey) const;
    bool RemoveValue(const std::string& strKey);

private:
    std::vector<StringPair>::iterator _Find(const std::string& strKey);
    std::vector<StringPair>::const_iterator _Find(const std::string& strKey) const;
};

} // namespace trtParser
} // namespace arctern
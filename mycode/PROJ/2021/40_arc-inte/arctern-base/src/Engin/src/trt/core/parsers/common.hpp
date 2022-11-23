#pragma once

#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <unordered_map>

namespace arctern
{
namespace trtParser
{

#define CUASSERT(status_)                                                                                              \
    {                                                                                                                  \
        auto s_ = status_;                                                                                             \
        if (s_ != cudaSuccess)                                                                                         \
        {                                                                                                              \
            std::cerr << __FILE__ << ", " << __LINE__ << ", " << s_ << ", " << cudaGetErrorString(s_) << std::endl;    \
        }                                                                                                              \
    }
    
using Shape = std::vector<size_t>;
using StringPair = std::pair<std::string, std::string>;

template <typename T>
using string_map = std::unordered_map<std::string, T>;

struct InferDeleter
{
    template <typename T>
    void operator()(T* obj) const
    {
        if (obj)
        {
            obj->destroy();
        }
    }
};

template <typename T>
inline std::shared_ptr<T> infer_object(T* obj)
{
    if (!obj)
    {
        throw std::runtime_error("Failed to create object");
    }
    return std::shared_ptr<T>(obj, InferDeleter());
}

} // namespace trtParser
} // namespace arctern

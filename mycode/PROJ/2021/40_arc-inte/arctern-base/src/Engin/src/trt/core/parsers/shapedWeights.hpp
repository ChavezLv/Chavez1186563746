#pragma once

#include <NvInfer.h>
#include <vector>

namespace arctern
{
namespace trtParser
{
/** \class ShapedWeights
 * 
 * \brief 
 */
class ShapedWeights
{
public:
    typedef nvinfer1::DataType DataType;
    DataType type;
    void* values;
    nvinfer1::Dims shape;
    /**
     * \brief return empty weights
     * 
     * \param type weights datatype
     */
    static ShapedWeights empty(DataType type);
    ShapedWeights();
    /**
     * \brief Construct a new Shaped Weights object
     * 
     * \param type weight data type
     * \param values weight data pointer
     * \param shape weight data shape
     */
    explicit ShapedWeights(DataType type, void* values, nvinfer1::Dims shape);
    size_t count() const;
    size_t size_bytes() const;
    operator bool() const;
    operator nvinfer1::Weights() const;
};

bool transposeWeights(ShapedWeights const& weights, nvinfer1::Permutation const& perm, ShapedWeights* result);

bool convertINT64(void* weightValues, const size_t nbWeights, std::vector<int32_t>& converted_weights);

} // namespace trtParser
} // namespace arctern
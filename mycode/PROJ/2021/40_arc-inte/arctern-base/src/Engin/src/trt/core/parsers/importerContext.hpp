#pragma once

#include <list>
#include <unordered_map>

#include "shapedWeights.hpp"

namespace arctern
{
namespace trtParser
{
/** \class ImporterContext
 *
 * \brief importer context
 */
class ImporterContext
{
public:
    ImporterContext(nvinfer1::INetworkDefinition* network, nvinfer1::ILogger* logger)
        : _network(network)
        , _logger(logger)
    {
    }
    /**
     * \brief return network definition
     */
    nvinfer1::INetworkDefinition* network()
    {
        return _network;
    }
    /**
     * \brief return logger
     */
    nvinfer1::ILogger& logger()
    {
        return *_logger;
    }
    /**
     * \brief Create a Temp Weights object
     * 
     * \param type DataType
     * \param shape Data shape
     * \return  ShapedWeights
     */
    ShapedWeights createTempWeights(ShapedWeights::DataType type, nvinfer1::Dims shape)
    {
        ShapedWeights weights(type, nullptr, shape);
        _temp_bufs.emplace_back(weights.size_bytes());
        weights.values = _temp_bufs.back().data();
        return weights;
    }
    /**
     * \brief Set the User Input object
     * 
     * \param name node name
     * \param input input tensor 
     */
    bool setUserInput(const char* name, nvinfer1::ITensor* input)
    {
        _user_inputs[name] = input;
        return true;
    }
    /**
     * \brief Set the User Output object
     * 
     * \param name node name
     * \param output output tensor
     */
    bool setUserOutput(const char* name, nvinfer1::ITensor** output)
    {
        _user_outputs[name] = output;
        return true;
    }
    /**
     * \brief Get the User Input object
     * 
     * \param name node name
     */
    nvinfer1::ITensor* getUserInput(const char* name)
    {
        if (!_user_inputs.count(name))
        {
            return nullptr;
        }
        else
        {
            return _user_inputs.at(name);
        }
    }
    /**
     * \brief Get the User Output object
     * 
     * \param name node name
     */
    nvinfer1::ITensor** getUserOutput(const char* name)
    {
        if (!_user_outputs.count(name))
        {
            return nullptr;
        }
        else
        {
            return _user_outputs.at(name);
        }
    }
    /**
     * \brief Get the User Outputs object
     */
    std::unordered_map<std::string, nvinfer1::ITensor**> const& getUserOutputs() const
    {
        return _user_outputs;
    }

private:
    nvinfer1::INetworkDefinition* _network;
    nvinfer1::ILogger* _logger;
    std::list<std::vector<uint8_t>> _temp_bufs;
    std::unordered_map<std::string, nvinfer1::ITensor*> _user_inputs;
    std::unordered_map<std::string, nvinfer1::ITensor**> _user_outputs;
};

} // namespace trtParser
} // namespace arctern
#pragma once

#include "mxnetOpImporters.hpp"

namespace arctern
{
namespace trtParser
{
/** \class MxnetParser
 * \brief an object for parsing Mxnet models into a TensorRT network definition
 */
class MxnetParser final : public Parser
{
public:
    MxnetParser(nvinfer1::INetworkDefinition* network, nvinfer1::ILogger* logger)
        : _op_importers(getMxnetOpImporterMap())
        , _importer_ctx(network, logger){}

    /**
     * \brief Parse Mxnet symbol and params files to TensorRT network
     * \param symbol  mxnet symbol
     * \param params  mxnet params
     * \param inputDims Model input shape
     * \return true if parse success
     */
    bool parseFromFile(const char* symbol, const void* params, nvinfer1::Dims& inputDims) override;

    /**
     * \brief Destroy this object
     */
    void destroy() override
    {
        delete this;
    }
    /**
     * \brief Get the Nb Errors object
     */
    int getNbErrors() const override
    {
        return _errors.size();
    }
    /**
     * \brief Get the Error object
     * \param index Error index
     */
    ParserError const* getError(int index) const override
    {
        assert(0 <= index && index < (int) _errors.size());
        return &_errors[index];
    }
    /**
     * \brief Clear the Error object
     */
    void clearErrors() override
    {
        _errors.clear();
    }

private:
    string_map<MxnetNodeImporter> _op_importers;
    ImporterContext _importer_ctx;
    int _current_node{-1};
    std::vector<Status> _errors;
    MxnetSymbol mxnetSymbol_;
    std::vector<MxnetParam> mxnetParams_;

    /**
     * \brief parse mxnet symbol and params to TensorRT network
     *
     * \param symbol mxnet symbol
     * \param params mxnet params
     * \param inputDims mxnet model input shape
     * \return true if parse success
     */
    bool parse(MxnetSymbol& symbol, std::vector<MxnetParam>& params, nvinfer1::Dims inputDims);
    /**
     * \brief import mxnet symbol and params to TensorRT network
     *
     * \param symbol mxnet symbol
     * \param params mxnet params
     * \param inputDims mxnet model input shape
     * \return Status
     */
    Status importModel(MxnetSymbol& symbol, std::vector<MxnetParam>& params, nvinfer1::Dims inputDims);
    /**
     * \brief import mxnet node
     *
     * \param node mxnet node
     * \param inputs node inputs
     * \return  MxnetNodeImportResult
     */
    MxnetNodeImportResult importNode(MxnetNode const& node, std::vector<TensorOrWeights>& inputs);
};
} // namespace trtParser
} // namespace arctern

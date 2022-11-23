/**
 * \file parser.h
 * \author zengzuoqi (zeng.zuoqi@intellif.com)
 * \brief
 * \version 0.1
 * \date 2019-08-09
 *
 * @copyright Copyright (c) 2019
 *
 */
#pragma once

#include <ctime>
#include <iostream>
#include "NvInfer.h"

namespace arctern
{
namespace trtParser
{

/** \class TRT_Logger
 * \brief Logger for TensorRT info/warning/errors
 */
class TRT_Logger : public nvinfer1::ILogger
{
public:
    TRT_Logger(Severity verbosity = Severity::kWARNING, std::ostream& ostream = std::cout)
        : _verbosity(verbosity)
        , _ostream(&ostream){}
        
    void log(Severity severity, const char* msg) override
    {
        if (severity <= _verbosity)
        {
            time_t rawtime = std::time(0);
            char buf[256];
            strftime(&buf[0], 256, "%Y-%m-%d %H:%M:%S", std::gmtime(&rawtime));
            const char* sevstr = (severity == Severity::kINTERNAL_ERROR ? "   BUG"
                                 : severity == Severity::kERROR ? "   ERROR"
                                 : severity == Severity::kWARNING ? "   WARNING"
                                 : severity == Severity::kINFO ? "   INFO" : "   UNKNOWN");
            (*_ostream) << "[" << buf << " " << sevstr << "] " << msg << std::endl;
        }
    }

private:
    nvinfer1::ILogger::Severity _verbosity;
    std::ostream* _ostream;
};

/**\brief: the deletor of auto-pointer
 * */
template <typename T>
struct Destroy
{
    void operator()(T* t) const
    {
        t->destroy();
    }
};

/** \enum ErrorCode
 * \brief the type of parser error
 */
enum class ErrorCode : int
{
    kSUCCESS = 0,
    kINTERNAL_ERROR = 1,
    kMEM_ALLOC_FAILED = 2,
    kMODEL_DESERIALIZE_FAILED = 3,
    kINVALID_VALUE = 4,
    kINVALID_GRAPH = 5,
    kINVALID_NODE = 6,
    kUNSUPPORTED_GRAPH = 7,
    kUNSUPPORTED_NODE = 8
};

/** \class ParserError
 * \brief an object containing information about an error
 */
class ParserError
{
public:
    /**
     * \brief the error code
     */
    virtual ErrorCode code() const = 0;
    /**
     * \brief description of the error
     */
    virtual const char* desc() const = 0;
    /**
     * \brief source function in which the error occurred
     */
    virtual const char* file() const = 0;
    /**
     * \brief source line at which the error occurred
     */
    virtual int line() const = 0;
    /**
     * \brief source function in which the error occurred
     */
    virtual const char* func() const = 0;
    /**
     * \brief index of the model node in which the error occurred
     */
    virtual int node() const = 0;

protected:
    virtual ~ParserError() {}
};

/** \class Parser
 * \brief an object for parsing models into a TensorRT network definition
 */
class Parser
{
public:
    /**
     * \brief Parse a mxnet model file
     * \param symbol  mxnet symbol
     * \param params  mxnet params
     * \param inputDims input dims
     * \return true if the model was parsed successfully
     */
    virtual bool parseFromFile(const char* symbol, const void* params, nvinfer1::Dims& inputDims) = 0;
    /**
     * \brief destroy this object
     */
    virtual void destroy() = 0;
    /**
     * \brief Get the number of errors that occurred during prior calss to \p parseFromFile
     */
    virtual int getNbErrors() const = 0;
    /**
     * \brief Get an error that occurred during prios calls to \p parseFromFile
     * \param index
     */
    virtual ParserError const* getError(int index) const = 0;
    /**
     * \brief Clear errors from prior calls to \p parse
     */
    virtual void clearErrors() = 0;

protected:
    virtual ~Parser() {}
};

/**
 * \brief Create a new parser object
 *
 * \param network The network definition that the parser will write to
 * \param logger The logger to use
 * \return a new parser object or NULL if an error occurred
 * \see Parser
 */
Parser* createParser(nvinfer1::INetworkDefinition& network, nvinfer1::ILogger& logger);

} // namespace trtParser
} // namespace arctern

#include "parser.h"
#include "mxnetParser.hpp"

namespace arctern
{
namespace trtParser
{
Parser* createParser(nvinfer1::INetworkDefinition& network, nvinfer1::ILogger& logger)
{
    return static_cast<Parser*>(new arctern::trtParser::MxnetParser(&network, &logger));
}

} // namespace trtParser
} // namespace arctern
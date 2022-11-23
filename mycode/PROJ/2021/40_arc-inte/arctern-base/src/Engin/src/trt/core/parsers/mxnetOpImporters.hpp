#pragma once

#include <functional>
#include "status.hpp"
#include "importerContext.hpp"
#include "mxnetModelParser.hpp"
#include "tensorOrWeights.hpp"

namespace arctern
{
namespace trtParser
{
typedef ValueOrStatus<std::vector<TensorOrWeights>> MxnetNodeImportResult;

typedef std::function<MxnetNodeImportResult(
    ImporterContext* ctx, MxnetNode const& node, std::vector<TensorOrWeights>& inputs)>
    MxnetNodeImporter;

/**
 * \brief Get the Mxnet Op Importer Map object
 */
string_map<MxnetNodeImporter>& getMxnetOpImporterMap();
} // namespace trtParser
} // namespace arctern

//
// Created by gpu on 19-9-1.
//

#ifndef ARCTERN_CONVERT_TRTMODEL_H
#define ARCTERN_CONVERT_TRTMODEL_H

#include <string>
#include <vector>
#include "src/common/arcterndefs_generated.h"

namespace arctern {
    namespace convert {

        /**\brief convert engine plan of trt to bin with adding some configure info
         * \param trtData: ptr of engine plan data
         * \param trtDataSize: the size of plan data
         * \param version: version of trt model
         * \param batchSize: the max batch size of trt model
         * \param configs_str: the flexible configure info
         * \param options: for flatbuff optional config, it's dynamic range cache for INT8 Model currently
         * \return string contain converted info
         * */
        void convertTrt2Bin(char *trtData, int trtDataSize, std::string &version, int &batchSize,
             const std::string &configs_str, std::vector<std::pair<std::string, std::string>> &options,
             const std::string &savePath);
    } /// namespace convert
}/// namespace arctern

#endif //ARCTERN_CONVERT_TRTMODEL_H

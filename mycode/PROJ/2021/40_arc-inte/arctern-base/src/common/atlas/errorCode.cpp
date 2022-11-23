//
// Created by zy on 8/31/20.
//

#include "atlasLog.h"
#include "errorCode.h"

namespace arctern {

    std::string GetAppErrCodeInfo(const APP_ERROR err) {
      if ((err < APP_ERR_ACL_END) && (err >= APP_ERR_ACL_FAILURE)) {
        return APP_ERR_ACL_LOG_STRING[((err < 0) ? (err + APP_ERR_ACL_END + 1) : err)];
      } else if ((err < APP_ERR_COMM_END) && (err > APP_ERR_COMM_BASE)) {
        return (err - APP_ERR_COMM_BASE) <
               (int) sizeof(APP_ERR_COMMON_LOG_STRING) / (int) sizeof(APP_ERR_COMMON_LOG_STRING[0]) ?
               APP_ERR_COMMON_LOG_STRING[err - APP_ERR_COMM_BASE] :
               "Undefine the error code information";
      } else if ((err < APP_ERR_DVPP_END) && (err > APP_ERR_DVPP_BASE)) {
        return (err - APP_ERR_DVPP_BASE) <
               (int) sizeof(APP_ERR_DVPP_LOG_STRING) / (int) sizeof(APP_ERR_DVPP_LOG_STRING[0]) ?
               APP_ERR_DVPP_LOG_STRING[err - APP_ERR_DVPP_BASE] :
               "Undefine the error code information";
      } else if ((err < APP_ERR_QUEUE_END) && (err > APP_ERR_QUEUE_BASE)) {
        return (err - APP_ERR_QUEUE_BASE) <
               (int) sizeof(APP_ERR_QUEUE_LOG_STRING) / (int) sizeof(APP_ERR_QUEUE_LOG_STRING[0]) ?
               APP_ERR_QUEUE_LOG_STRING[err - APP_ERR_QUEUE_BASE] :
               "Undefine the error code information";
      } else {
        return "Error code unknown";
      }
    }

    void AssertErrorCode(int code, std::string file, std::string function, int line) {
      if (code != APP_ERR_OK) {
        LOGASDError << "Failed at " << file << "->" << function << "->" << line << ": error code=" << code;
        exit(code);
      }
    }

    void CheckErrorCode(int code, std::string file, std::string function, int line) {
      if (code != APP_ERR_OK) {
        LOGASDError << "Failed at " << file << "->" << function << "->" << line << ": error code=" << code;
      }
    }
}
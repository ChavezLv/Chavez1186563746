//
// Created by zy on 8/31/20.
//

#ifndef ARCTERN_ATLASLOG_H
#define ARCTERN_ATLASLOG_H

#include <mutex>
#include <sstream>
#include <string>
#include <vector>

namespace arctern {
    namespace AtlasAscendLog {
        // log level
        enum LogLevels {
            LOG_LEVEL_DEBUG = 0,
            LOG_LEVEL_INFO = 1,
            LOG_LEVEL_WARN = 2,
            LOG_LEVEL_ERROR = 3,
            LOG_LEVEL_FATAL = 4,
            LOG_LEVEL_NONE
        };

        class LogAscend {
        public:
            LogAscend(std::string file, std::string function, int line, uint32_t level);

            ~LogAscend();

            std::ostringstream &Stream();

            // log switch, turn on and off both screen and file log of special level.
            static void LogDebugOn();

            static void LogInfoOn();

            static void LogWarnOn();

            static void LogErrorOn();

            static void LogFatalOn();

            static void LogAllOn();

            static void LogAllOff();

        private:
            std::ostringstream ss_;
            uint32_t myLevel_;
            std::string date_;

            static uint32_t logLevel;
            static std::vector <std::string> levelString;
            static std::mutex mutex;
            static std::string logFile;

        };
    } // namespace AtlasAscendLog

    #define LOGASDDebug AtlasAscendLog::LogAscend(__FILE__, __FUNCTION__, __LINE__, AtlasAscendLog::LOG_LEVEL_DEBUG).Stream()
    #define LOGASDInfo AtlasAscendLog::LogAscend(__FILE__, __FUNCTION__, __LINE__, AtlasAscendLog::LOG_LEVEL_INFO).Stream()
    #define LOGASDWarn AtlasAscendLog::LogAscend(__FILE__, __FUNCTION__, __LINE__, AtlasAscendLog::LOG_LEVEL_WARN).Stream()
    #define LOGASDError AtlasAscendLog::LogAscend(__FILE__, __FUNCTION__, __LINE__, AtlasAscendLog::LOG_LEVEL_ERROR).Stream()
    #define LOGASDFatal AtlasAscendLog::LogAscend(__FILE__, __FUNCTION__, __LINE__, AtlasAscendLog::LOG_LEVEL_FATAL).Stream()
} /// namespace arctern
#endif //ARCTERN_ATLASLOG_H

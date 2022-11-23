//
// Created by zy on 8/31/20.
//
#include "atlasLog.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

namespace arctern {

    /**
    * Create directory recursively
    *
    * @param file target directory to create
    */
    static void CreateDirRecursivelyByFile(const std::string &file) {
      size_t pos = file.rfind('/'); // for linux
      std::string filePath = file.substr(0, pos);
      if (access(filePath.c_str(), 0) != 0) {
        CreateDirRecursivelyByFile(filePath);
        int result = mkdir(filePath.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO); // for linux
        if (result < 0) {
          LOGASDError << "mkdir logs file " << filePath << " fail.";
          return;
        }
      }
    }

    namespace AtlasAscendLog {
        const int TIME_SIZE = 32;
        const int TIME_DIFF = 28800; // 8 hour
        const int BYTES6 = 6;
        uint32_t LogAscend::logLevel = LOG_LEVEL_INFO;
        std::vector<std::string> LogAscend::levelString({"[Debug]", "[Info ]", "[Warn ]", "[Error]", "[Fatal]"});
        std::mutex LogAscend::mutex;
        std::string LogAscend::logFile = "./logs/arctern_atlas_log.log"; // default log file, for linux

        LogAscend::LogAscend(std::string file, std::string function, int line, uint32_t level) {
          myLevel_ = level;
          if (level >= logLevel) {
            struct timeval time = {0, 0};
            gettimeofday(&time, nullptr);
            time_t timep = time.tv_sec + TIME_DIFF;
            struct tm *ptm = gmtime(&timep);
            char timeString[TIME_SIZE] = {0};
            strftime(timeString, TIME_SIZE, "[%F %X:", ptm);
            date_ = timeString;

            ss_.fill('0');
            ss_ << levelString[level] << timeString << std::setw(BYTES6) << time.tv_usec << "]";

            std::string fileName = file.substr(file.rfind('/') + 1); // for linux
            ss_ << "[" << fileName << " " << function << ":" << line << "] ";
          }
        }

        LogAscend::~LogAscend() {
          if (myLevel_ >= logLevel) {
            std::lock_guard<std::mutex> locker(mutex);
            // cout to screen
            std::cout << ss_.str() << std::endl;
            // log to the file
            CreateDirRecursivelyByFile(logFile);
            size_t posDot = logFile.rfind(".");
            std::string dataString = date_.substr(date_.find('[') + 1, date_.find(' ') - date_.find('[') - 1);
            std::string file = logFile.substr(0, posDot) + "_" + dataString + logFile.substr(posDot);
            std::ofstream fs(file, std::ios::app);
            if (!fs) {
              std::cout << "open file " << file << " fail" << std::endl;
              return;
            }
            fs << ss_.str() << std::endl;
            fs.close();
          }
        };

        std::ostringstream &LogAscend::Stream() {
          return ss_;
        }


        void LogAscend::LogDebugOn() {
          logLevel = LOG_LEVEL_DEBUG;
          return;
        }

        void LogAscend::LogInfoOn() {
          logLevel = LOG_LEVEL_INFO;
          return;
        }

        void LogAscend::LogWarnOn() {
          logLevel = LOG_LEVEL_WARN;
          return;
        }

        void LogAscend::LogErrorOn() {
          logLevel = LOG_LEVEL_ERROR;
          return;
        }

        void LogAscend::LogFatalOn() {
          logLevel = LOG_LEVEL_FATAL;
          return;
        }

        void LogAscend::LogAllOn() {
          logLevel = LOG_LEVEL_DEBUG;
          return;
        }

        void LogAscend::LogAllOff() {
          logLevel = LOG_LEVEL_NONE;
          return;
        }

    } // namespace AtlasAscendLog
}
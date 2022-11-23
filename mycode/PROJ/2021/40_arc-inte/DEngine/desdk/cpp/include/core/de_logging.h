/**
 * @file de_logging.h
 * @brief Defines logging macros of DeSdk
 * @author DeSdk Team
 * @date 2019/12/15
 * @copyright Intellif
 */

#ifndef DE_LOGGING_H_
#define DE_LOGGING_H_
#include "de_base.h"
#include "de_c_type.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
#include <chrono>
#include <thread>
#include <mutex>

#include<functional>
#if defined(__linux__) || defined(__CYGWIN__)
#include<sys/time.h>
#endif

#if DE_LOG_STACK_TRACE
#include <cxxabi.h>
#include DE_EXECINFO_H
#endif

namespace de
{
/*!
 * \brief exception class that will be thrown by
 *  default logger if DE_LOG_FATAL_THROW == 1
 */
struct Error : public std::runtime_error
{
    /*!
     * \brief constructor
     * \param s the error message
     */
    explicit Error(const std::string &s) : std::runtime_error(s) {}
};
}  // namespace de

// use a light version of glog
#include <assert.h>
#include <iostream>
#include <sstream>
#include <ctime>

#if defined(_MSC_VER)
#pragma warning(disable : 4722)
#pragma warning(disable : 4068)

#ifdef ERROR
#undef ERROR
#endif
#endif

enum LogLevelSeverity
{
    DEBUG = 0,
    INFO,
    IMPORTANT,
    WARNING,
    ERROR,
    FATAL
};

#ifndef LOG_LEVEL
#define LOG_LEVEL INFO
#endif

#define _DE_DEBUG_0_ 0
#define _DE_DEBUG_1_ 1
#define _DE_DEBUG_2_ 2

namespace de
{
extern DE_DLL LogLevelSeverity g_log_level;
DE_DLL void InitLogLevel(int levlel);

/**
 * @brief 注册LOG回调函数
 * @param user_log_cb 用户提供的LOG回调函数
 */
DE_DLL void SDKSetLogCallBack(std::function<void(std::string&)> user_log_cb);

/**
 * @brief Check if error occurs
 */
class LogCheckError
{
public:
    LogCheckError() : str_(nullptr) {}
    explicit LogCheckError(const std::string& str_) : str_(new std::string(str_)) {}
    LogCheckError(const LogCheckError& other) = delete;
    LogCheckError(LogCheckError&& other) : str_(other.str_)
    {
        other.str_ = nullptr;
    }
    ~LogCheckError()
    {
        if (str_ != nullptr) delete str_;
    }
    operator bool() const
    {
        return str_ != nullptr;
    }
    LogCheckError& operator=(const LogCheckError& other) = delete;
    LogCheckError& operator=(LogCheckError&& other) = delete;
    std::string* str_;
};


#define DEFINE_CHECK_FUNC(name, op)                               \
  template <typename X, typename Y>                               \
  inline LogCheckError LogCheck##name(const X& x, const Y& y) {   \
    if (x op y) return LogCheckError();                           \
    std::ostringstream os;                                        \
    os << " (" << x << " vs. " << y << ") ";  /* CHECK_XX(x, y) requires x and y can be serialized to string. Use CHECK(x OP y) otherwise. NOLINT(*) */ \
    return LogCheckError(os.str());                               \
  }                                                               \
  inline LogCheckError LogCheck##name(int x, int y) {             \
    return LogCheck##name<int, int>(x, y);                        \
  }


#define CHECK_BINARY_OP(name, op, x, y)                               \
  if (de::LogCheckError _check_err = de::LogCheck##name(x, y))    \
    de::LogMessageFatal(__FILE__, __LINE__).stream()                \
      << "Check failed: " << #x " " #op " " #y << *(_check_err.str_) << ": "

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
DEFINE_CHECK_FUNC(_LT, < )
DEFINE_CHECK_FUNC(_GT, > )
DEFINE_CHECK_FUNC(_LE, <= )
DEFINE_CHECK_FUNC(_GE, >= )
DEFINE_CHECK_FUNC(_EQ, == )
DEFINE_CHECK_FUNC(_NE, != )
#pragma GCC diagnostic pop

// Always-on checking
#define CHECK(x)                                           \
  if (!(x))                                                \
    de::LogMessageFatal(__FILE__, __LINE__).stream()     \
      << "Check failed: " #x << ": "
#define CHECK_LT(x, y) CHECK_BINARY_OP(_LT, <, x, y)
#define CHECK_GT(x, y) CHECK_BINARY_OP(_GT, >, x, y)
#define CHECK_LE(x, y) CHECK_BINARY_OP(_LE, <=, x, y)
#define CHECK_GE(x, y) CHECK_BINARY_OP(_GE, >=, x, y)
#define CHECK_EQ(x, y) CHECK_BINARY_OP(_EQ, ==, x, y)
#define CHECK_NE(x, y) CHECK_BINARY_OP(_NE, !=, x, y)
#define CHECK_NOTNULL(x) \
  ((x) == NULL ? de::LogMessageFatal(__FILE__, __LINE__).stream() << "Check  notnull: "  #x << ' ', (x) : (x)) // NOLINT(*)
// Debug-only checking.
#ifdef NDEBUG
#define DCHECK(x) \
  while (false) CHECK(x)
#define DCHECK_LT(x, y) \
  while (false) CHECK((x) < (y))
#define DCHECK_GT(x, y) \
  while (false) CHECK((x) > (y))
#define DCHECK_LE(x, y) \
  while (false) CHECK((x) <= (y))
#define DCHECK_GE(x, y) \
  while (false) CHECK((x) >= (y))
#define DCHECK_EQ(x, y) \
  while (false) CHECK((x) == (y))
#define DCHECK_NE(x, y) \
  while (false) CHECK((x) != (y))
#else
#define DCHECK(x) CHECK(x)
#define DCHECK_LT(x, y) CHECK((x) < (y))
#define DCHECK_GT(x, y) CHECK((x) > (y))
#define DCHECK_LE(x, y) CHECK((x) <= (y))
#define DCHECK_GE(x, y) CHECK((x) >= (y))
#define DCHECK_EQ(x, y) CHECK((x) == (y))
#define DCHECK_NE(x, y) CHECK((x) != (y))
#endif  // NDEBUG

#if DE_LOG_CUSTOMIZE
#define LOG_INFO de::CustomLogMessage()
#else
#define LOG_INFO de::LogMessage(__FILE__, __LINE__)
#endif
#define LOG_DEBUG LOG_INFO
#define LOG_IMPORTANT LOG_INFO
#define LOG_ERROR LOG_INFO
#define LOG_WARNING LOG_INFO
#define LOG_FATAL de::LogMessageFatal(__FILE__, __LINE__)
#define LOG_QFATAL LOG_FATAL

// Poor man version of VLOG
#define VLOG(x) LOG_INFO.stream()

#define LG LOG_INFO.stream()
#define LOG_IF(severity, condition) \
  !(condition) ? (void)0 : de::LogMessageVoidify() & LOG_##severity.stream()
#define LOG(severity) LOG_IF(severity, (severity >= LOG_LEVEL) && (severity >= de::g_log_level))

#ifdef NDEBUG
#define LOG_DFATAL LOG_ERROR
#define DFATAL ERROR
#define DLOG(severity) true ? (void)0 : de::LogMessageVoidify() & LOG(severity)
#define DLOG_IF(severity, condition) \
  (true || !(condition)) ? (void)0 : de::LogMessageVoidify() & LOG(severity)
#else
#define LOG_DFATAL LOG_FATAL
#define DFATAL FATAL
#define DLOG(severity) LOG(severity)
#define DLOG_IF(severity, condition) LOG_IF(severity, condition)
#endif

// Poor man version of LOG_EVERY_N
#define LOG_EVERY_N(severity, n) LOG(severity)


/**
 * @brief Log time support
 */
class DateLogger
{
public:
    DateLogger()
    {
#if defined(_MSC_VER)
        _tzset();
#endif
    }
    const char* HumanDate()
    {
#if defined(_MSC_VER)
        _strtime_s(buffer_, sizeof(buffer_));
#else
        struct timeval now;

#if !defined(_WIN32)
        struct tm   tm;
        gettimeofday(&now, NULL);
        localtime_r(&now.tv_sec, &tm);
        snprintf(buffer_, sizeof(buffer_), "%02d:%02d:%02d:%03d",
                 static_cast<int>(tm.tm_hour), static_cast<int>(tm.tm_min), static_cast<int>(tm.tm_sec),
                 static_cast<int>(now.tv_usec / 1000));
#else
        pnow = localtime(&time_value);  // NOLINT(*)
        snprintf(buffer_, sizeof(buffer_), "%02d:%02d:%02d",
                 pnow->tm_hour, pnow->tm_min, pnow->tm_sec);
#endif

#endif	// _MSC_VER
        return buffer_;
    }

private:
    char buffer_[13] { 0 };
};

/**
 * @brief Log function implementation class
 */
class LogMessage {
public:
	LogMessage(const char* file, int line)
	{
		static std::chrono::steady_clock::time_point log_start_time = std::chrono::steady_clock::now();

		mutex_.lock();
		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		auto time_span = std::chrono::duration_cast<std::chrono::microseconds>(now - log_start_time);
		auto time_count = time_span.count();
		log_stream_ << "Host-" << time_count << "[" << pretty_date_.HumanDate() << "] " << file << ":"
			<< line << ": ";
	}
	~LogMessage() { 
		log_stream_ << '\n'; 
		Log(log_stream_.str());
		mutex_.unlock();
	}
	std::ostream& stream() { return log_stream_; }
	static void DE_DLL Log(const std::string& msg);

private:
	DE_DLL static std::mutex mutex_;
	DateLogger pretty_date_;
	LogMessage(const LogMessage&);
	void operator=(const LogMessage&);
	std::ostringstream log_stream_;
};

/**
 * @brief customized logger that can allow user to define where to log the message. 
 */
class CustomLogMessage {
public:
	DE_DLL CustomLogMessage();
	~CustomLogMessage() {
		log_stream_ << "\n";
		Log(log_stream_.str());
	}
	std::ostream& stream() { return log_stream_; }
	/*!
	 * \brief customized logging of the message.
	 * \param msg The message to be logged.
	 */
	static void DE_DLL Log(const std::string& msg);

private:
	std::ostringstream log_stream_;
};




#if DE_LOG_STACK_TRACE
inline std::string Demangle(char const *msg_str)
{
    using std::string;
    string msg(msg_str);
    size_t symbol_start = string::npos;
    size_t symbol_end = string::npos;
    if (((symbol_start = msg.find("_Z")) != string::npos)
            && (symbol_end = msg.find_first_of(" +", symbol_start)))
    {
        string left_of_symbol(msg, 0, symbol_start);
        string symbol(msg, symbol_start, symbol_end - symbol_start);
        string right_of_symbol(msg, symbol_end);

        int status = 0;
        size_t length = string::npos;
        std::unique_ptr<char, void(*)(void *__ptr)> demangled_symbol =
        { abi::__cxa_demangle(symbol.c_str(), 0, &length, &status), &std::free };
        if (demangled_symbol && status == 0 && length > 0)
        {
            string symbol_str(demangled_symbol.get());
            std::ostringstream os;
            os << left_of_symbol << symbol_str << right_of_symbol;
            return os.str();
        }
    }
    return string(msg_str);
}

// By default skip the first frame because
// that belongs to ~LogMessageFatal
inline std::string StackTrace(
    size_t start_frame = 1,
    const size_t stack_size = DE_LOG_STACK_TRACE_SIZE)
{
    using std::string;
    std::ostringstream stacktrace_os;
    std::vector<void*> stack(stack_size);
    int nframes = backtrace(stack.data(), static_cast<int>(stack_size));
    stacktrace_os << "Stack trace:\n";
    char **msgs = backtrace_symbols(stack.data(), nframes);
    if (msgs != nullptr)
    {
        for (int frameno = start_frame; frameno < nframes; ++frameno)
        {
            string msg = de::Demangle(msgs[frameno]);
            stacktrace_os << "  [bt] (" << frameno - start_frame << ") " << msg << "\n";
        }
    }
    free(msgs);
    string stack_trace = stacktrace_os.str();
    return stack_trace;
}

#else  // DE_LOG_STACK_TRACE is off

inline std::string demangle(char const* msg_str)
{
    return std::string();
}

inline std::string StackTrace(size_t start_frame = 1,
                              const size_t stack_size = 0)
{
    return std::string("Stack trace not available when "
                       "DE_LOG_STACK_TRACE is disabled at compile time.");
}

#endif  // DE_LOG_STACK_TRACE

/**
 * @brief Log function implementation class when fatal error occurs
 */
class LogMessageFatal
{
public:
    LogMessageFatal(const char* file, int line)
    {
        log_stream_ << "[" << pretty_date_.HumanDate() << "] " << file << ":"
                    << line << ": ";
    }
    std::ostringstream &stream()
    {
        return log_stream_;
    }
    ~LogMessageFatal() DE_THROW_EXCEPTION
    {
#if DE_LOG_STACK_TRACE
        log_stream_ << "\n" << StackTrace() << "\n";
#endif

#if DE_LOG_CUSTOMIZE
        CustomLogMessage::Log(log_stream_.str());
        //waiting for dev flushing log to host
        std::this_thread::sleep_for(std::chrono::seconds(2));
#endif

        // throwing out of destructor is evil
        // hopefully we can do it here
        // also log the message before throw
#if DE_LOG_BEFORE_THROW
        LOG(ERROR) << log_stream_.str();
#endif
        throw Error(log_stream_.str());
    }

private:
    std::ostringstream log_stream_;
    DateLogger pretty_date_;
    LogMessageFatal(const LogMessageFatal&);
    void operator=(const LogMessageFatal&);
};

/**
 * @brief This class is used to explicitly ignore values in the conditional
 *  logging macros.  This avoids compiler warnings like "value computed
 *  is not used" and "statement has no effect".
 */
class LogMessageVoidify
{
public:
    LogMessageVoidify() {}
    // This has to be an operator with a precedence lower than << but
    // higher than "?:". See its usage.
    void operator&(std::ostream&) {}
};

#if DE_LOG_CUSTOMIZE
/**
 * @breif Print out log in NDArray format
 * @param nd		an NDArray object which holds DSP log
 * @note	This function will only be exectuted in DP1000.
 *		If there is a RPC connection between DP1000 and host, DP1000
 *		sends nd by P2P to host. Otherwise DP1000 directly prints it out.
 */
class NDArray;
DE_DLL void PrintOutLogInNDArray(const NDArray& nd);
#endif

}  // namespace de

#endif  // DE_LOGGING_H_

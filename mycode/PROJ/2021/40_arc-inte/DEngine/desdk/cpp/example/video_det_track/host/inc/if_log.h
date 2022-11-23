/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file     image_mixer.h
/// @brief    图像叠加头文件
/// @author   caoyan
/// @date     2019/8/21
/// @version  0.0.1
/// @par      Copyright(C) 2018, Shenzhen Intellifusion Technologies Co.Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef IF_LOG_H
#define IF_LOG_H

#include <memory>
#include <string>
#include <stdarg.h>
#include <chrono>

#ifndef LOG_NAME
#define LOG_NAME "LOG"
#endif

/// @brief 日志输出等级，等于或者小于当前等级的日志会被输出
typedef enum {
  IF_LOG_LEVEL_NONE = 0, ///< 无日志输出
  IF_LOG_LEVEL_NOTICE, ///< 极少的用于交互的输出
  IF_LOG_LEVEL_FATAL, ///< 严重错误
  IF_LOG_LEVEL_ERROR, ///< 普通错误
  IF_LOG_LEVEL_WARNING, ///< 警告
  IF_LOG_LEVEL_INFO, ///< 普通信息
  IF_LOG_LEVEL_DEBUG, ///< 调试信息
#if DEBUG
  IF_LOG_LEVEL_DEFAULT = IF_LOG_LEVEL_DEBUG,
#else
  IF_LOG_LEVEL_DEFAULT = IF_LOG_LEVEL_ERROR,
#endif
} if_log_level_t;

/// @brief SDK日志回调函数
/// @param[in] fmt 日志格式
/// @param[in] arglist 参数列表
/// @return 无
typedef void (*if_sdk_log_cb_t)(const char *fmt, va_list arglist);

//log变量声明，需要在源文件中定义
extern if_log_level_t g_sdk_log_level;
extern if_sdk_log_cb_t g_sdk_log_cb;

#if __ANDROID__
#include  <android/log.h>
#define TAG "JNI_IFaceRecSDK"
#define DEMO_LOG_FATAL(...)  if(IF_LOG_LEVEL_FATAL <= g_sdk_log_level) __android_log_print(ANDROID_LOG_FATAL, TAG,__VA_ARGS__)
#define LOG_NOTICE(...) if(IF_LOG_LEVEL_NOTICE <= g_sdk_log_level) __android_log_print(ANDROID_LOG_FATAL, TAG,__VA_ARGS__)
#define DEMO_LOG_ERROR(...)  if(IF_LOG_LEVEL_ERROR <= g_sdk_log_level) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#define LOG_WARN(...)   if(IF_LOG_LEVEL_WARNING <= g_sdk_log_level) __android_log_print(ANDROID_LOG_ERROR, TAG,__VA_ARGS__)
#define DEMO_LOG_INFO(...)   if(IF_LOG_LEVEL_INFO <= g_sdk_log_level) __android_log_print(ANDROID_LOG_INFO, TAG,__VA_ARGS__)
#define DEMO_LOG_DEBUG(...)  if(IF_LOG_LEVEL_DEBUG <= g_sdk_log_level) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#else

#if 0
#define GET_TIME {auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());\
  struct tm* ptm = localtime(&tt);\
  printf("[%s][%d-%02d-%02d-%02d.%02d.%02d]", LOG_NAME, \
    (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday,\
    (int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec);}
#else
#define GET_TIME {auto timeNow = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()); \
  printf("[%s][%ld]", LOG_NAME, timeNow.count());}
#endif

#if 0
#define DEMO_LOG_FATAL(...) if(IF_LOG_LEVEL_FATAL <= g_sdk_log_level) {GET_TIME; printf(__VA_ARGS__);}
#define DEMO_LOG_ERROR(...) if(IF_LOG_LEVEL_ERROR <= g_sdk_log_level) {GET_TIME; printf(__VA_ARGS__);}
#define LOG_WARN(...)  if(IF_LOG_LEVEL_WARNING <= g_sdk_log_level) {GET_TIME; printf(__VA_ARGS__);}
#define DEMO_LOG_INFO(...)  if(IF_LOG_LEVEL_INFO <= g_sdk_log_level) {GET_TIME; printf(__VA_ARGS__);}
#define DEMO_LOG_DEBUG(...) if(IF_LOG_LEVEL_DEBUG <= g_sdk_log_level) {GET_TIME; printf(__VA_ARGS__);}
#else
inline
void DEMO_LOG_FATAL(const char *fmt, ...) {
  if (IF_LOG_LEVEL_FATAL > g_sdk_log_level) return;
  char buf[256] = {0};
  va_list vl;
  va_start(vl, fmt);
  vsnprintf(buf, sizeof(buf), fmt, vl);
  GET_TIME;
  if(g_sdk_log_cb)
    g_sdk_log_cb(fmt,vl);
  else
    printf("FATAL|%s", buf);
  va_end(vl);
}

inline
void LOG_NOTICE(const char *fmt, ...) {
  if (IF_LOG_LEVEL_NOTICE > g_sdk_log_level) return;
  char buf[256] = {0};
  va_list vl;
  va_start(vl, fmt);
  vsnprintf(buf, sizeof(buf), fmt, vl);
  GET_TIME;
  if(g_sdk_log_cb)
    g_sdk_log_cb(fmt,vl);
  else
    printf("NOTICE|%s", buf);
  va_end(vl);
}

inline
void DEMO_LOG_ERROR(const char *fmt, ...) {
  if (IF_LOG_LEVEL_ERROR > g_sdk_log_level) return;
  char buf[256] = {0};
  va_list vl;
  va_start(vl, fmt);
  vsnprintf(buf, sizeof(buf), fmt, vl);
  GET_TIME;
  if(g_sdk_log_cb)
    g_sdk_log_cb(fmt,vl);
  else
    printf("ERROR|%s", buf);
  va_end(vl);
}

inline
void LOG_WARN(const char *fmt, ...) {
  if (IF_LOG_LEVEL_WARNING > g_sdk_log_level) return;
  char buf[256] = {0};
  va_list vl;
  va_start(vl, fmt);
  vsnprintf(buf, sizeof(buf), fmt, vl);
  GET_TIME;
  if(g_sdk_log_cb)
    g_sdk_log_cb(fmt,vl);
  else
    printf("WARN|%s", buf);
  va_end(vl);
}

inline
void DEMO_LOG_INFO(const char *fmt, ...) {
  if (IF_LOG_LEVEL_INFO > g_sdk_log_level) return;
  char buf[256] = {0};
  va_list vl;
  va_start(vl, fmt);
  vsnprintf(buf, sizeof(buf), fmt, vl);
  GET_TIME;
  if(g_sdk_log_cb)
    g_sdk_log_cb(fmt,vl);
  else
    printf("INFO|%s", buf);
  va_end(vl);
}

inline
void DEMO_LOG_DEBUG(const char *fmt, ...) {
  if (IF_LOG_LEVEL_DEBUG > g_sdk_log_level) return;
  char buf[256] = {0};
  va_list vl;
  va_start(vl, fmt);
  vsnprintf(buf, sizeof(buf), fmt, vl);
  GET_TIME;
  if(g_sdk_log_cb)
    g_sdk_log_cb(fmt,vl);
  else
    printf("DEBUG|%s", buf);
  va_end(vl);
}

#endif

#endif

#endif //IF_LOG_H


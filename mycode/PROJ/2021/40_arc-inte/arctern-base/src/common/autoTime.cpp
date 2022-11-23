/****************************************************************************
 *  Filename:       autoTime.cpp
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.11
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include <stdlib.h>
#include <string.h>
#if defined(_MSC_VER)
#include <Windows.h>
#else
#include <sys/time.h>
#endif
#include "include/autoTime.h"
#include "log/logger.h"


namespace arctern {

Timer::Timer() {
    reset();
}

Timer::~Timer() {
    // do nothing
}

void Timer::reset() {
    #if defined(_MSC_VER)
        LARGE_INTEGER time, freq;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&time);
        uint64_t sec = time.QuadPart / freq.QuadPart;
        uint64_t usec = (time.QuadPart % freq.QuadPart) * 1000000 / freq.QuadPart;
        mLastResetTime = sec * 1000000 + usec;
    #else
        struct timeval Current;
        gettimeofday(&Current, nullptr);
        mLastResetTime = Current.tv_sec * 1000000 + Current.tv_usec;
    #endif
}

uint64_t Timer::durationInUs() {
    #if defined(_MSC_VER)
        LARGE_INTEGER time, freq;
        QueryPerformanceCounter(&time);
        QueryPerformanceFrequency(&freq);
        uint64_t sec = time.QuadPart / freq.QuadPart;
        uint64_t usec = (time.QuadPart % freq.QuadPart) * 1000000 / freq.QuadPart;
        auto lastTime = sec * 1000000 + usec;
    #else
        struct timeval Current;
        gettimeofday(&Current, nullptr);
        auto lastTime = Current.tv_sec * 1000000 + Current.tv_usec;
    #endif

    return lastTime - mLastResetTime;
}

AutoTime::AutoTime(int line, const char* func) : Timer() {
    mName = ::strdup(func);
    mLine = line;
}
AutoTime::~AutoTime() {
    auto timeInUs = durationInUs();
    spdlog::info("{0}, {1}, cost time: {2} ms\n", mName, mLine, timeInUs / 1000.0f);
    free(mName);
}

}  // namespace arctern

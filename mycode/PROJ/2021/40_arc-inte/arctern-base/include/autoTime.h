/****************************************************************************
 *  Filename:       autoTime.hpp
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.11
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef INCLUDE_AUTOTIME_H_
#define INCLUDE_AUTOTIME_H_

#include <stdint.h>
#include <stdio.h>

#define ARCTERN_OPEN_TIME_TRACE

namespace arctern {
class Timer {
 public:
  Timer();
  ~Timer();
  Timer(const Timer &) = delete;
  Timer(Timer &&) = delete;
  Timer &operator=(const Timer &) = delete;
  Timer &operator=(Timer &&) = delete;
  ///> reset timer
  void reset();
  ///> get duration (us) from init or latest reset.
  uint64_t durationInUs();
 protected:
  uint64_t mLastResetTime;
};

/** time tracing util. prints duration between init and deinit. */
class AutoTime : Timer {
 public:
  AutoTime(int line, const char *func);
  ~AutoTime();
  AutoTime(const AutoTime &) = delete;
  AutoTime(AutoTime &&) = delete;
  AutoTime &operator=(const AutoTime &) = delete;
  AutoTime &operator=(AutoTime &&) = delete;

 private:
  int mLine;
  char *mName;
};
}  // namespace arctern

#ifdef ARCTERN_OPEN_TIME_TRACE
#define AUTOTIME arctern::AutoTime ___t(__LINE__, __func__)
#else
#define AUTOTIME
#endif

#endif /* INCLUDE_AUTOTIME_H_ */

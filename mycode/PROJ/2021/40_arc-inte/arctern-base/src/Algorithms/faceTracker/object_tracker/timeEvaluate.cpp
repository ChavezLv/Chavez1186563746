/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "src/Algorithms/faceTracker/object_tracker/timeEvaluate.h"

#ifdef _WIN32

void mfxGetTime(mfxTime* timestamp) {
  mfxTime tp = std::chrono::high_resolution_clock::now();
  *timestamp = tp;
}

double TimeDiffMsec(mfxTime tfinish, mfxTime tstart) {
  std::chrono::duration<double> diff;
  diff = tfinish - tstart;
  return diff.count();
}

#else

void mfxGetTime(mfxTime *timestamp) {
  clock_gettime(CLOCK_REALTIME, timestamp);
}

double TimeDiffMsec(mfxTime tfinish, mfxTime tstart) {
  double result;
  long long elapsed_nsec = tfinish.tv_nsec - tstart.tv_nsec;   // NOLINT
  long long elapsed_sec = tfinish.tv_sec - tstart.tv_sec;   // NOLINT

  // if (tstart.tv_sec==0) return -1;

  // timespec uses two fields -- check if borrowing necessary
  if (elapsed_nsec < 0) {
    elapsed_sec -= 1;
    elapsed_nsec += 1000000000;
  }
  // return total converted to milliseconds
  result = static_cast<double >(elapsed_sec) * 1000.0;
  result += static_cast<double>(elapsed_nsec) / 1000000;

  return result;
}
#endif

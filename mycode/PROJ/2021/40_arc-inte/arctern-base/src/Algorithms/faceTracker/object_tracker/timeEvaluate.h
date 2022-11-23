/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_TIMEEVALUATE_H_
#define SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_TIMEEVALUATE_H_

#define  NOMINMAX

#ifdef _WIN32
#include <chrono>                    // NOLINT
typedef  std::chrono::high_resolution_clock::time_point mfxTime;
void mfxGetTime(mfxTime* timestamp);
double TimeDiffMsec(mfxTime tfinish, mfxTime tstart);
#else
#include <time.h>
typedef timespec mfxTime;
void mfxGetTime(mfxTime *timestamp);
double TimeDiffMsec(mfxTime tfinish, mfxTime tstart);
#endif

#endif  // SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_TIMEEVALUATE_H_

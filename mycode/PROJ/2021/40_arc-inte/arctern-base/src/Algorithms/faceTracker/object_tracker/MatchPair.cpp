/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include "MatchPair.hpp"

MatchPair::MatchPair(cv::Rect a, cv::Rect b, double s) {
  r1 = a;
  r2 = b;
  score = s;
}

MatchPair::~MatchPair() {
}


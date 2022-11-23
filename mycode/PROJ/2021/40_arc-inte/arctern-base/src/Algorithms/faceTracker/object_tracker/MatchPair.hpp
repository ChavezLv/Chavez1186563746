/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef __MATCHPAIR_HPP__
#define __MATCHPAIR_HPP__

#include <opencv2/opencv.hpp>

/**
 * Specifies a pair for two regions as matched regions
 * */
// class MatchPair{
//    public:
//   // First region in the pair
//  Region *r1;
//   // Second region in the pair
//   Region *r2;
//   // Match score
//   double score;
//   // Constructor
//   MatchPair(Region *a, Region *b, double s);
//   // Destructor
//   ~MatchPair();
//};

class MatchPair {
 public:
  // First region in the pair
  cv::Rect r1;
  // Second region in the pair
  cv::Rect r2;
  // Match score
  double score;
  // Constructor
  MatchPair(cv::Rect a, cv::Rect b, double s);
  // Destructor
  ~MatchPair();
};

#endif

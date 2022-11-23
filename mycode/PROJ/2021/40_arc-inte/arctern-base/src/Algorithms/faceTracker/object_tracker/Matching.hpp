/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef __MATCHING_HPP__
#define __MATCHING_HPP__

#include <string>
#include <vector>
#include <iostream>
#include "MatchPair.hpp"
#include "Hungarian.hpp"

using std::vector;
/**
 * Class that computes the matching between annotated and 
 * detected regions
 * */
class Matching {
 public:
  /// Name of the algorithm used for matching
  std::string matchingAlgoStr;
  // @zzf_added 20161014
  Matching(vector<cv::Rect> *, vector<cv::Rect> *);
  /// Destructor
  ~Matching();
  /// Compute the matching pairs.
  /// Returns a vector of MatchPair pointers
  vector<MatchPair *> *getMatchPairs();
  // @zzf_added 20161014
  void clearMatchPairs(vector<MatchPair *> *);

 private:
  ///// Set of annotated regions
  // RegionsSingleImage *annot;
  ///// Set of detected regions
  // RegionsSingleImage *det;

  // @zzf_added 20161014
  vector<cv::Rect> *annot_list;
  vector<cv::Rect> *det_list;

  /// Matrix of matching scores for annnotation and detections
  vector<vector<double> *> *pairWiseScores;

  /// Computes the score for a single pair of regions
  /*double computeScore(Region *, Region *);*/
  double computeScore(cv::Rect cvr1, cv::Rect cvr2);
  /// populate the pairWiseScores matrix
  void computePairWiseScores();
  /// Free the memory for the pairWiseScores matrix
  void clearPairWiseScores();
  /// Runs the Hungarian algorithm
  vector<MatchPair *> *runHungarian();
};

#endif

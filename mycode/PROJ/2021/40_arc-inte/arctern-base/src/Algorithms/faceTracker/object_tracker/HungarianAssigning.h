/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_HUNGARIANASSIGNING_H_
#define SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_HUNGARIANASSIGNING_H_

#include <string>
#include <vector>
#include <iostream>
#include "./hunmatrix.h"
#include "./munkres.h"

typedef struct {
  unsigned int row_id;
  unsigned int col_id;
} AssignPair;

typedef struct {
  std::vector<AssignPair> match_ids;
  std::vector<unsigned int> unmatch_row_ids;
  std::vector<unsigned int> unmatch_col_ids;
} AssignResult;

using std::vector;
/**
 * Class that computes assignment pairs given matching scores*/
class HungarianAssigning {
 public:
  HungarianAssigning() {}
  /// Destructor
  ~HungarianAssigning() {}
  /// Returns assignment matrix
  /// @param min_flag  ture :get minus value sum after hungarian, otherwise get maximum;
  AssignResult getAssignResult(double *match_matrix,
                               unsigned int rows,
                               unsigned int cols,
                               double score_thres,
                               bool min_flag = false);
 private:
};

#endif  // SRC_ALGORITHMS_FACETRACKER_OBJECT_TRACKER_HUNGARIANASSIGNING_H_

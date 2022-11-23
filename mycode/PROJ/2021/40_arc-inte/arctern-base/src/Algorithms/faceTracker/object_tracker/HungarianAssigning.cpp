/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include <vector>
#include "src/Algorithms/faceTracker/object_tracker/HungarianAssigning.h"

/// @brief refer www.github.com/saebyn/munkres-cpp  20180903 zhangzhaofeng
/// @param min_flag  ture :get minus value sum after hungarian, otherwise get maximum;
AssignResult HungarianAssigning::getAssignResult(double *match_matrix,
                                                 unsigned int rows,
                                                 unsigned int cols,
                                                 double score_thres,
                                                 bool min_flag) {
  AssignResult assign_result;
  if ((rows == 0) || (cols == 0)) {
    return assign_result;
  }
  for (unsigned int i = 0; i < rows; i++) { assign_result.unmatch_row_ids.push_back(i); }
  for (unsigned int j = 0; j < cols; j++) { assign_result.unmatch_col_ids.push_back(j); }
  // Initialize the match matrix used in the hungarian algorithm
  HunMatrix<double> matchMat(rows, cols);
  for (unsigned int i = 0; i < rows; i++) {
    for (unsigned int j = 0; j < cols; j++) {
      if (min_flag == true) {
        matchMat(i, j) = match_matrix[i * cols + j];
      } else {
        matchMat(i, j) = -1.0 * match_matrix[i * cols + j];
      }
    }
  }
  Munkres<double> munkres;
  munkres.solve(matchMat);
  // Create the set of match pairs from the assignment matrix from the Hungarian algo
  for (unsigned int i = 0; i < rows; i++) {
    for (unsigned int j = 0; j < cols; j++) {
      double score = match_matrix[i * cols + j];
      if (matchMat(i, j) == 0 && score > score_thres) {
        AssignPair assign_pair;
        assign_pair.row_id = i;
        assign_pair.col_id = j;
        assign_result.match_ids.push_back(assign_pair);
        for (vector<unsigned int>::iterator iter = assign_result.unmatch_row_ids.begin();
             iter != assign_result.unmatch_row_ids.end(); iter++) {
          if (*iter == i) {
            assign_result.unmatch_row_ids.erase(iter);
            break;
          }
        }
        for (vector<unsigned int>::iterator iter = assign_result.unmatch_col_ids.begin();
             iter != assign_result.unmatch_col_ids.end(); iter++) {
          if (*iter == j) {
            assign_result.unmatch_col_ids.erase(iter);
            break;
          }
        }
        // break;
      }
    }
  }
  return assign_result;
}

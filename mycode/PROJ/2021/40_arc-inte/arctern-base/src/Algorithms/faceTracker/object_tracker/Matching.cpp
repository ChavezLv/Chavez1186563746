/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.10
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include "Matching.hpp"
#include <iostream>
#include <vector>
#include "./overlap.h"

using std::cerr;
using std::endl;
using std::vector;

Matching::Matching(vector<cv::Rect> *v1, vector<cv::Rect> *v2) {
  annot_list = v1;
  det_list = v2;
  pairWiseScores = NULL;
}

Matching::~Matching() {
  // free memory for the score matrix
  clearPairWiseScores();
}

// double Matching::computeScore(Region *r1, Region *r2)
// {
//  double score = (r1->setIntersect(r2)) / (r1->setUnion(r2));
//  return score;
// }

// double Matching::computeScore(Region *r1, Region *r2)
// {
//  cv::Rect cvr1 = ((RectangleR*)r1)->getRect();
//  cv::Rect cvr2 = ((RectangleR*)r2)->getRect();
//  seRectPair M = seRectPair(cvr1, cvr2);
//  double score = M.getOverlapRatio();
//  return score;
// }

double Matching::computeScore(cv::Rect cvr1, cv::Rect cvr2) {
  seRectPair M = seRectPair(cvr1, cvr2);
  double score = M.getOverlapRatio();
  return score;
}

void Matching::computePairWiseScores() {
  int nAnnot = annot_list->size();
  int nDet = det_list->size();

  if (pairWiseScores == NULL) {
    // first-time access of the score matrix, so initialize it
    pairWiseScores = new vector<vector<double> *>;
    pairWiseScores->reserve(nAnnot);
    for (int ai = 0; ai < nAnnot; ai++)
      pairWiseScores->push_back(NULL);
  }

  for (int ai = 0; ai < nAnnot; ai++) {
    cv::Rect ar = annot_list->at(ai);
    vector<double> *v;
    if (pairWiseScores->at(ai) != NULL)
      v = pairWiseScores->at(ai);
    else
      v = new vector<double>(nDet, -1);

    for (int di = 0; di < nDet; di++) {
      cv::Rect dr = det_list->at(di);
      v->at(di) = computeScore(ar, dr);
    }
    pairWiseScores->at(ai) = v;
  }
}

void Matching::clearPairWiseScores() {
  if (pairWiseScores == NULL)
    return;

  for (unsigned int ai = 0; ai < pairWiseScores->size(); ai++)
    delete (pairWiseScores->at(ai));
  delete (pairWiseScores);
}

vector<MatchPair *> *Matching::runHungarian() {
  int nAnnot = annot_list->size();
  int nDet = det_list->size();

  // Remove the annotations with no matching detections
  int *mapI = reinterpret_cast<int *>(calloc(nAnnot, sizeof(int)));
  int mI = 0;
  for (int i = 0; i < nAnnot; i++) {
    double sum = 0;
    for (int j = 0; j < nDet; j++)
      if (1)
        sum += pairWiseScores->at(i)->at(j);        // 遍历对应每一个i的每一个j的score
    if (sum != 0)                            // 当有和i匹配的j时，sum不为0
      mapI[mI++] = i;                        // save i when sum！=0
  }

  // Remove the detections with no matching annotations
  int *mapJ = reinterpret_cast<int *>(calloc(nDet, sizeof(int)));
  int mJ = 0;
  for (int j = 0; j < nDet; j++) {
    if (1) {
      double sum = 0;
      for (int i = 0; i < nAnnot; i++)
        sum += pairWiseScores->at(i)->at(j);
      if (sum != 0)
        mapJ[mJ++] = j;
    }
  }

  int nRow, nCol;
  bool useTranspose = (mI > mJ);
  if (useTranspose) {
    nCol = mI;
    nRow = mJ;
  } else {
    nRow = mI;
    nCol = mJ;
  }

  // Initialize the match matrix used in the hungarian algorithm
  double **matchMat = reinterpret_cast<double **>(calloc(nRow, sizeof(double *)));
  for (int i = 0; i < nRow; i++) {
    matchMat[i] = reinterpret_cast<double *>(calloc(nCol, sizeof(double)));
  }

  if (useTranspose) {
    for (int i = 0; i < nRow; i++)
      for (int j = 0; j < nCol; j++)
        matchMat[i][j] = pairWiseScores->at(mapI[j])->at(mapJ[i]);
  } else {
    for (int i = 0; i < nRow; i++)
      for (int j = 0; j < nCol; j++)
        matchMat[i][j] = pairWiseScores->at(mapI[i])->at(mapJ[j]);
  }

  hungarian_t prob;
  hungarian_init(&prob, matchMat, nRow, nCol, HUNGARIAN_MAX);
  // hungarian_init(&prob,matchMat,nAnnot,nDet,HUNGARIAN_MAX);
  // hungarian_print_rating(&prob);
  hungarian_solve(&prob);
  // hungarian_print_assignment(&prob);

  // Create the set of match pairs from the assignment matrix from the Hungarian algo
  vector<MatchPair *> *mps = NULL;
  if (hungarian_check_feasibility(&prob)) {
    mps = new vector<MatchPair *>;
    if (useTranspose) {
      for (int i = 0; i < nRow; i++)
        for (int j = 0; j < nCol; j++) {
          double score = pairWiseScores->at(mapI[j])->at(mapJ[i]);
          if (prob.a[i] == j && score > 0) {
            cv::Rect dr = det_list->at(mapJ[i]);
            cv::Rect ar = annot_list->at(mapI[j]);
            mps->push_back(new MatchPair(ar, dr, score));
          }
        }
    } else {
      for (int i = 0; i < nRow; i++) {
        vector<double> *pwsI = pairWiseScores->at(mapI[i]);
        for (int j = 0; j < nCol; j++) {
          double score = pwsI->at(mapJ[j]);
          if (prob.a[i] == j && score > 0) {
            cv::Rect dr = det_list->at(mapJ[j]);
            cv::Rect ar = annot_list->at(mapI[i]);
            mps->push_back(new MatchPair(ar, dr, score));
          }
        }
      }
    }
  } else {
    cerr << "Not yet implemented" << endl;
  }

  // free memory used by hungarian algorithm
  hungarian_fini(&prob);

  free(mapI);
  free(mapJ);
  for (int i = 0; i < nRow; i++)
    free(matchMat[i]);
  free(matchMat);
  return mps;
}

vector<MatchPair *> *Matching::getMatchPairs() {
  computePairWiseScores();
  return runHungarian();
}

void Matching::clearMatchPairs(vector<MatchPair *> *mps) {
  for (size_t i = 0; i < mps->size(); i++) {
    delete (*mps)[i];
  }
  delete mps;
}

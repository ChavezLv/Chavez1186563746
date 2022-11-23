/****************************************************************************
 *  Filename:       algorithmbase.h
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.02.20
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/
#ifndef SRC_BASE_ALGORITHMBASE_H_
#define SRC_BASE_ALGORITHMBASE_H_

#include "include/initparameter.h"
#include "include/runparameter.h"
#include "include/result.h"

namespace arctern {

class AlgorithmBase {
 public:
  AlgorithmBase() = default;
  virtual ~AlgorithmBase() = default;
  virtual ErrorType Init(const InitParameter *p) = 0;
  virtual ErrorType Process(const RunParameter *p, Result *r) = 0;
  virtual void Release() = 0;
};

}  // namespace arctern
#endif  // SRC_BASE_ALGORITHMBASE_H_

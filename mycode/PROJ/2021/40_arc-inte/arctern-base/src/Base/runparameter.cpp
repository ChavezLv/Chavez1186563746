/****************************************************************************
 *  Filename:       runparameter.cpp
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.02.21
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/
#include "include/runparameter.h"
namespace arctern{
  ErrorType RunParameter::CheckSize() const {
    if(imageV_.size() != rectsV_.size()) return ERR_RUN_PARM;
    return ERR_SUCCESS;
  }
}




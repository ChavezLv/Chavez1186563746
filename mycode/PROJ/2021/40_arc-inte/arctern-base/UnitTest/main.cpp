/****************************************************************************
 *  Filename:       initparameter.h
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.07
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include "gtest/gtest.h"

int main(int argc, char ** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
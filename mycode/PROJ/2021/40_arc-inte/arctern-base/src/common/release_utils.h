
/****************************************************************************
 *  Filename:       release_utils.h
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.08.11
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/

/// Don't edit this file, this file will be replaced in release phase
// Contact Xutao Lv if you really need to use this file
#ifndef SRC_COMMON_RELEASE_UTILS_H_
#define SRC_COMMON_RELEASE_UTILS_H_
#include <string>
/// Return a std::string type of variable named "RELEASE_KEY_STRING",
// which can be used to initialize ArcternManagerImpl
#define GENERATE_RELEASE_KEYS_BLOCK \
    std::string RELEASE_KEY_STRING = "0123456789123456";

#endif  // SRC_COMMON_RELEASE_UTILS_H_

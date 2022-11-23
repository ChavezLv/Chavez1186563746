/****************************************************************************
 *  Filename:       aes_cryptor.cpp
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.05.11
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#ifndef SRC_COMMON_ENCRYPTOR_H_
#define SRC_COMMON_ENCRYPTOR_H_
#include <vector>

namespace arctern {

class Encryptor {
 public:
  Encryptor() {}
  virtual ~Encryptor()=default;
  virtual void init() = 0;
};

}  // namespace arctern

#endif  // SRC_COMMON_ENCRYPTOR_H_

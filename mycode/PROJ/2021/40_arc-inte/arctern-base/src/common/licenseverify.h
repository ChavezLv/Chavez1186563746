/****************************************************************************
 *  Filename:       aes_cryptor.cpp
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.05.11
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#ifndef SRC_COMMON_LICENSEVERIFY_H_
#define SRC_COMMON_LICENSEVERIFY_H_

#include <string>
#include <memory>
#include "include/arctern.h"
#include "src/common/keys.h"
#include "src/common/aes_cryptor.h"
#include "src/common/release_utils.h"
#ifdef WITH_LICENSE
#include "./licenser.h"
#endif  // if def WITH_LICENSE

namespace arctern {

class LicenseVerify {
 public:
  explicit LicenseVerify(const std::string & licfile_path);
  ErrorType license_verify();
  ErrorType license_verify_periodic();

 private:
  const int period_ = 10000;
  mutable int process_counter_;

#ifdef WITH_LICENSE
  std::shared_ptr<licenser::Licenser> licenser_;
#endif  // if def WITH_LICENSE

 public:
  std::shared_ptr<AESCryptor> aescryptor_;
};

}  // namespace arctern

#endif  // SRC_COMMON_LICENSEVERIFY_H_



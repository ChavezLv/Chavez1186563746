/****************************************************************************
 *  Filename:       aes_cryptor.cpp
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.05.11
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#include <iostream>
#include <cstdlib>
#include "src/common/licenseverify.h"

namespace arctern {

#ifdef WITH_LICENSE
LicenseVerify::LicenseVerify(const std::string & licfile_path) {
  srand((unsigned)time(NULL));
  licenser_ = std::make_shared<licenser::Licenser>(
      licfile_path, LICENSER_AES_KEY, LICENSER_PUB_KEY);
  GENERATE_RELEASE_KEYS_BLOCK;
  aescryptor_ = std::make_shared<AESCryptor>(RELEASE_KEY_STRING);
  process_counter_ = period_;
}
#else
LicenseVerify::LicenseVerify(const std::string &licfile_path) {
  srand((unsigned) time(NULL));
  GENERATE_RELEASE_KEYS_BLOCK;
  aescryptor_ = std::make_shared<AESCryptor>(RELEASE_KEY_STRING);
}
#endif  // if def WITH_LICENSE

ErrorType LicenseVerify::license_verify() {
#ifdef WITH_LICENSE
  if (!licenser_->verify()) {
    std::cerr << "Failed to verify the license!" << std::endl;
    return ErrorType::ERR_BAD_LICENSE;
  }
#endif  // if def WITH_LICENSE
  return ErrorType::ERR_SUCCESS;
}

ErrorType LicenseVerify::license_verify_periodic() {
  ++process_counter_;
  if (process_counter_ > period_) {
    process_counter_ = 0;
    return license_verify();
  }
  return ErrorType::ERR_SUCCESS;
}

}  // namespace arctern

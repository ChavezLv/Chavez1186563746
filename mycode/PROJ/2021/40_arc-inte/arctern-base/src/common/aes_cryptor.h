/****************************************************************************
 *  Filename:       aes_cryptor.cpp
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.05.11
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#ifndef  SRC_COMMON_AES_CRYPTOR_H_
#define  SRC_COMMON_AES_CRYPTOR_H_

#include <string>
#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

#include "src/common/encryptor.h"
#include "src/common/file_buffer.h"
#if !defined(BUILD_OSX) && defined(TARGET_OS_IPHONE)
#include "./config.h"  // byte
#else
#include <cryptopp/config.h>  // byte
#endif

namespace CryptoPP {
#ifndef byte
typedef unsigned char byte;
#endif
}  // namespace CryptoPP

namespace arctern {

class AESCryptor : public Encryptor {
 public:
  static const int NBYTE_KEY = 16;

  explicit AESCryptor(const std::string& k);

  void init();

 public:
  bool encrypt_string(const std::string& content, std::string& cipher);
  bool decrypt_string(const std::string& cipher, std::string& content);

  bool encrypt_file(const std::string& file_name);
  bool encrypt_file(const std::string& file_name, const std::string &buf);
  bool decrypt_file(const std::string& file_name);
  bool decrypt_file(const std::string& file_name, FileBuffer& filebuffer);
  bool decrypt_file(const FileBuffer& ifilebuffer, FileBuffer& ofilebuffer);
  bool encrypt_folder(const std::string& folder);
  bool decrypt_folder(const std::string& folder);

 private:
  std::string hex_key();
  void print_key();

 private:
  const std::string crypt_ext_ = ".cipher";
  CryptoPP::byte key_[NBYTE_KEY];
};

}  // namespace arctern

#endif  //  SRC_COMMON_AES_CRYPTOR_H_

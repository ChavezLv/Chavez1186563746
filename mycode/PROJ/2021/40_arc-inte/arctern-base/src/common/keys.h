/****************************************************************************
 *  Filename:       aes_cryptor.cpp
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.05.11
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#ifndef SRC_COMMON_KEYS_H_
#define SRC_COMMON_KEYS_H_

#include <string>

// #define LICENSER_AES_KEY "1516D2A628AEABF7158809CF2B7E4F3C"
#define LICENSER_AES_KEY_XOR0x55 {  \
  0x64, 0x60, 0x64, 0x63, 0x11, 0x67, 0x14, 0x63, \
  0x67, 0x6d, 0x14, 0x10, 0x14, 0x17, 0x13, 0x62, \
  0x64, 0x60, 0x6d, 0x6d, 0x65, 0x6c, 0x16, 0x13, \
  0x67, 0x17, 0x62, 0x10, 0x61, 0x13, 0x66, 0x16 }

static std::string gety() {
  std::string x(LICENSER_AES_KEY_XOR0x55);
  for (auto it = x.begin(); it != x.end(); ++it) {
      (*it) ^= 0x55;
  }

  return x;
}

#define LICENSER_AES_KEY gety()

#define LICENSER_PUB_KEY \
  "-----BEGIN PUBLIC KEY-----\n"  \
  "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDC5WLq3ShMEzTaM6yH5cgh8B1j\n"  \
  "Ri0275M27TC48TnD13GW3ux5aheG9zKzlqAJgQfCVkez2Wz9TQPEIlgC4AJMgYy1\n"  \
  "UC0HfJtNSHpRXIcnsTdWvJ6+ZQK5cBS7D/YO7d0UNqGfsMhC0+9PkUpoM/jkyZyh\n"  \
  "p5Om3XwCY36vW7DvxQIDAQAB\n"  \
  "-----END PUBLIC KEY-----"

#endif    // SRC_COMMON_KEYS_H_


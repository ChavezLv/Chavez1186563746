/****************************************************************************
 *  Filename:       aes_cryptor.cpp
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.05.11
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#include <string>
#ifndef WIN32
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include <fstream>
#include <iostream>
#include <sstream>

#ifdef __APPLE__
#include <TargetConditionals.h>
#endif
#if !defined(BUILD_OSX) && defined(TARGET_OS_IPHONE)
#include "./cryptlib.h"
#include "./filters.h"
#include "./hex.h"
#include "./modes.h"
#include "./osrng.h"
#else
#include <cryptopp/cryptlib.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#endif

#include "./aes_cryptor.h"

using arctern::AESCryptor;

AESCryptor::AESCryptor(const std::string &k) {
  if ("" == k) {
    CryptoPP::AutoSeededRandomPool prng;
    prng.GenerateBlock(key_, NBYTE_KEY);
    std::string s = hex_key();
    memcpy(key_, s.c_str(), NBYTE_KEY);
    // print_key();
  } else {
    if (k.size() < NBYTE_KEY) {
      std::stringstream ss;
      ss << "key size needs to be larger than " << NBYTE_KEY;
      throw std::runtime_error(ss.str());
    }

    CryptoPP::byte *byte_key = (CryptoPP::byte *) k.c_str();
    memcpy(key_, byte_key, NBYTE_KEY);
  }
}

void AESCryptor::init() {}

bool AESCryptor::encrypt_string(const std::string &content,
                                std::string &cipher) {
  try {
    CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption encrypt;
    encrypt.SetKey(key_, NBYTE_KEY);
    CryptoPP::StringSource s(content, true,
                             new CryptoPP::StreamTransformationFilter(
                                 encrypt, new CryptoPP::StringSink(cipher)));
  } catch (const CryptoPP::Exception &e) {
    std::cerr << e.what() << std::endl;
    return false;
  }

  return true;
}

bool AESCryptor::decrypt_string(const std::string &cipher,
                                std::string &content) {
  try {
    CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption decrypt;
    decrypt.SetKey(key_, NBYTE_KEY);
    CryptoPP::StringSource s(cipher, true,
                             new CryptoPP::StreamTransformationFilter(
                                 decrypt, new CryptoPP::StringSink(content)));
  } catch (const CryptoPP::Exception &e) {
    std::cerr << e.what() << std::endl;
    return false;
  }

  return true;
}

bool AESCryptor::encrypt_file(const std::string &file_name) {
#ifndef WIN32
  if (0 != access(file_name.c_str(), F_OK)) {
    std::cerr << "Input path " << file_name << " does not exist." << std::endl;
    return false;
  }
#endif

  if (file_name.find(crypt_ext_) == file_name.size() - crypt_ext_.size()) {
    std::cerr << "Input path is a \".cipher\" file (already encrypted)."
              << std::endl;
    return false;
  }

  // std::cout << "encrypting " << file_name << std::endl;
  std::ifstream ifs(file_name.c_str(), std::ios::binary);
  if (!ifs.is_open()) {
    std::cerr << "Failed to open " << file_name << "." << std::endl;
    return false;
  }

  std::string file_name_cipher(file_name + crypt_ext_);
  std::ofstream ofs(file_name_cipher.c_str(), std::ios::binary);
  if (!ofs.is_open()) {
    std::cerr << "Failed to open " << file_name_cipher << "." << std::endl;
    return false;
  }

  std::stringstream buffer;
  buffer << ifs.rdbuf();
  std::string read_content(buffer.str());

  std::string write_content;
  encrypt_string(read_content, write_content);
  ofs << write_content;

  return true;
}

bool AESCryptor::encrypt_file(const std::string &file_name, const std::string &buf) {
  FILE *fpout = fopen(file_name.c_str(), "wb");

  if (fpout) {
#ifdef PLAIN_MODEL
    fwrite(buf.c_str(), sizeof(char), buf.size(), fpout);
#else
    std::string write_content;
    encrypt_string(buf, write_content);
    fwrite(write_content.c_str(), sizeof(char), write_content.size(), fpout);
#endif
    fclose(fpout);
    return true;
  }

  return false;
}

bool AESCryptor::decrypt_file(const std::string &file_name) {
#ifndef WIN32
  if (0 != access(file_name.c_str(), F_OK)) {
    std::cerr << "Input path " << file_name << " does not exist." << std::endl;
    return false;
  }
#endif

  // std::cout << file_name << std::endl;
  if (file_name.find(crypt_ext_) != file_name.size() - crypt_ext_.size()) {
    std::cerr << "Input path is not a \".cipher\" file." << std::endl;
    return false;
  }

  // std::cout << "decrypting " << file_name << std::endl;
  std::ifstream ifs(file_name.c_str(), std::ios::binary);
  if (!ifs.is_open()) {
    std::cerr << "Failed to open " << file_name << "." << std::endl;
    return false;
  }

  std::string file_name_plain(file_name, 0,
                              file_name.size() - crypt_ext_.size());
  std::ofstream ofs(file_name_plain.c_str(), std::ios::binary);
  if (!ofs.is_open()) {
    std::cerr << "Failed to open " << file_name_plain << "." << std::endl;
    return false;
  }

  std::stringstream buffer;
  buffer << ifs.rdbuf();
  std::string read_content(buffer.str());

  std::string write_content;
  decrypt_string(read_content, write_content);
  ofs << write_content;

  return true;
}

bool AESCryptor::decrypt_file(const std::string &file_name,
                              FileBuffer &ofilebuffer) {
  FileBuffer ifilebuffer(file_name);
  return decrypt_file(ifilebuffer, ofilebuffer);
}

bool AESCryptor::decrypt_file(const FileBuffer &ifilebuffer,
                              FileBuffer &ofilebuffer) {
  std::string cypher;
  cypher.assign(ifilebuffer.GetBuffer(), ifilebuffer.GetLength());
  std::string content;
  decrypt_string(cypher, content);
  ofilebuffer.SetBuffer(content);

  return true;
}

/*bool AESCryptor::encrypt_folder(const std::string& folder) {
  if (0 != access(folder.c_str(), F_OK)) {
    std::cerr << "Input path " << folder << " does not exist." << std::endl;
    return false;
  }

  DIR* dirp = opendir(folder.c_str());
  if (!dirp) {
    std::cerr << "Failed to open folder." << std::endl;
    return false;
  }

  struct dirent* dir;
  struct stat st;
  if (-1 == lstat(folder.c_str(), &st)) {
    std::cout << "Failed to lstat file." << std::endl;
    closedir(dirp);
    return false;
  }

  if (S_ISDIR(st.st_mode)) {
    while (nullptr != (dir = readdir(dirp))) {
      if (0 == strcmp(dir->d_name, ".") || 0 == strcmp(dir->d_name, "..")) {
        continue;
      }

      std::string full_path = folder + '/' + dir->d_name;
      if (full_path.find(crypt_ext_) == full_path.size() - crypt_ext_.size()) {
        continue;
      }
      if (-1 == lstat(full_path.c_str(), &st)) {
        std::cout << "Failed to lstat file." << std::endl;
        closedir(dirp);
        return false;
      }

      if (S_ISDIR(st.st_mode)) {
        continue;
      } else if (S_ISREG(st.st_mode)) {
        encrypt_file(full_path);
      }
    }
  } else if (S_ISREG(st.st_mode)) {
    encrypt_file(folder);
  }

  closedir(dirp);

  return true;
}

bool AESCryptor::decrypt_folder(const std::string& folder) {
  if (0 != access(folder.c_str(), F_OK)) {
    std::cerr << "Input path " << folder << " does not exist." << std::endl;
    return false;
  }

  DIR* dirp = opendir(folder.c_str());
  if (!dirp) {
    std::cerr << "Failed to open folder." << std::endl;
    return false;
  }

  struct dirent* dir;
  struct stat st;
  if (-1 == lstat(folder.c_str(), &st)) {
    std::cout << "Failed to lstat file." << std::endl;
    closedir(dirp);
    return false;
  }

  if (S_ISDIR(st.st_mode)) {
    while (nullptr != (dir = readdir(dirp))) {
      if (0 == strcmp(dir->d_name, ".") || 0 == strcmp(dir->d_name, "..")) {
        continue;
      }

      std::string full_path = folder + '/' + dir->d_name;
      if (full_path.find(crypt_ext_) != full_path.size() - crypt_ext_.size()) {
        continue;
      }
      if (-1 == lstat(full_path.c_str(), &st)) {
        std::cout << "Failed to lstat file." << std::endl;
        closedir(dirp);
        return false;
      }

      if (S_ISDIR(st.st_mode)) {
        continue;
      } else if (S_ISREG(st.st_mode)) {
        decrypt_file(full_path);
      }
    }
  } else if (S_ISREG(st.st_mode)) {
    decrypt_file(folder);
  }

  closedir(dirp);

  return true;
}*/

std::string AESCryptor::hex_key() {
  std::string encoded;
  encoded.clear();
  CryptoPP::StringSource(
      key_, NBYTE_KEY, true,
      new CryptoPP::HexEncoder(new CryptoPP::StringSink(encoded)));
  return encoded;
}

void AESCryptor::print_key() {
  printf("key: ");
  for (auto a : key_) {
    printf("%c", a);
  }
  printf("\n");
}

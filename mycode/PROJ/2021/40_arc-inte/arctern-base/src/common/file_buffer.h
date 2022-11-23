/****************************************************************************
 *  Filename:       aes_cryptor.cpp
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.05.11
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#ifndef SRC_COMMON_FILE_BUFFER_H_
#define SRC_COMMON_FILE_BUFFER_H_
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
namespace arctern {

class FileBuffer {
 public:
  FileBuffer() : file_path_(""), buffer_(NULL), length_(0) {}

  explicit FileBuffer(const std::string &file_path) : file_path_(file_path) {
    std::ifstream ifs(file_path.c_str(), std::ios::in | std::ios::binary);
    if (!ifs) {
      std::cerr << "Can't open the file. Please check " << file_path << ". \n";
      length_ = 0;
      buffer_ = NULL;
      return;
    }
    ifs.seekg(0, std::ios::end);
    length_ = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    // std::cout << file_path.c_str() << " ... "<< length_ << " bytes\n";
    buffer_ = new char[sizeof(char) * length_];
    ifs.read(buffer_, length_);
    ifs.close();
  }

  FileBuffer(const char *buff, size_t len) {
    length_ = len;
    buffer_ = new char[sizeof(char) * length_];
    memcpy(buffer_, buff, sizeof(char) * length_);
  }

  FileBuffer(const FileBuffer &buf) {
    length_ = buf.length_;
    buffer_ = buf.buffer_;
    buffer_ = new char[sizeof(char) * length_];
    memcpy(buffer_, buf.buffer_, sizeof(char) * length_);
  }

  FileBuffer(FileBuffer &&buf) {
    length_ = buf.length_;
    buffer_ = buf.buffer_;
    buf.buffer_ = NULL;
    buf.length_ = 0;
  }

  FileBuffer &operator=(const FileBuffer &other) {  // copy assignment
    if (this != &other) {              // self-assignment check expected
      if (other.length_ != length_) {  // storage cannot be reused
        delete[] buffer_;              // destroy storage in this
        length_ = 0;
        buffer_ = nullptr;  // preserve invariants in case next line throws
        buffer_ = new char[other.length_];  // create storage in this
        length_ = other.length_;
      }
      std::copy(other.buffer_, other.buffer_ + other.length_, buffer_);
    }
    return *this;
  }

  FileBuffer &operator=(FileBuffer &&other) {  
    if (this != &other) {              
      if (0 != length_) {  
        delete[] buffer_;
        buffer_ = nullptr;             
        length_ = 0;   
      }
      buffer_ = other.buffer_;
      length_ = other.length_;
      other.buffer_ = nullptr;
      other.length_ = 0;
    }
    return *this;
  }

  int GetLength() const { return length_; }

  char *GetBuffer() const { return buffer_; }

  void SetBuffer(const std::string &buffer) {
    if (buffer_) {
      delete[] buffer_;
      buffer_ = NULL;
    }

    length_ = buffer.size();
    buffer_ = new char[sizeof(char) * length_];
    memcpy(buffer_, buffer.c_str(), sizeof(char) * length_);
  }

  void SetBufferFromFile(const std::string &file){
    FileBuffer filebuf(file);
    *this = std::move(filebuf);
  }

  ~FileBuffer() {
    if (buffer_) {
      delete[] buffer_;
      buffer_ = NULL;
    }
  }

  std::string file_path_;
  char *buffer_;
  int length_;

 private:
};

}  // namespace arctern

#endif  // SRC_COMMON_FILE_BUFFER_H_

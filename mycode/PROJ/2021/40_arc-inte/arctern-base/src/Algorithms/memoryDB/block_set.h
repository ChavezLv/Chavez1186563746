/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.28
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_MEMORYDB_BLOCKSET_H_
#define SRC_ALGORITHMS_MEMORYDB_BLOCKSET_H_

#include <iostream>
#include <cstddef>
#include <cstdint>
#include <vector>
#include <list>

namespace arctern {
#ifdef ERROR_EMPTY
#undef ERROR_EMPTY
#endif
enum BLOCK_SET_STATUS {
  ERROR_UNKNOWN       =  1,   /// < unknown error
  SUCCESS             =  0,     /// < success return
  ERROR_FULL          = -1,     /// < block is full
  ERROR_BAD_ALLOC     = -2,     /// < failed to allocate memory
  ERROR_EMPTY         = -3,     /// < block is empty
  ERROR_MISMATCH      = -4,     /// < size is mismatch
  ERROR_FEATLENGTH    = -5,     /// < feature length is mismatch
  ERROR_NOTFOUND      = -6,     /// < id is not found
  ERROR_INVALID       = -7,     /// < inputed data is invalid
};

class BlockSet {
public:
  BlockSet(size_t cap, size_t dim);

  BlockSet(BlockSet&& bs) noexcept;

  BlockSet(const BlockSet&) = delete;

  BlockSet& operator=(const BlockSet&) = delete;

  ~BlockSet();

  friend class MemoryDB;

  friend std::ostream& operator<<(std::ostream& os, const BlockSet& bs);

public:
  int64_t at(size_t idx) const { return id_.at(idx); }

  bool empty() const { return num_ == 0; }

  size_t size() const { return num_; }

  size_t capacity() const { return cap_; }

  BLOCK_SET_STATUS insert(const std::vector<int64_t>& id,
                          const std::vector<uint8_t>* feature);

  BLOCK_SET_STATUS insert(int64_t id, const std::vector<uint8_t>& feature);

  BLOCK_SET_STATUS update(int64_t id, const std::vector<uint8_t>& feature);

  void clear() { num_ = 0; }

  BLOCK_SET_STATUS erase(int64_t id);

  BLOCK_SET_STATUS compare(const std::vector<uint8_t>& feature,
                           std::vector<float>& similarity) const;

  BLOCK_SET_STATUS compare(const std::list<std::vector<uint8_t>>& feature,
                           std::list<std::vector<float>>& similarity) const;

 public:
  BLOCK_SET_STATUS allocate(size_t szlen);

  void deallocate();

  float* get(int64_t id);

 public:
  std::vector<int64_t> id_;       ///< set saves id(s)
  float* feature_ = nullptr;      ///< set saves feature(s)
  size_t num_ = 0;                ///< the number of feature(s)
  size_t dim_ = 0;              ///< dim of each feature, float as unit
  size_t cap_ = 0;           ///< capacity volume
};

} // namespace arctern

#endif  // SRC_ALGORITHMS_MEMORYDB_BLOCKSET_H_

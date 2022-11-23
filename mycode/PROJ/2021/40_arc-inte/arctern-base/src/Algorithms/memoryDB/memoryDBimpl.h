/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.28
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#ifndef SRC_ALGORITHMS_MEMORYDB_MEMORYDBIMPL_H_
#define SRC_ALGORITHMS_MEMORYDB_MEMORYDBIMPL_H_

#include "block_set.h"

namespace arctern {

class MemoryDBImpl {
public:
  static float compute_similarity(const std::vector<uint8_t> &encfeature1,
                                  const std::vector<uint8_t> &encfeature2);
 
  MemoryDBImpl(size_t cap, size_t dim);

  MemoryDBImpl(const MemoryDBImpl&) = delete;

  MemoryDBImpl& operator=(const MemoryDBImpl&) = delete;

  ~MemoryDBImpl() = default;

  friend std::ostream& operator<<(std::ostream& os, const MemoryDBImpl& mdb);

public:
  int64_t at(size_t idx) const;

  bool empty() const { return block_.empty(); }

  size_t size() const { return num_; }

  int insert(int64_t id, const std::vector<uint8_t>& feature);

  int insert(const std::vector<int64_t>& id, const std::vector<uint8_t>* feature);

  void clear() { block_.clear(); num_ = 0; }

  int erase(int64_t id);

  int update(int64_t id, const std::vector<uint8_t>& feature);

  int compare(const std::vector<uint8_t>& feature, std::vector<float>& similarity) const;

  int compare(const std::list<std::vector<uint8_t>>& feature,
              std::list<std::vector<float>>& similarity) const;

  std::vector<uint8_t> mean_merge(int64_t id, const std::vector<uint8_t>& feature);

 private:
  std::list<BlockSet> block_;     /// < set saves id(s)
  size_t num_ = 0;
  size_t dim_ = 0;                /// < dim of each feature, float as unit
  size_t cap_ = 512;         /// < capacity for each block for inserting
};

}   // namespace arctern

#endif  // SRC_ALGORITHMS_MEMORYDB_MEMORYDBIMPL_H_

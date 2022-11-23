/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.28
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include <iostream>
#include <cstring>
#include "memoryDBimpl.h"
#include "common/featureUtil.h"
#include <opencv2/core/core.hpp>

using arctern::MemoryDBImpl;

float MemoryDBImpl::compute_similarity(const std::vector<uint8_t> &encfeature1,
        const std::vector<uint8_t> &encfeature2) {
  // size > 16 is needed by caculate_feature_length in utils.cpp
  if (encfeature1.size() != encfeature2.size() || encfeature1.size() <= 16) {
    std::cerr << "[ArcternManager::compute_similarity]: two features size mismatch, or feature size too short" << std::endl;
    return 0;
  }

  std::vector<float> feat1 = decrypt_feature(encfeature1);
  std::vector<float> feat2 = decrypt_feature(encfeature2);
  cv::Mat mfeat1(feat1);
  cv::Mat mfeat2(feat2);

  return mfeat1.dot(mfeat2);
}

MemoryDBImpl::MemoryDBImpl(size_t cap, size_t dim) : cap_(cap), dim_(dim){
}

int64_t MemoryDBImpl::at(size_t idx) const {
  size_t q = idx / cap_;
  if (q > block_.size()) {
    std::cout << "Out of index" << std::endl;
    return -1;
  }
  auto itr = block_.cbegin();
  while (q--) {
    ++itr;
  }
  size_t r = idx % cap_;
  if (r > itr->size()) {
    std::cout << "Out of memory" << std::endl;
    return -1;
  }

  return itr->at(r);
}

int MemoryDBImpl::insert(int64_t id, const std::vector<uint8_t>& feature) {
  //TIME_DEBUG_START("[memdb::insert]");
  BLOCK_SET_STATUS retval = BLOCK_SET_STATUS::ERROR_UNKNOWN;
  if (block_.empty()) {
    BlockSet bs(cap_, dim_);
    auto res = bs.insert(id, feature);
    if (res == BLOCK_SET_STATUS::ERROR_FEATLENGTH) {
      return (int)res;
    }
    block_.push_back(std::move(bs));
  } else {
    auto itr = --block_.end();
    if (itr->size() < itr->capacity()) {
      auto res = itr->insert(id, feature);
      if (res == BLOCK_SET_STATUS::ERROR_FEATLENGTH) {
        return (int)res;
      }
    } else {
      BlockSet bs(cap_, dim_);
      auto res = bs.insert(id, feature);
      if (res == BLOCK_SET_STATUS::ERROR_FEATLENGTH) {
        return (int)res;
      }
      block_.push_back(std::move(bs));
    }
  }

  ++num_;

  //TIME_DEBUG_END("[memdb::insert]: ends");

  return 0;
}

int MemoryDBImpl::insert(const std::vector<int64_t>& id,
                     const std::vector<uint8_t>* feature) {
  size_t num_new_block = 0;
  size_t remind_block = 0;
  size_t offset = 0;
  if (!block_.empty()) {
    auto itr = --block_.end();
    size_t space = itr->capacity() - itr->size();
    if (space > id.size()) {
      num_ += id.size();
      return (int)(itr->insert(id, feature));
    } else {
      auto res = itr->insert(std::vector<int64_t>(id.cbegin(), id.cbegin() + space),
                  feature);
      if (res == BLOCK_SET_STATUS::ERROR_FEATLENGTH) {
        return (int)res;
      }
      offset = space;
    }
  }

  num_new_block = (id.size() - offset) / cap_;
  remind_block = (id.size() - offset) % cap_;

  for (size_t i = 0; i < num_new_block; ++i) {
    BlockSet bs(cap_, dim_);
    auto res = bs.insert(std::vector<int64_t>(id.cbegin() + offset + i * cap_,
                  id.cbegin() + offset + (i + 1) * cap_),
              feature + offset + i * cap_);
    if (res == BLOCK_SET_STATUS::ERROR_FEATLENGTH) {
      return (int)res;
    }
    block_.push_back(std::move(bs));
  }

  if (remind_block) {
    BlockSet bs(cap_, dim_);
    auto res = bs.insert(std::vector<int64_t>(id.cbegin() + offset + num_new_block * cap_,
                  id.cbegin() + offset + num_new_block * cap_ + remind_block),
              feature + offset + num_new_block * cap_);
    if (res == BLOCK_SET_STATUS::ERROR_FEATLENGTH) {
      return (int)res;
    }
    block_.push_back(std::move(bs));
  }

  num_ += id.size();

  return 0;
}

int MemoryDBImpl::erase(int64_t id) {
  BLOCK_SET_STATUS retval = BLOCK_SET_STATUS::ERROR_UNKNOWN;
  auto itr = block_.begin();
  for ( ; itr != block_.end(); ++itr) {
    retval = itr->erase(id);
    if (BLOCK_SET_STATUS::ERROR_NOTFOUND != retval) {
      break;
    }
  }

  if (BLOCK_SET_STATUS::SUCCESS == retval) {
    if (--num_) {
      auto litr = --block_.end();
      memcpy(itr->feature_ + dim_ * itr->num_, litr->feature_ + dim_ * (litr->num_ -1), dim_);
      itr->id_[itr->num_] = litr->id_[litr->num_ - 1];
      ++itr->num_;
      --litr->num_;
      if (litr->empty()) {
        block_.erase(litr);
      }
    } else {
      block_.erase(itr);
    }

    return 0;
  }

  return -6;
}

int MemoryDBImpl::update(int64_t id, const std::vector<uint8_t>& feature) {
  BLOCK_SET_STATUS retval = BLOCK_SET_STATUS::ERROR_UNKNOWN;
  for (auto itr = block_.begin(); itr != block_.end(); ++itr) {
    retval = itr->update(id, feature);
    if (BLOCK_SET_STATUS::SUCCESS == retval) {
      break;
    }
  }

  if (BLOCK_SET_STATUS::SUCCESS != retval) {
    return (int)retval;
  }

  return 0;
}

int MemoryDBImpl::compare(const std::vector<uint8_t>& feature,
                      std::vector<float>& similarity) const {
  //TIME_DEBUG_START("[memdb::compare]");
  BLOCK_SET_STATUS retval = BLOCK_SET_STATUS::ERROR_UNKNOWN;
  similarity.resize(num_);
  auto sim_itr = similarity.begin();
  for (auto itr = block_.cbegin(); itr != block_.cend(); ++itr) {
    std::vector<float> block_sim;
    retval = itr->compare(feature, block_sim);
    if (BLOCK_SET_STATUS::SUCCESS != retval) {
      similarity.clear();
      return (int)retval;
    }
    std::copy(block_sim.cbegin(), block_sim.cend(), sim_itr);
    sim_itr += itr->size();
  }
  //TIME_DEBUG_END("[memdb::compare]: ends");

  return 0;
}

int MemoryDBImpl::compare(const std::list<std::vector<uint8_t>>& feature,
                      std::list<std::vector<float>>& similarity) const {
  BLOCK_SET_STATUS retval = BLOCK_SET_STATUS::ERROR_UNKNOWN;
  similarity.resize(feature.size());
  for (auto& a : similarity) {
    a.resize(num_);
  }

  size_t copy_offset = 0;
  for (auto itr = block_.cbegin(); itr != block_.cend(); ++itr) {
    std::list<std::vector<float>> block_sim;
    retval = itr->compare(feature, block_sim);
    if (BLOCK_SET_STATUS::SUCCESS != retval) {
      similarity.clear();
      return (int)retval;
    }

    auto sim_itr = similarity.begin();
    for (auto blk_itr = block_sim.cbegin();
         blk_itr != block_sim.cend(); ++blk_itr, ++sim_itr) {
      std::copy(blk_itr->cbegin(), blk_itr->cend(), sim_itr->begin() + copy_offset);
    }

    copy_offset += itr->size();
  }

  return 0;
}

std::vector<uint8_t> MemoryDBImpl::mean_merge(int64_t id, const std::vector<uint8_t>& feature) {
  float* pfeat = nullptr;
  for (auto& a : block_) {
    if (nullptr != (pfeat = a.get(id))) {
      break;
    }
  }

  if (nullptr == pfeat) {   // no id is found
    return std::vector<uint8_t>();
  }

  auto plain_feature = arctern::decrypt_feature(feature);
  if (plain_feature.size() != dim_) {   // the dim of feature to be merged is mismatch
    return std::vector<uint8_t>();
  }

  for (size_t i = 0; i < dim_; ++i) {
    pfeat[i] = (pfeat[i] + plain_feature[i]) * 0.5;    // 0.5 is best weight value
  }

  l2norm(pfeat, dim_);

  return arctern::encrypt_feature(std::vector<float>(pfeat, pfeat + dim_));
}

namespace arctern {
std::ostream& operator<<(std::ostream& os, const MemoryDBImpl& mdb) {
  os << "Num of MemDB blocks  : " << mdb.block_.size() << std::endl
     << "Num of MemDB features: " << mdb.num_ << std::endl
     << "MemDB Dimension      : " << mdb.dim_ << std::endl
     << "MemDB Capacity def   : " << mdb.cap_ << std::endl;
  for (const auto& a : mdb.block_) {
    os << a << std::endl;
  }

  return os;
}

} // namespace arctern

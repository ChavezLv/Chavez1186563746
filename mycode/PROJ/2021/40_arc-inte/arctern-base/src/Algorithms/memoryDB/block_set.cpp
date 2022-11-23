/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.28
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include <iostream>

#ifdef USE_OPENBLAS
#include <cblas.h>
#else
#include <opencv2/core/core.hpp>
#endif

#include "common/encryx.h"

#include "block_set.h"

using arctern::BlockSet;
using arctern::BLOCK_SET_STATUS;

BlockSet::BlockSet(size_t cap, size_t dim) : cap_(cap), dim_(dim) {
  if (BLOCK_SET_STATUS::SUCCESS != allocate(cap_ * dim_)) {
    throw std::runtime_error("Failed to allocate memory");
  }
  id_.resize(cap_);
}

BlockSet::BlockSet(BlockSet&& bs) noexcept : num_(bs.num_), dim_(bs.dim_),
    cap_(bs.cap_), id_(std::move(bs.id_)), feature_(bs.feature_) {
  bs.num_ = 0;
  bs.dim_ = 0;
  bs.cap_ = 0;
  bs.id_.clear();
  bs.feature_ = nullptr;
}

BlockSet::~BlockSet() {
  deallocate();
}

BLOCK_SET_STATUS BlockSet::insert(const std::vector<int64_t>& id,
      const std::vector<uint8_t>* feature) {
  if (id.size() > cap_ - num_) {
    return BLOCK_SET_STATUS::ERROR_FULL;
  }

  for (size_t i = 0; i < id.size(); ++i) {
    id_[num_ + i] = id[i];
    if (encry2x(feature_ + (num_ + i) * dim_, dim_,
                const_cast<uint8_t*>(feature[i].data()), feature[i].size()) == 
                ENCRYX_STATUS::ERROR_CODELENGTH_MISMATCH) {
      return BLOCK_SET_STATUS::ERROR_FEATLENGTH;
    }
  }
  num_ += id.size();

  return BLOCK_SET_STATUS::SUCCESS;
}

BLOCK_SET_STATUS BlockSet::insert(int64_t id,
      const std::vector<uint8_t>& feature) {
  if (num_ == cap_) {
    return BLOCK_SET_STATUS::ERROR_FULL;
  }

  id_[num_] = id;
  if (encry2x(feature_ + num_ * dim_, dim_,
              const_cast<uint8_t*>(feature.data()), feature.size()) ==
              ENCRYX_STATUS::ERROR_CODELENGTH_MISMATCH) {
    return BLOCK_SET_STATUS::ERROR_FEATLENGTH;
  }
  ++num_;

  return BLOCK_SET_STATUS::SUCCESS;
}

BLOCK_SET_STATUS BlockSet::update(int64_t id,
      const std::vector<uint8_t>& feature) {
  auto pos = std::find(id_.begin(), id_.end(), id);
  if (pos == id_.end()) {
    return BLOCK_SET_STATUS::ERROR_NOTFOUND;
  }

  float *p_add_pos = feature_ + std::distance(id_.begin(), pos) * dim_;
  if (encry2x(p_add_pos, dim_,
              const_cast<uint8_t*>(feature.data()), feature.size()) == 
              ENCRYX_STATUS::ERROR_CODELENGTH_MISMATCH) {
    return BLOCK_SET_STATUS::ERROR_FEATLENGTH;
  }

  return BLOCK_SET_STATUS::SUCCESS;
}

BLOCK_SET_STATUS BlockSet::erase(int64_t id) {
  auto pos = std::find(id_.begin(), id_.end(), id);
  if (pos == id_.end()) {
    return BLOCK_SET_STATUS::ERROR_NOTFOUND;
  }

  if (--num_) {
    auto dist = std::distance(id_.begin(), pos);
    *pos = id_[num_];
    memcpy(feature_ + dist * dim_, feature_ + num_ * dim_,
           dim_ * sizeof(float));
  }

  return BLOCK_SET_STATUS::SUCCESS;
}

BLOCK_SET_STATUS
BlockSet::compare(const std::vector<uint8_t>& feature,
                  std::vector<float>& similarity) const {
  //auto aimfeat = decrypt_feature(feature);
  std::vector<float> aimfeat(dim_);
  if (encry2x(&aimfeat[0], dim_,
              const_cast<uint8_t*>(&feature[0]), feature.size()) == 
              ENCRYX_STATUS::ERROR_CODELENGTH_MISMATCH) {
    return BLOCK_SET_STATUS::ERROR_FEATLENGTH;
  }
  
#if USE_OPENBLAS
  similarity.resize(num_);
  cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasTrans, 1, num_, dim_, 1.0,
              aimfeat.data(), dim_, feature_, dim_, 0.0,
              similarity.data(), num_);
#else
  cv::Mat cvmat_set(num_, dim_, CV_32F, feature_);
  cv::Mat cvmat(dim_, 1, CV_32F, aimfeat.data());
  cv::Mat result = cvmat_set * cvmat;
  float *p = result.ptr<float>(0);
  similarity.assign(p, p + num_);
#endif

  return BLOCK_SET_STATUS::SUCCESS;
}

BLOCK_SET_STATUS
BlockSet::compare(const std::list<std::vector<uint8_t>>& feature,
                  std::list<std::vector<float>>& similarity) const {
  float* aimfeat = nullptr;
  size_t szfeat = feature.size();
  try {
    aimfeat = new float[dim_ * szfeat];
  } catch (const std::bad_alloc& e) {
    std::cerr << "Failed to allocate memory: " << e.what() << std::endl;
    return BLOCK_SET_STATUS::ERROR_BAD_ALLOC;
  } catch (...) {
    std::cerr << "Unknown error" << std::endl;
    return BLOCK_SET_STATUS::ERROR_UNKNOWN;
  }

  size_t i = 0;
  for (auto itr = feature.cbegin(); itr != feature.cend(); ++itr, ++i) {
    if (encry2x(aimfeat + i * dim_, dim_,
                const_cast<uint8_t*>(itr->data()), itr->size()) == 
                ENCRYX_STATUS::ERROR_CODELENGTH_MISMATCH) {
      return BLOCK_SET_STATUS::ERROR_FEATLENGTH;
    }
  }

  similarity.clear();
#if USE_OPENBLAS
  float* sim = nullptr;
  try {
    sim = new float[feature.size() * num_];
  } catch (const std::bad_alloc& e) {
    std::cerr << "Failed to allocate memory: " << e.what() << std::endl;
    return BLOCK_SET_STATUS::ERROR_BAD_ALLOC;
  } catch (...) {
    std::cerr << "Unknown error" << std::endl;
    return BLOCK_SET_STATUS::ERROR_UNKNOWN;
  }
  cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasTrans, feature.size(), num_,
              dim_, 1.0, aimfeat, dim_, feature_, dim_, 0.0, sim, num_);
  for (size_t i = 0; i < feature.size(); ++i) {
    std::vector<float> vec(sim + i * num_, sim + (i + 1) * num_);
    similarity.push_back(std::move(vec));
  }
  delete[] sim;

#else
  cv::Mat cvmat_set(num_, dim_, CV_32F, feature_);
  cv::Mat cvmat_aim(szfeat, dim_, CV_32F, aimfeat);
  cv::transpose(cvmat_aim, cvmat_aim);
  cv::Mat result = cvmat_set * cvmat_aim;
  cv::transpose(result, result);
  for (size_t i = 0; i < szfeat; ++i) {
    float* p = result.ptr<float>(i);
    std::vector<float> vec(p, p + num_);
    similarity.push_back(std::move(vec));
  }
#endif

  delete[] aimfeat;
  aimfeat = nullptr;

  return BLOCK_SET_STATUS::SUCCESS;
}

BLOCK_SET_STATUS BlockSet::allocate(size_t szlen) {
  float *p = nullptr;
  try {
    p = new float[szlen];
  } catch (const std::bad_alloc& e) {
    std::cerr << "Failed to allocate memory: " << e.what() << std::endl;
    return BLOCK_SET_STATUS::ERROR_BAD_ALLOC;
  } catch (...) {
    std::cerr << "Unknown error" << std::endl;
    return BLOCK_SET_STATUS::ERROR_UNKNOWN;
  }

  if (nullptr != feature_) {
    delete[] feature_;
  }

  feature_ = p;

  return BLOCK_SET_STATUS::SUCCESS;
}

void BlockSet::deallocate() {
  if (nullptr != feature_) {
    delete[] feature_;
    feature_ = nullptr;
    id_.clear();
    num_ = 0;
    cap_ = 0;
    dim_ = 0;
  }
}

float* BlockSet::get(int64_t id) {
  auto pos = std::find(id_.cbegin(), id_.cend(), id);
  return (pos == id_.cend() ?
        nullptr : feature_ + dim_ * std::distance(id_.cbegin(), pos));
}

namespace arctern {
std::ostream& operator<<(std::ostream& os, const BlockSet& bs) {
  os << "Capacity of block ids: " << bs.id_.size() << std::endl
     << "Num of block features: " << bs.num_ << std::endl
     << "Block Dimension      : " << bs.dim_ << std::endl
     << "Block Capacity       : " << bs.cap_ << std::endl
     << "Block Details        : " << std::endl;
  for (size_t i = 0; i < bs.num_; ++i) {
    os << "  " << bs.id_[i] << " ";
    for (size_t j = 0; j < 4; ++j) {
      os << bs.feature_[bs.dim_ * i + j] << " ";
    }
    std::cout << std::endl;
  }

  return os;
}

} // namespace arctern

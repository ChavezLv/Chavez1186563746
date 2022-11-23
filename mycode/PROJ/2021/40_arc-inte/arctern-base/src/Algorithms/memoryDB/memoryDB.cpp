/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.28
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/

#include "memoryDB.h"
#include "memoryDBimpl.h"

using arctern::ErrorType;
using arctern::MemoryDB;
using arctern::MemoryDBImpl;

static ErrorType int2ErrorType(int code) {
  switch (code) {
    case 0  : return ErrorType::ERR_SUCCESS;
    case 1  : return ErrorType::ERR_UNEXPECT;
    case -1 : return ErrorType::ERR_MEMDB_FULL;
    case -2 : return ErrorType::ERR_MEMDB_ALLOC;
    case -3 : return ErrorType::ERR_MEMDB_EMPTY;
    case -4 : return ErrorType::ERR_MEMDB_MISMATCH;
    case -5 : return ErrorType::ERR_MEMDB_FEATLENGTH;
    case -6 : return ErrorType::ERR_MEMDB_NOTFOUND;
    case -7 : return ErrorType::ERR_MEMDB_INVALID;
    default : return ErrorType::ERR_SUCCESS;
  }
}

float arctern::compute_similarity(const std::vector<uint8_t> &encfeature1,
        const std::vector<uint8_t> &encfeature2) {
    return MemoryDBImpl::compute_similarity(encfeature1, encfeature2);
}

class MemoryDB::Impl {
 public:
  friend MemoryDB;
  Impl() {}
  std::shared_ptr<MemoryDBImpl> memDBImpl_ = nullptr;
};

MemoryDB::MemoryDB() : impl_(nullptr) {
}

MemoryDB::~MemoryDB() noexcept {
}

ErrorType MemoryDB::Init(size_t cap, size_t dim) {
  Release();
  impl_ = std::make_shared<Impl>();
  impl_->memDBImpl_ = std::make_shared<arctern::MemoryDBImpl>(cap, dim);
  return ErrorType::ERR_SUCCESS;
}

ErrorType MemoryDB::feature_memdb_insert(int64_t id,
        const std::vector<uint8_t>& feature) {
  if ( impl_ == nullptr || nullptr == impl_->memDBImpl_ ) {
    return ErrorType::ERR_NO_INIT;
  }
  auto ret0 = impl_->memDBImpl_->insert(id, feature);
  return int2ErrorType(ret0);
}

ErrorType MemoryDB::feature_memdb_insert(const std::vector<int64_t>& id,
                           const std::vector<uint8_t>* feature) {
  if ( impl_ == nullptr || nullptr == impl_->memDBImpl_ ) {
    return ErrorType::ERR_NO_INIT;
  }
  auto ret0 = impl_->memDBImpl_->insert(id, feature);
  return int2ErrorType(ret0);
}

ErrorType MemoryDB::feature_memdb_erase(int64_t id) {
  if ( impl_ == nullptr || nullptr == impl_->memDBImpl_ ) {
    return ErrorType::ERR_NO_INIT;
  }
  auto ret0 = impl_->memDBImpl_->erase(id);
  return int2ErrorType(ret0);
}

void MemoryDB::feature_memdb_clear() {
  if ( impl_ == nullptr || nullptr == impl_->memDBImpl_) {
    return;
  }
  impl_->memDBImpl_->clear();
}

ErrorType MemoryDB::feature_memdb_update(int64_t id,
        const std::vector<uint8_t>& feature) {
  if ( impl_ == nullptr || nullptr == impl_->memDBImpl_ ) {
    return ErrorType::ERR_NO_INIT;
  }
  auto ret0 = impl_->memDBImpl_->update(id, feature);
  return int2ErrorType(ret0);
}

ErrorType MemoryDB::feature_memdb_compare(const std::vector<uint8_t>& feature,
        std::vector<float>& similarity) const {
  if ( impl_ == nullptr || nullptr == impl_->memDBImpl_ ) {
    return ErrorType::ERR_NO_INIT;
  }
  auto ret0 = impl_->memDBImpl_->compare(feature, similarity);
  return int2ErrorType(ret0);
}

ErrorType MemoryDB::feature_memdb_compare(
        const std::list<std::vector<uint8_t>>& feature,
        std::list<std::vector<float>>& similarity) const {
  if ( impl_ == nullptr || nullptr == impl_->memDBImpl_ ) {
    return ErrorType::ERR_NO_INIT;
  }
  auto ret0 = impl_->memDBImpl_->compare(feature, similarity);
  return int2ErrorType(ret0);
}

int64_t MemoryDB::feature_memdb_at(size_t idx) const {
  if ( impl_ == nullptr || nullptr == impl_->memDBImpl_ ) {
    return -1;
  }
  return impl_->memDBImpl_->at(idx);
}

bool MemoryDB::feature_memdb_empty() const {
  if ( impl_ == nullptr || nullptr == impl_->memDBImpl_ ) {
    return true;
  }
  return impl_->memDBImpl_->empty();
}

size_t MemoryDB::feature_memdb_size() const {
  if ( impl_ == nullptr || nullptr == impl_->memDBImpl_ ) {
    return 0;
  }
  return impl_->memDBImpl_->size();
}

std::vector<uint8_t> MemoryDB::feature_memdb_mean_merge(int64_t id,
        const std::vector<uint8_t>& feature) {
  if ( impl_ == nullptr || nullptr == impl_->memDBImpl_ ) {
    return std::vector<uint8_t>();
  }
  return impl_->memDBImpl_->mean_merge(id, feature);
}

void MemoryDB::Release() {
  if ( impl_ == nullptr || nullptr == impl_->memDBImpl_) {
    return;
  }
  impl_->memDBImpl_->clear();
}

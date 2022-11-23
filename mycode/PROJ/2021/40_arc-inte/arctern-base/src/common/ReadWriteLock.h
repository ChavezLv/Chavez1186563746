//
// Created by Admin on 2021/9/16.
//

#ifndef ARCTERN_BASE_SRC_COMMON_READWRITELOCK_H_
#define ARCTERN_BASE_SRC_COMMON_READWRITELOCK_H_
#include <atomic>
#include <thread>
#include <cassert>
class ReadWriteLock{
#define WRITE_LOCK_STATUS -1
#define FREE_LOCK_STATUS 0
 public:
  ReadWriteLock(bool bWriteFirst = true):
  m_bWriteFirst(bWriteFirst),
  m_lockCount(0),
  m_currentWriteThreadId(m_id),
  m_waitWriteLockCount(0){
  }
  ~ReadWriteLock() = default;
  ReadWriteLock(const ReadWriteLock&) = delete;
  ReadWriteLock(ReadWriteLock &&) = delete;
  ReadWriteLock& operator=(const ReadWriteLock&) =delete;
  ReadWriteLock& operator=(ReadWriteLock&&) =delete;

  int ReadLock(){
    if(std::this_thread::get_id() != m_currentWriteThreadId){
      int tmpCount;
      if(m_bWriteFirst){
        do{
          tmpCount = m_lockCount;
          while((m_lockCount == WRITE_LOCK_STATUS) || m_waitWriteLockCount > 0);
        }while(!m_lockCount.compare_exchange_weak(tmpCount,tmpCount+1));
      }
      else{
        do{
          tmpCount = m_lockCount;
          while((m_lockCount == WRITE_LOCK_STATUS));
        }while(!m_lockCount.compare_exchange_weak(tmpCount,tmpCount+1));
      }
    }
    return m_lockCount;
  }
  int ReadUnLock(){
    if(std::this_thread::get_id() != m_currentWriteThreadId)
    {
      --m_lockCount;
    }
    return m_lockCount;
  }

  int WriteLock(){
    if(std::this_thread::get_id() == m_currentWriteThreadId) return m_lockCount;
    int zero = 0;
    while(!m_lockCount.compare_exchange_weak(zero,WRITE_LOCK_STATUS)){
      zero = 0;
    }
    m_currentWriteThreadId = std::this_thread::get_id();
  }

  int WriteUnlock(){
    if(std::this_thread::get_id() != m_currentWriteThreadId) return m_lockCount;
    m_currentWriteThreadId = m_id;
    m_lockCount.store(FREE_LOCK_STATUS);
    return m_lockCount;
  }
 private:
  ///> -1 is write lock,0 is free, >0 is read lock count
  std::atomic<int> m_lockCount;
  ///> if write lock first
  bool m_bWriteFirst;
  ///> wait to get write lock
  std::atomic<unsigned int> m_waitWriteLockCount;
  ///> save current write thread id
  std::thread::id m_currentWriteThreadId;
  static std::thread::id m_id;
};

class ReadLock_Guard{
 public:
   explicit ReadLock_Guard(ReadWriteLock *read_write_lock):
   read_write_lock_(read_write_lock){
     read_write_lock_->ReadLock();
   }
   ~ReadLock_Guard(){
     read_write_lock_->ReadUnLock();
   }
  ReadLock_Guard() = delete;
  ReadLock_Guard(const ReadLock_Guard&) = delete;
  ReadLock_Guard& operator=(const ReadLock_Guard&) = delete;
  ReadLock_Guard(ReadLock_Guard&&) = delete;
  ReadLock_Guard& operator=(ReadLock_Guard&&) = delete;

 private:
  ReadWriteLock *read_write_lock_;
};

class WriteLock_Guard{
 public:
  explicit  WriteLock_Guard(ReadWriteLock *read_write_lock):
  read_write_lock_(read_write_lock){
    read_write_lock_->WriteLock();
  }
  ~WriteLock_Guard(){
    read_write_lock_->WriteUnlock();
  }
  WriteLock_Guard() = delete;
  WriteLock_Guard(const WriteLock_Guard&) =delete;
  WriteLock_Guard(WriteLock_Guard&&) =delete;
  WriteLock_Guard& operator=(const WriteLock_Guard&)=delete;
  WriteLock_Guard& operator=(WriteLock_Guard&&) =delete;
 private:
  ReadWriteLock* read_write_lock_;
};

#endif //ARCTERN_BASE_SRC_COMMON_READWRITELOCK_H_

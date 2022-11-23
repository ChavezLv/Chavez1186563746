/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file     if_common.hpp
/// @brief    公用定义C++头文件
/// @author   caoyan
/// @date     2019/8/21
/// @version  0.0.1
/// @par      Copyright(C) 2018, Shenzhen Intellifusion Technologies Co.Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef IF_COMMON_HPP
#define IF_COMMON_HPP

#include "if_common.h"
#include "if_log.h"
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <chrono>
#include <vector>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include "media_common.h"

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(classname) \
private:                                   \
    classname(const classname &);             \
    classname &operator=(const classname &);
#endif

class Image {

 public:
  Image() {
    image_.data = nullptr;
    image_.extra_data[0] = nullptr;
    image_.extra_data[1] = nullptr;
  }
  Image(if_image_t &image) : image_(image) {}
  Image(uint8_t *data, int width, int height) {
    image_.data = data;
    image_.extra_data[0] = nullptr;
    image_.extra_data[1] = nullptr;
    image_.width = width;
    image_.height = height;
  }

  int width() {return image_.width;}
  int height() {return image_.height;}
  uint64_t time_stamp() {return image_.time_stamp;}
  int format() {return image_.format;}
  int size() {return image_.size;}
  uint8_t* data() {return image_.data;}
  if_image_t& image() {return image_;}
  void set_path(char* path){path_= path;}
  std::string path(){return path_;}

  ~Image() {
    //extra_data需手动释放
    if (image_.data) {
      delete[] image_.data;
      image_.data = nullptr;
    }
  }
  intellif::DisplayInfo info_;

 private:
  if_image_t image_;
  std::string path_;
};

typedef std::shared_ptr<Image> SPImage;

struct SPImageCmp{
    bool operator() ( SPImage a, SPImage b ){return a->time_stamp()< b->time_stamp();}
};

//threads
typedef std::thread                     if_thread_t;
typedef std::shared_ptr<if_thread_t>    if_sp_thread_t;
#define if_bind                         std::bind

template <typename T>
using if_function = std::function<T>;
template <typename T>
using if_vector = std::vector<T>;
template <typename T>
using if_list = std::list<T>;
template <typename T>
using if_set = std::set<T>;
template <typename TK, typename TV>
using if_map = std::map<TK, TV>;
template <typename TK, typename TV>
using if_unordered_map = std::unordered_map<TK, TV>;
template <typename T1, typename T2>
using if_pair = std::pair<T1, T2>;


//mutex
typedef std::mutex if_mutex_t;

//cond
typedef std::condition_variable if_cond_t;
#define cond_time_wait(cond, lock, millSec) (cond.wait_for(lock, std::chrono::milliseconds(millSec)))
#define cond_time_out (std::cv_status::timeout)

#define if_make_shared std::make_shared

#define if_dynamic_pointer_cast std::dynamic_pointer_cast

template <typename T>
using if_shared_ptr = std::shared_ptr<T>;

template <typename T>
using if_weak_ptr = std::weak_ptr<T>;

/**
 * @brief just Reference resources
 * https://stackoverflow.com/questions/26583433/c11-implementation-of-spinlock-using-atomic
 */
typedef class spin_mutex
{
public:
    spin_mutex() = default;
    ~spin_mutex() = default;
    spin_mutex(const spin_mutex&) = delete;
    spin_mutex& operator= (const spin_mutex&) = delete;

    void lock()
    {
        while (m_flag.test_and_set(std::memory_order_acquire))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    void unlock() {
        m_flag.clear(std::memory_order_release);
    }
private:
    std::atomic_flag m_flag = ATOMIC_FLAG_INIT;
} if_spin_mutex_t;

typedef std::lock_guard<if_spin_mutex_t>  if_spin_lock_t;



#endif //IF_COMMON_HPP


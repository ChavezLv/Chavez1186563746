#ifndef SRC_ARCTERN_UTILS_H_
#define SRC_ARCTERN_UTILS_H_

#include <numeric>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>

#include <opencv2/core/core.hpp>
#include "../define.h"
//#include "arctern_manager.h"


/// \brief the wrap of assert for printing log message
#ifdef WIN32
#define ARCTERN_ASSERT_MSG(X, MSG)                            \
  do{                                                         \
    if(!(X)){                                                 \
      std::cerr << "Arctern assertion " << "'" << #X << "'"   \
        << " failed in " << __FUNCTION__ << " at "    \
         << __FILE__ << ":" << __LINE__ << ";details:"        \
         << MSG << std::endl;                                 \
      abort();                                                \
    }                                                         \
}while(false)
#else
#define ARCTERN_ASSERT_MSG(X, MSG)                            \
  do{                                                         \
    if(!(X)){                                                 \
      std::cerr << "Arctern assertion " << "'" << #X << "'"   \
        << " failed in " << __PRETTY_FUNCTION__ << " at "    \
         << __FILE__ << ":" << __LINE__ << ";details:"        \
         << MSG << std::endl;                                 \
      abort();                                                \
    }                                                         \
}while(false)
#endif



#ifndef uint8
typedef unsigned char uint8;
#endif


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


#ifndef TIME_DEBUG_MACROS
#define TIME_DEBUG_MACROS

#ifdef WITH_TIME_DEBUG
  #define TIME_DEBUG_START(xxx) \
    std::string start_xxx = xxx; \
    auto start_time = std::chrono::high_resolution_clock::now();\
    auto start_time_0 = start_time; \
    auto end_time = start_time; \
    std::chrono::duration<double> elapsed_time; \
    std::vector<std::pair<std::string, double>> elapsed_time_points_vector;
  
  #define TIME_DEBUG_LOG(xxx) \
    end_time = std::chrono::high_resolution_clock::now(); \
    elapsed_time = end_time - start_time; \
    start_time = end_time; \
    elapsed_time_points_vector.push_back({std::string(xxx), elapsed_time.count()});
  
  #define TIME_DEBUG_END(xxx) \
    end_time = std::chrono::high_resolution_clock::now(); \
    elapsed_time = end_time - start_time; \
    start_time = end_time; \
    elapsed_time_points_vector.push_back({std::string(xxx), elapsed_time.count()}); \
    double accu_elapsed_time = 0; \
    std::cout << ">>>> begin results for " << start_xxx << std::endl; \
    for (const std::pair<std::string, double> &elapsed_time_point_pair: elapsed_time_points_vector) { \
      accu_elapsed_time += elapsed_time_point_pair.second; \
      std::cout << elapsed_time_point_pair.first << ": elapsed: " << elapsed_time_point_pair.second << ", accu: " << accu_elapsed_time << std::endl; \
    } \
    std::cout << "<<<< end results for " << start_xxx << std::endl;
#else // ifdef WITH_TIME_DEBUG
  #define TIME_DEBUG_START(xxx) 
  #define TIME_DEBUG_LOG(xxx) 
  #define TIME_DEBUG_END(xxx) 
#endif // ifdef WITH_TIME_DEBUG

#ifdef WITH_TIME_DEBUG_LOOP
  #define TIME_DEBUG_LOOP_START(xxx, n) \
    { \
      std::cout << xxx << ": " << std::endl; \
      double time_loop_total_elapsed_time = 0; \
      int time_loop_total_count = n; \
      for (int time_loop_i = 0; time_loop_i < n; ++time_loop_i) { \
        auto time_loop_start_time = std::chrono::high_resolution_clock::now(); \
        {
  
  #define TIME_DEBUG_LOOP_END(xxx) \
        } \
        auto time_loop_end_time = std::chrono::high_resolution_clock::now(); \
        std::chrono::duration<double> time_loop_elapsed_time = time_loop_end_time - time_loop_start_time; \
        time_loop_total_elapsed_time += time_loop_elapsed_time.count(); \
      } \
      std::cout << xxx << ": total_elapsed: " << time_loop_total_elapsed_time << ", average: " << (time_loop_total_elapsed_time / time_loop_total_count) << std::endl; \
    }
#else // ifdef WITH_TIME_DEBUG_LOOP
  #define TIME_DEBUG_LOOP_START(xxx, n)
  #define TIME_DEBUG_LOOP_END(xxx)
#endif // ifdef WITH_TIME_DEBUG_LOOP

#endif // ifndef TIME_DEBUG_MACROS

#ifndef to_string
namespace std {
template <typename T>
std::string to_string(T value) {
  std::ostringstream os;
  os << value;
  return os.str();
}
}  // namespace std
#endif



#endif  // SRC_ARCTERN_UTILS_H_

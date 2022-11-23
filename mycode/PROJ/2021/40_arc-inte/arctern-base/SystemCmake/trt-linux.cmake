set(CMAKE_CUDA_COMPILER "/usr/local/cuda/bin/nvcc")
set(CMAKE_CUDA_COMPILER_ENV_VAR "/usr/local/cuda/bin/nvcc")
set(CMAKE_CUDA_ARCHITECTURES "53")
enable_language("CUDA")

set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS}  --default-stream per-thread -Xcompiler -fPIC -std=c++11")
message(STATUS "CUDA NVCC target flags: ${CMAKE_CUDA_FLAGS}")

find_package(PkgConfig)

pkg_search_module(CUDA REQUIRED cuda)
include_directories(${CUDA_INCLUDE_DIRS})
link_directories(${CUDA_LIBRARY_DIRS})

pkg_search_module(TENSORRT REQUIRED tensorrt)
include_directories(${TENSORRT_INCLUDE_DIRS})
link_directories(${TENSORRT_LIBRARY_DIRS})

pkg_search_module(OPENCV REQUIRED opencv)
include_directories(${OPENCV_INCLUDE_DIRS})
link_directories(${OPENCV_LIBRARY_DIRS})


set(THIRD_PARTY_INC_PATH "${THIRD_PARTY_LIB_ROOT}/include")
set(THIRD_PARTY_LIB_ROOT_PATH "${THIRD_PARTY_LIB_ROOT}/lib/linux")
set(THIRD_PARTY_LIB_PATH "${THIRD_PARTY_LIB_ROOT_PATH}/gcc")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -msse4.1")

add_definitions(-DTRT_ALLOWED)


#-------------------- set third libs  --------------------
set(THIRD_PARTY_INC_PATH "${THIRD_PARTY_LIB_ROOT}/include")
set(THIRD_PARTY_LIB_ROOT_PATH "${THIRD_PARTY_LIB_ROOT}/lib/linux")
set(THIRD_PARTY_LIB_PATH "${THIRD_PARTY_LIB_ROOT_PATH}/gcc")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -msse4.1")

link_directories(${THIRD_PARTY_LIB_PATH})
link_directories(${THIRD_PARTY_LIB_PATH}/opencv/3rdparty)
link_directories(${THIRD_PARTY_LIB_PATH}/opencv/cpu)
link_directories(${THIRD_PARTY_LIB_PATH}/openblas)
link_directories(${THIRD_PARTY_LIB_PATH}/yaml-cpp)
link_directories(${THIRD_PARTY_LIB_PATH}/cryptopp)
link_directories(${THIRD_PARTY_LIB_PATH}/gtest)

set(GTEST_LIBRARY ${THIRD_PARTY_LIB_PATH}/gtest/libgtest.a)
set(YAML_CPP_LIBRARY ${THIRD_PARTY_LIB_PATH}/yaml-cpp/libyaml-cpp.a)

set(OPENCV_LIBS ${OPENCV_LIBRARIES})
message("--------start to EnginCmake/JudgeEngin.cmake")
include(${CMAKE_SOURCE_DIR}/EnginCmake/JudgeEngin.cmake)

set(COMPUTE_LIBS   ${TENSORRT_LIBRARIES}  ${CUDA_LIBRARIES}
            dl     rt  )

message(STATUS "COMPUTE_LIBS : ${COMPUTE_LIBS}")

if(WITH_LICENSE)
    set(EXTRA_DEPLOY_TARGET_LIBRARY
            -pthread    gomp    z    m    licenser    cryptopp   )
else()
    set(EXTRA_DEPLOY_TARGET_LIBRARY
            -pthread    gomp    z    m    cryptopp   )
endif()
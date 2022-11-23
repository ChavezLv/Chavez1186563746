
link_directories(${CMAKE_CURRENT_SOURCE_DIR})


find_package(PkgConfig)

pkg_search_module(CUDA REQUIRED cuda)
include_directories(${CUDA_INCLUDE_DIRS})
link_directories(${CUDA_LIBRARY_DIRS})

pkg_search_module(TENSORRT REQUIRED tensorrt)
include_directories(${TENSORRT_INCLUDE_DIRS})
link_directories(${TENSORRT_LIBRARY_DIRS})

#[[
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../opencv4/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../opencv4/lib)
]]

file(GLOB ARCTERN_CUDA_SOURCE
        src/ImgPreProcess/cuda/*.cu
        src/ImgPreProcess/cuda/*.cuh
        src/ImgPreProcess/cuda/*.hpp)
message("${ARCTERN_CUDA_SOURCE}")
add_library(ArcternCuda STATIC ${ARCTERN_CUDA_SOURCE})
set(ArcternCudaLibs ArcternCuda)

file(GLOB ImgProcSource
        src/ImgPreProcess/*.cpp
        src/ImgPreProcess/*.h)

set(ImgProc ImgPreprocessor CACHE INTERNAL "images preprocess libs")

add_library(${ImgProc} STATIC ${ImgProcSource})

target_link_libraries(${ImgProc} ${ArcternCudaLibs}  libcudart.so libnppial.so libnppig.so libnppidei.so libnppicc.so libnppif.so libnppc.so  #[[libopencv_cudawarping.so]])

#[[
file(GLOB TestImgProcSource
        src/ImgPreProcess/UnitTest/TestImgPreprocessorOnNppEx.cpp
        src/common/trt/TrtAlgorithmUtil.cpp
        src/common/trt/TrtAlgorithmUtil.h
        UnitTest/main.cpp)
message(${TestImgProcSource})
add_executable(${ImgProc}_TEST ${TestImgProcSource})

target_link_libraries(${ImgProc}_TEST ${ImgProc} ${COMPUTE_LIBS} ${GTEST_LIBRARY} )]]

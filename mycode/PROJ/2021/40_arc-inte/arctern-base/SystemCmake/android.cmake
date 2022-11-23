
add_definitions(-mfpu=neon -mfloat-abi=softfp)
if (PLATFORM_RK_ARM64)
    add_definitions(-D__ARM_NEON__)
    add_definitions(-D__ARM_NEON)
endif ()

add_definitions(-fopenmp -ftree-vectorize -fPIE -fPIC -Wno-unused-function -frtti -fexceptions -std=c++11)

if (PLATFORM_RK)
    set(THIRD_PARTY_ROOT_PATH "${THIRD_PARTY_LIB_ROOT}/arm-linux-androideabi")
elseif (PLATFORM_RK_ARM64)
    set(THIRD_PARTY_ROOT_PATH "${THIRD_PARTY_LIB_ROOT}/arm-linux-androideabi-r18b-arm64")
endif ()

add_definitions(-DWITH_TVM)

if (PLATFORM_RK)
    set(THIRD_PARTY_ROOT_PATH "${THIRD_PARTY_LIB_ROOT}/arm-linux-androideabi")
elseif (PLATFORM_RK_ARM64)
    set(THIRD_PARTY_ROOT_PATH "${THIRD_PARTY_LIB_ROOT}/arm-linux-androideabi-r18b-arm64")
endif ()
include_directories("${THIRD_PARTY_ROOT_PATH}/include")
link_directories("${THIRD_PARTY_ROOT_PATH}/lib")
set(THIRD_PARTY_LIB_PATH ${THIRD_PARTY_ROOT_PATH}/lib)
set(THIRD_PARTY_INC_PATH ${THIRD_PARTY_ROOT_PATH}/include)
set(Z_LIB_LIBRARY_PATH ${THIRD_PARTY_LIB_PATH})

set(OPENBLAS_INCLUDE_DIR ${THIRD_PARTY_INC_PATH}/openblas)
include_directories(${OPENBLAS_INCLUDE_DIR})
link_directories(${THIRD_PARTY_LIB_PATH})

set(GTEST_LIBRARY libgtest.a)
set(YAML_CPP_LIBRARY libyaml-cpp.a)

set(OPENCV_LIBS
        opencv_video
        opencv_calib3d
        opencv_highgui
        opencv_flann
        opencv_imgcodecs
        opencv_imgproc
        opencv_core
        quirc libprotobuf tbb cpufeatures ittnotify libjpeg-turbo libwebp libpng libtiff libjasper IlmImf tegra_hal

        )

set(COMPUTE_LIBS -Wl,--whole-archive ${TVM_MODELS_DEFINED_SYMBOL} -Wl,-no-whole-archive tvm_runtime_pack dmlc GLES_mali)

if (WITH_LICENSE)
    set(LICENSER licenser)
else ()
endif ()

set(EXTRA_DEPLOY_TARGET_LIBRARY
        log
        -pthread
        z
        m
        ${GOMP_LIB}
        ${LICENSER}
        cryptopp
        )

include(${CMAKE_SOURCE_DIR}/EnginCmake/JudgeEngin.cmake)

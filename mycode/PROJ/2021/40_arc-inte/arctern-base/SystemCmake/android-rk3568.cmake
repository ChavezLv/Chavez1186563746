
add_definitions(-DBUILD_ARM_LINUX)
add_compile_options(-fvisibility=hidden)

set(PLATFORM_RV1109 ON)

set(THIRD_PARTY_LIB_ROOT_PATH "${THIRD_PARTY_LIB_ROOT}/aarch64-linux-android-rk3568/")
set(THIRD_PARTY_INC_PATH "${THIRD_PARTY_LIB_ROOT_PATH}/include")

message("---------------------------->${THIRD_PARTY_INC_PATH}")
set(THIRD_PARTY_LIB_PATH "${THIRD_PARTY_LIB_ROOT_PATH}/lib")

set(GTEST_LIBRARY ${THIRD_PARTY_LIB_PATH}/gtest/libgtest.a)
set(YAML_CPP_LIBRARY ${THIRD_PARTY_LIB_PATH}/yaml-cpp/libyaml-cpp.a)

##the first include directory
include_directories(
        ${CMAKE_BINARY_DIR}/include
        ${THIRD_PARTY_INC_PATH}
)

set(CRYPTOPP_LIBRARY_DIR ${THIRD_PARTY_LIB_PATH}/cryptopp)
link_directories(${CRYPTOPP_LIBRARY_DIR})

set(OPENCV_LIBRARY_DIR ${THIRD_PARTY_LIB_PATH}/opencv/cpu)
set(OPENCV_THIRD_PARTY_DIR ${THIRD_PARTY_LIB_PATH}/opencv/3rdparty)
link_directories(${OPENCV_LIBRARY_DIR})
link_directories(${OPENCV_THIRD_PARTY_DIR})


set(OPENSSL_LIBRARY_DIR ${THIRD_PARTY_LIB_PATH}/openssl)
link_directories(${OPENSSL_LIBRARY_DIR})

set(FLATBUFFERS_LIBRARY_DIR ${THIRD_PARTY_LIB_PATH}/flatbuffers)
link_directories(${FLATBUFFERS_LIBRARY_DIR})

set(RKNN_API_LIBRARY_DIR ${THIRD_PARTY_LIB_PATH}/rknn_api)
link_directories(${RKNN_API_LIBRARY_DIR})

set(OPENCV_LIBS
        opencv_shape
        opencv_photo
        opencv_video
        opencv_videoio
        opencv_videostab
        opencv_features2d
        opencv_calib3d
        opencv_superres
        opencv_objdetect
        opencv_stitching
        opencv_imgcodecs
        opencv_imgproc
        opencv_highgui
        opencv_ml
        opencv_dnn
        opencv_flann
        opencv_core
        libpng
        libjpeg-turbo
        libtiff
        libjasper
        libprotobuf
        libwebp
        IlmImf
        tegra_hal
	ittnotify
	cpufeatures
	log
	z
        )

set(OTHER_LIBS
        gomp
        dl
        rt
        pthread
        )

if (WITH_LICENSE)
    set(EXTRA_DEPLOY_TARGET_LIBRARY
            licenser
            cryptopp
            rknn_api
            )
else ()
    set(EXTRA_DEPLOY_TARGET_LIBRARY
            cryptopp
            rknn_api
            )
endif ()

include(${CMAKE_SOURCE_DIR}/EnginCmake/JudgeEngin.cmake)

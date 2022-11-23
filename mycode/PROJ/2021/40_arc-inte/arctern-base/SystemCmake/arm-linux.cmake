if (NOT AARCH64_HIMIX100 AND NOT AARCH64_HIMIX200)
    add_compile_options(-fvisibility=hidden) #disable this feature on hisi platform, it will cause yolov3 crash when sigmoid_neon
endif ()

add_definitions(-DBUILD_ARM_LINUX)
if (AARCH64_HIMIX100)
    add_definitions(-DBUILD_AARCH64_HIMIX100)
endif () # if(AARCH64_HIMIX100)
if (AARCH64_HIMIX200)
    add_definitions(-DBUILD_AARCH64_HIMIX200)
endif () # if(AARCH64_HIMIX200)

set(THIRD_PARTY_LIB_ROOT_PATH "${THIRD_PARTY_LIB_ROOT}/lib/arm")
if (${AARCH64_HIMIX200})
    set(THIRD_PARTY_LIB_PATH "${THIRD_PARTY_LIB_ROOT_PATH}/himix200")
elseif (${AARCH64_HIMIX100})
    set(THIRD_PARTY_LIB_PATH "${THIRD_PARTY_LIB_ROOT_PATH}/himix100")
else ()
    set(THIRD_PARTY_LIB_PATH "${THIRD_PARTY_LIB_ROOT_PATH}/armeabi-v7a")
endif ()

set(GTEST_LIBRARY ${THIRD_PARTY_LIB_PATH}/gtest/libgtest.a)
set(YAML_CPP_LIBRARY ${THIRD_PARTY_LIB_PATH}/yaml-cpp/libyaml-cpp.a)

set(THIRD_PARTY_INC_PATH "${THIRD_PARTY_LIB_ROOT}/include")
##the first include directory
include_directories(${CMAKE_BINARY_DIR}/include ${dmlc_INCLUDE_DIR}
        ${cryptopp_INCLUDE_DIR}
        ${THIRD_PARTY_INC_PATH}
        )


if (AARCH64_HIMIX100 OR AARCH64_HIMIX200)
    add_definitions(-DWITH_NNIE)
    if (WITH_YOLO_TINY)
        add_definitions(-DWITH_YOLO_TINY)
    endif () # if (WITH_YOLO_TINY)
endif () # if (AARCH64_HIMIX100 OR AARCH64_HIMIX200)

if (AARCH64_HIMIX100 OR AARCH64_HIMIX200)
    if (${AARCH64_HIMIX200_3519AV100}) # 3519av100
        set(HIMIX_LIB_PATH ${THIRD_PARTY_LIB_PATH}/nnie-3519av100)
    else () # 3559, 3516dv300
        set(HIMIX_LIB_PATH ${THIRD_PARTY_LIB_PATH}/nnie)
    endif ()

    set(Z_LIBRARY ${THIRD_PARTY_LIB_PATH}/opencv/3rdparty/libzlib.a)
    set(IF_ALGO_LIBRARY ${THIRD_PARTY_LIB_PATH}/if_algo/lib_algo.a)
    set(VOICEENGINE_LINRARY ${HIMIX_LIB_PATH}/libVoiceEngine.a)
    set(DNVQE_LINRARY ${HIMIX_LIB_PATH}/libdnvqe.a)
    set(UPVQE_LINRARY ${HIMIX_LIB_PATH}/libupvqe.a)
    set(AACDEC_LINRARY ${HIMIX_LIB_PATH}/libaacdec.a)
    set(AACENC_LINRARY ${HIMIX_LIB_PATH}/libaacenc.a)
    set(MPI_LINRARY ${HIMIX_LIB_PATH}/libmpi.a)
    set(MPI_PHOTO_LINRARY ${HIMIX_LIB_PATH}/libmpi_photo.a)
    set(NNIE_LIBRARY ${HIMIX_LIB_PATH}/libnnie.a)
    set(SECUREC_LIBRARY ${HIMIX_LIB_PATH}/libsecurec.a)
    if (AARCH64_HIMIX100)
        set(DSP_LIBRARY ${HIMIX_LIB_PATH}/libdsp.a)
        set(IVE_LIBRARY ${HIMIX_LIB_PATH}/libive.a)
        set(OPENBLAS_LIBRARY ${THIRD_PARTY_LIB_PATH}/openblas/libopenblas.a)
    else ()
        set(OPENBLAS_LIBRARY ${THIRD_PARTY_LIB_PATH}/openblas/libopenblas.a)
        set(GOMP_LIBS ${THIRD_PARTY_LIB_PATH}/gomp/libgomp.a
                ${THIRD_PARTY_LIB_PATH}/gomp/libomp.a)
        set(IVE_LIBRARY ${HIMIX_LIB_PATH}/libive.a)
    endif ()
endif ()

set(CRYPTOPP_LIBRARY_DIR ${THIRD_PARTY_LIB_PATH}/cryptopp)
set(OPENCV_THIRD_PARTY_DIR ${THIRD_PARTY_LIB_PATH}/opencv/3rdparty)
set(OPENCV_LIBRARY_DIR ${THIRD_PARTY_LIB_PATH}/opencv/cpu)
set(OPENBLAS_INCLUDE_DIR ${THIRD_PARTY_INC_PATH}/openblas)
set(OPENBLAS_LIBRARY_DIR ${THIRD_PARTY_LIB_PATH}/openblas)
include_directories(${OPENBLAS_INCLUDE_DIR})
set(FLATBUFFERS_LIBRARY_DIR ${THIRD_PARTY_LIB_PATH}/flatbuffers)
set(PROTOBUF_LIBRARY_DIR "${THIRD_PARTY_LIB_PATH}/protobuf")

link_directories(${CRYPTOPP_LIBRARY_DIR})
link_directories(${OPENCV_LIBRARY_DIR})
link_directories(${OPENCV_THIRD_PARTY_DIR})
link_directories(${DMLC_LIB_DIRS})
link_directories(${OPENSSL_LIBRARY_DIR})
link_directories(${FLATBUFFERS_LIBRARY_DIR})
link_directories(${PROTOBUF_LIBRARY_DIR})
link_directories(${GRPC_LIBRARY_DIR})
link_directories(${Z_LIB_LIBRARY_PATH})


if (AARCH64_HIMIX200)
    set(other_libs
            dl
            rt
            ${OPENBLAS_LIBRARY}
            ${GOMP_LIBS}
            pthread)
else () # 100 and other arm devices
    set(other_libs
            gomp
            dl
            rt
            ${OPENBLAS_LIBRARY}
            pthread)
endif ()

if (AARCH64_HIMIX100 OR AARCH64_HIMIX200)
    set(hisi_libs
            ${IF_ALGO_LIBRARY}
            ${MPI_LINRARY}
            ${NNIE_LIBRARY}
            ${IVE_LIBRARY}
            ${SECUREC_LIBRARY}
            ${DNVQE_LINRARY}
            ${UPVQE_LINRARY}
            ${VOICEENGINE_LINRARY}
            ${DSP_LIBRARY})
endif ()


set(OPENCV_LIBS
        opencv_video
        opencv_calib3d
        opencv_highgui
        opencv_flann
        opencv_imgcodecs
        opencv_imgproc
        libwebp
        libpng
        libjpeg
        libtiff
        libjasper
        IlmImf
        opencv_core
        ${Z_LIBRARY}
        )

if (WITH_LICENSE)
    set(EXTRA_DEPLOY_TARGET_LIBRARY
            licenser
            cryptopp
            ${hisi_libs}
            ${other_libs}
            )
else ()
    set(EXTRA_DEPLOY_TARGET_LIBRARY
            cryptopp
            ${hisi_libs}
            ${other_libs})
endif ()

include(${CMAKE_SOURCE_DIR}/EnginCmake/JudgeEngin.cmake)
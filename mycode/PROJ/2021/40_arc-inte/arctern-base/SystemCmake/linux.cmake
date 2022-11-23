set(THIRD_PARTY_INC_PATH "${THIRD_PARTY_LIB_ROOT}/include")

set(THIRD_PARTY_LIB_ROOT_PATH "${THIRD_PARTY_LIB_ROOT}/lib/linux")
set(THIRD_PARTY_LIB_PATH "${THIRD_PARTY_LIB_ROOT_PATH}/gcc")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -msse4.1")
add_definitions(-fvisibility=hidden)

#-------------------- set third libs  --------------------
link_directories(${THIRD_PARTY_LIB_PATH})
link_directories(${THIRD_PARTY_LIB_PATH}/opencv/3rdparty)
link_directories(${THIRD_PARTY_LIB_PATH}/opencv/cpu)
link_directories(${THIRD_PARTY_LIB_PATH}/openblas)
link_directories(${THIRD_PARTY_LIB_PATH}/yaml-cpp)
link_directories(${THIRD_PARTY_LIB_PATH}/cryptopp)
link_directories(${THIRD_PARTY_LIB_PATH}/gtest)



set(GTEST_LIBRARY ${THIRD_PARTY_LIB_PATH}/gtest/libgtest.a)
set(YAML_CPP_LIBRARY ${THIRD_PARTY_LIB_PATH}/yaml-cpp/libyaml-cpp.a)

set(OPENCV_LIBS
     opencv_shape opencv_stitching opencv_objdetect opencv_superres
     opencv_videostab  opencv_calib3d opencv_features2d opencv_highgui 
     opencv_videoio opencv_imgcodecs opencv_video opencv_photo 
     opencv_ml opencv_imgproc opencv_flann opencv_core
     libwebp   libpng   libjpeg   libtiff    libjasper  IlmImf  )

message("--------start to JudgeEngin.cmake")
include(${CMAKE_SOURCE_DIR}/EnginCmake/JudgeEngin.cmake)

add_definitions(-DBUILD_LINUX)
set(PLATFORM_LINUX ON)

if (MXNET_ALLOWED)
    link_directories(${THIRD_PARTY_LIB_PATH}/mxnet/gcc4.8/mxnet_mkldnn/lib)
    set(OPENBLAS_LIB  ${THIRD_PARTY_LIB_PATH}/mxnet/gcc4.8/openblas/lib/libopenblas.a)
    include_directories(${THIRD_PARTY_LIB_PATH}/mxnet/gcc4.8/openblas/include)
    set(COMPUTE_LIBS   ${MXNET_LIBRARY}     ${OPENBLAS_LIB} ${MKLDNN_LIB}
            ${DMLC_LIB}      dl     rt  )
endif ()

if (TVM_ALLOWED)
    message("0000000000000000000000000000000${TVM_MODELS_DEFINED_SYMBOL}")
    set(COMPUTE_LIBS  -Wl,--whole-archive ${TVM_MODELS_DEFINED_SYMBOL} -Wl,-no-whole-archive tvm_runtime_pack  dl rt pthread )
endif ()

if(MNN_ALLOWED)
    set(COMPUTE_LIBS MNN  dl rt pthread )
endif()

if(WITH_LICENSE)
    set(EXTRA_DEPLOY_TARGET_LIBRARY
    -pthread    gomp    z    m    licenser    cryptopp   )
else()
    set(EXTRA_DEPLOY_TARGET_LIBRARY
            -pthread    gomp    z    m    cryptopp   )
endif()

if (${USE_OPENBLAS})
  add_definitions(-DUSE_OPENBLAS)
endif()

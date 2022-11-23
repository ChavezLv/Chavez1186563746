set(THIRD_PARTY_LIB_PATH "${THIRD_PARTY_LIB_ROOT}/lib/arm/taishan_atlas300")
add_definitions(-fvisibility=hidden -g )
add_definitions(-DENABLE_DVPP_INTERFACE)
message(STATUS "build with Atlas ACL support ${THIRD_PARTY_LIB_ROOT_PATH}")
# Set include directory and library directory
set(ACL_INC_DIR $ENV{ASCEND_HOME}/$ENV{ASCEND_VERSION}/$ENV{ARCH_PATTERN}/acllib/include)
set(ACL_LIB_DIR $ENV{ASCEND_HOME}/$ENV{ASCEND_VERSION}/$ENV{ARCH_PATTERN}/acllib/lib64/stub)
message("++++++++++++++++ACL_INC_DIR = ${ACL_INC_DIR}++++++++++++++++")
message("++++++++++++++++ACL_LIB_DIR = ${ACL_LIB_DIR}++++++++++++++++")

#message($ENV{LD_LIBRARY_PATH})

include_directories(${ACL_INC_DIR})
#link_directories($ENV{ASCEND_HOME}/$ENV{ASCEND_VERSION}/$ENV{ARCH_PATTERN}/acllib/lib64)
include_directories(${THIRD_PARTY_LIB_ROOT}/include/taishan_atlas300)
# Add definitions ENABLE_DVPP_INTERFACE to use dvpp api
link_directories(${ACL_LIB_DIR})

#-------------------- set third libs  --------------------
link_directories(${THIRD_PARTY_LIB_PATH})
link_directories(${THIRD_PARTY_LIB_PATH}/opencv/3rdparty)
link_directories(${THIRD_PARTY_LIB_PATH}/opencv/cpu)
link_directories(${THIRD_PARTY_LIB_PATH}/flatbuffers)
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
        libpng   libjpeg   libtiff    libjasper  IlmImf zlib )

include(${CMAKE_SOURCE_DIR}/EnginCmake/JudgeEngin.cmake)
add_definitions(-DBUILD_LINUX)
set(PLATFORM_LINUX ON)
set(COMPUTE_LIBS        ${OPENBLAS_LIB} ascendcl acl_dvpp #acl_cblas acl_retr runtime
          dl     rt  )

set(EXTRA_DEPLOY_TARGET_LIBRARY 
    -pthread    gomp      m     cryptopp )


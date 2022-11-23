message("========This is in localbuild.cmake")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-fvisibility=hidden  -fPIC)

if(NOT ARCTERN1_ROOT_PATH)
    set(ARCTERN1_ROOT_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
endif()

include(${ARCTERN1_ROOT_PATH}/cmake/colors.cmake)

#-------------------- some compiler options --------------------
set(DEPLOY_TARGET linux CACHE STRING "Only enable this in production environment")
set_property(CACHE DEPLOY_TARGET PROPERTY STRINGS osx linux arm android win32 win64)

if(${DEPLOY_TARGET} STREQUAL "win32" OR ${DEPLOY_TARGET} STREQUAL "win64")
    add_definitions(-DBUILD_WIN32)
elseif(${DEPLOY_TARGET} STREQUAL "osx")
    add_definitions(-DBUILD_OSX)
elseif(${DEPLOY_TARGET} STREQUAL "linux")
    add_definitions(-DBUILD_LINUX)
    set(PLATFORM_LINUX ON)
elseif(${DEPLOY_TARGET} STREQUAL "arm")
    add_definitions(-DBUILD_ARM_LINUX)
elseif(${DEPLOY_TARGET} STREQUAL "android")
    add_definitions(-DBUILD_ANDROID)
else()
    message(FATAL "Not supported deploy target: ${DEPLOY_TARGET}")
endif()
#-------------------- some compiler options --------------------

#-------------------- Third party libraries' path --------------------
set(THIRD_PARTY_LIB_ROOT ${ARCTERN1_ROOT_PATH}/../arctern-libs)
if(NOT EXISTS ${THIRD_PARTY_LIB_ROOT})
    message(FATAL_ERROR "${BoldRed} cannot find ${THIRD_PARTY_LIB_ROOT} !!!!!!${ColourReset}")
endif()

include(${ARCTERN1_ROOT_PATH}/SystemCmake/${DEPLOY_TARGET}.cmake)
include_directories(${THIRD_PARTY_INC_PATH})

message("========THIRD_PARTY_LIB_PATH: ${THIRD_PARTY_LIB_PATH}")
message("========THIRD_PARTY_INC_PATH: ${THIRD_PARTY_INC_PATH}")
#-------------------- Third party libraries' path --------------------

#--------------------  WITH_LICENSE  --------------------
if(WITH_LICENSE)
  add_definitions(-DWITH_LICENSE)
endif() # if WITH_LICENSE
set(LICENSER_DIR ${ARCTERN1_ROOT_PATH}/3rdparty/licenser)
include_directories(${LICENSER_DIR}/include)
add_subdirectory(${LICENSER_DIR} LICENSER_DIR.out)
link_directories(${LICENSER_DIR})
#--------------------  WITH_LICENSE  --------------------

file(GLOB_RECURSE LOCALBUILD_SOURCES
     ${ARCTERN1_ROOT_PATH}/src/Base/*.cpp
     ${ARCTERN1_ROOT_PATH}/src/log/*.cpp
     ${ARCTERN1_ROOT_PATH}/src/Engin/*.cpp
     ${ARCTERN1_ROOT_PATH}/src/common/*.cpp
     ${ARCTERN1_ROOT_PATH}/src/Processors/*.cpp)

 include_directories(${ARCTERN1_ROOT_PATH})
 include_directories(${ARCTERN1_ROOT_PATH}/src)

if(MXNET_ALLOWED)
    add_definitions(-DMXNET_ALLOWED)
endif()

if(TENSORFLOW_ALLOWED)
    add_definitions(-DTENSORFLOW_ALLOWED)
endif()

if(RKNN_ALLOWED)
    add_definitions(-DRKNN_ALLOWED)
endif()

message("-----------------------local cmake -------------------")
set(LOCAL_OPENCV_LIBS ${OPENCV_LIBS} )
set(LOCAL_COMPUTE_LIBS ${COMPUTE_LIBS})
set(LOCAL_EXTRA_DEPLOY_TARGET_LIBRARY ${EXTRA_DEPLOY_TARGET_LIBRARY})

set(CMAKE_INSTALL_PREFIX ./install)

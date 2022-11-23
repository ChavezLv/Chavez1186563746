# Install script for directory: /workspace/arctern-base

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/workspace/arctern-base/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "/workspace/arctern-base/src/version.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libarctern-base.so.0.1.1" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libarctern-base.so.0.1.1")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libarctern-base.so.0.1.1"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/workspace/arctern-base/build-trt/libarctern-base.so.0.1.1")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libarctern-base.so.0.1.1" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libarctern-base.so.0.1.1")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libarctern-base.so.0.1.1"
         OLD_RPATH "/usr/local/cuda/lib64:/usr/local/TensorRT-7.2.1.6/lib:/usr/local/lib:/usr/local/share/OpenCV/3rdparty/lib:/workspace/arctern-base/../arctern-libs/lib/linux/gcc:/workspace/arctern-base/../arctern-libs/lib/linux/gcc/opencv/3rdparty:/workspace/arctern-base/../arctern-libs/lib/linux/gcc/opencv/cpu:/workspace/arctern-base/../arctern-libs/lib/linux/gcc/openblas:/workspace/arctern-base/../arctern-libs/lib/linux/gcc/yaml-cpp:/workspace/arctern-base/../arctern-libs/lib/linux/gcc/cryptopp:/workspace/arctern-base/../arctern-libs/lib/linux/gcc/gtest:/workspace/arctern-base:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libarctern-base.so.0.1.1")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libarctern-base.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libarctern-base.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libarctern-base.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/workspace/arctern-base/build-trt/libarctern-base.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libarctern-base.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libarctern-base.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libarctern-base.so"
         OLD_RPATH "/usr/local/cuda/lib64:/usr/local/TensorRT-7.2.1.6/lib:/usr/local/lib:/usr/local/share/OpenCV/3rdparty/lib:/workspace/arctern-base/../arctern-libs/lib/linux/gcc:/workspace/arctern-base/../arctern-libs/lib/linux/gcc/opencv/3rdparty:/workspace/arctern-base/../arctern-libs/lib/linux/gcc/opencv/cpu:/workspace/arctern-base/../arctern-libs/lib/linux/gcc/openblas:/workspace/arctern-base/../arctern-libs/lib/linux/gcc/yaml-cpp:/workspace/arctern-base/../arctern-libs/lib/linux/gcc/cryptopp:/workspace/arctern-base/../arctern-libs/lib/linux/gcc/gtest:/workspace/arctern-base:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libarctern-base.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/include" TYPE FILE FILES
    "/workspace/arctern-base/include/arctern.h"
    "/workspace/arctern-base/include/result.h"
    "/workspace/arctern-base/include/runparameter.h"
    "/workspace/arctern-base/include/initparameter.h"
    "/workspace/arctern-base/include/algorithminterface.h"
    "/workspace/arctern-base/include/atlasInit.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/workspace/arctern-base/build-trt/src/Engin/cmake_install.cmake")
  include("/workspace/arctern-base/build-trt/src/Algorithms/memoryDB/cmake_install.cmake")
  include("/workspace/arctern-base/build-trt/src/Algorithms/licenseLandmark/cmake_install.cmake")
  include("/workspace/arctern-base/build-trt/src/Algorithms/cargoIllegalDetect/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/workspace/arctern-base/build-trt/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")

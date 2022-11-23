# Install script for directory: /workspace/arctern-base/src/Algorithms/licenseLandmark

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
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
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Algorithms/licenseLandmark/licenseLandmark_UnitTest_D" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Algorithms/licenseLandmark/licenseLandmark_UnitTest_D")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Algorithms/licenseLandmark/licenseLandmark_UnitTest_D"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/Algorithms/licenseLandmark" TYPE EXECUTABLE FILES "/workspace/arctern-base/build-trt/src/Algorithms/licenseLandmark/licenseLandmark_UnitTest_D")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Algorithms/licenseLandmark/licenseLandmark_UnitTest_D" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Algorithms/licenseLandmark/licenseLandmark_UnitTest_D")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Algorithms/licenseLandmark/licenseLandmark_UnitTest_D"
         OLD_RPATH "/usr/local/cuda/lib64:/usr/local/TensorRT-7.2.1.6/lib:/usr/local/lib:/usr/local/share/OpenCV/3rdparty/lib:/workspace/arctern-base/../arctern-libs/lib/linux/gcc:/workspace/arctern-base/../arctern-libs/lib/linux/gcc/opencv/3rdparty:/workspace/arctern-base/../arctern-libs/lib/linux/gcc/opencv/cpu:/workspace/arctern-base/../arctern-libs/lib/linux/gcc/openblas:/workspace/arctern-base/../arctern-libs/lib/linux/gcc/yaml-cpp:/workspace/arctern-base/../arctern-libs/lib/linux/gcc/cryptopp:/workspace/arctern-base/../arctern-libs/lib/linux/gcc/gtest:/workspace/arctern-base:/workspace/arctern-base/build-trt:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Algorithms/licenseLandmark/licenseLandmark_UnitTest_D")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Algorithms/licenseLandmark/licenseLandmark_Example_D" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Algorithms/licenseLandmark/licenseLandmark_Example_D")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Algorithms/licenseLandmark/licenseLandmark_Example_D"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/Algorithms/licenseLandmark" TYPE EXECUTABLE FILES "/workspace/arctern-base/build-trt/src/Algorithms/licenseLandmark/licenseLandmark_Example_D")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Algorithms/licenseLandmark/licenseLandmark_Example_D" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Algorithms/licenseLandmark/licenseLandmark_Example_D")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Algorithms/licenseLandmark/licenseLandmark_Example_D"
         OLD_RPATH "/usr/local/cuda/lib64:/usr/local/TensorRT-7.2.1.6/lib:/usr/local/lib:/usr/local/share/OpenCV/3rdparty/lib:/workspace/arctern-base/../arctern-libs/lib/linux/gcc:/workspace/arctern-base/../arctern-libs/lib/linux/gcc/opencv/3rdparty:/workspace/arctern-base/../arctern-libs/lib/linux/gcc/opencv/cpu:/workspace/arctern-base/../arctern-libs/lib/linux/gcc/openblas:/workspace/arctern-base/../arctern-libs/lib/linux/gcc/yaml-cpp:/workspace/arctern-base/../arctern-libs/lib/linux/gcc/cryptopp:/workspace/arctern-base/../arctern-libs/lib/linux/gcc/gtest:/workspace/arctern-base:/workspace/arctern-base/build-trt:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/Algorithms/licenseLandmark/licenseLandmark_Example_D")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "/workspace/arctern-base/src/Algorithms/licenseLandmark/licenseLandmark.h")
endif()


# Install script for directory: /workspace/arctern-base/src/Engin/src/trt/core/plugin

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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/workspace/arctern-base/build-trt/src/Engin/src/trt/core/plugin/common/cmake_install.cmake")
  include("/workspace/arctern-base/build-trt/src/Engin/src/trt/core/plugin/BilinearSampler/cmake_install.cmake")
  include("/workspace/arctern-base/build-trt/src/Engin/src/trt/core/plugin/Embedding/cmake_install.cmake")
  include("/workspace/arctern-base/build-trt/src/Engin/src/trt/core/plugin/GridGenerator/cmake_install.cmake")
  include("/workspace/arctern-base/build-trt/src/Engin/src/trt/core/plugin/L2Normalization/cmake_install.cmake")
  include("/workspace/arctern-base/build-trt/src/Engin/src/trt/core/plugin/LastDimTopK/cmake_install.cmake")
  include("/workspace/arctern-base/build-trt/src/Engin/src/trt/core/plugin/Pad/cmake_install.cmake")
  include("/workspace/arctern-base/build-trt/src/Engin/src/trt/core/plugin/PReLU/cmake_install.cmake")
  include("/workspace/arctern-base/build-trt/src/Engin/src/trt/core/plugin/SpatialTransformer/cmake_install.cmake")
  include("/workspace/arctern-base/build-trt/src/Engin/src/trt/core/plugin/UpSampling/cmake_install.cmake")
  include("/workspace/arctern-base/build-trt/src/Engin/src/trt/core/plugin/Equal/cmake_install.cmake")
  include("/workspace/arctern-base/build-trt/src/Engin/src/trt/core/plugin/ModSaclar/cmake_install.cmake")
  include("/workspace/arctern-base/build-trt/src/Engin/src/trt/core/plugin/Repeat/cmake_install.cmake")
  include("/workspace/arctern-base/build-trt/src/Engin/src/trt/core/plugin/Erf/cmake_install.cmake")

endif()


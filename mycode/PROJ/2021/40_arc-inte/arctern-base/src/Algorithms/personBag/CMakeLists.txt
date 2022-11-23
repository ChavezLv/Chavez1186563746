cmake_minimum_required(VERSION 3.1)

project(personBag)

set(ARCTERN_personBag_VERSION_MAJOR    1)
set(ARCTERN_personBag_VERSION_MINOR    0)
set(ARCTERN_personBag_VERSION_REVISION 0)

if(MXNET_ALLOWED)
include(${SOLUTION_SOURCE_DIR}/CmakeCommon/algorithmCommon.cmake)
else(TRT_ALLOWED)
include(${SOLUTION_SOURCE_DIR}/CmakeCommon/trtAlgorithmCommon.cmake)
endif(MXNET_ALLOWED)

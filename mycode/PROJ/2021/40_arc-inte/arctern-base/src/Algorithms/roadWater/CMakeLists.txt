cmake_minimum_required(VERSION 3.1)

project(roadWater)

set(ARCTERN_roadWater_VERSION_MAJOR    1)
set(ARCTERN_roadWater_VERSION_MINOR    0)
set(ARCTERN_roadWater_VERSION_REVISION 0)

if(TRT_ALLOWED)
    include(${SOLUTION_SOURCE_DIR}/CmakeCommon/trtAlgorithmCommon.cmake)
endif(TRT_ALLOWED)

if(ATLAS_ALLOWED)
    include(${SOLUTION_SOURCE_DIR}/CmakeCommon/atlasAlgorithmCommon.cmake)
endif(ATLAS_ALLOWED)

if(MXNET_ALLOWED)
    include(${SOLUTION_SOURCE_DIR}/CmakeCommon/algorithmCommon.cmake)
endif(MXNET_ALLOWED)


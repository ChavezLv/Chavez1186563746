cmake_minimum_required(VERSION 3.1)

project(faceImageColor)

set(ARCTERN_faceImageColor_VERSION_MAJOR    1)
set(ARCTERN_faceImageColor_VERSION_MINOR    0)
set(ARCTERN_faceImageColor_VERSION_REVISION 0)

if(TRT_ALLOWED)
    include(${SOLUTION_SOURCE_DIR}/CmakeCommon/trtAlgorithmCommon.cmake)
else(TRT_ALLOWED)
    include(${SOLUTION_SOURCE_DIR}/CmakeCommon/algorithmCommon.cmake)
endif(TRT_ALLOWED)
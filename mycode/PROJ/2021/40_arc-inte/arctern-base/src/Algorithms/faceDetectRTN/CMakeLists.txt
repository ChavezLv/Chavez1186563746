cmake_minimum_required(VERSION 3.1)

project(faceDetectRTN)
message("------------Here Is faceDetectRTN Project")
set(ARCTERN_faceDetectRTN_VERSION_MAJOR    1)
set(ARCTERN_faceDetectRTN_VERSION_MINOR    0)
set(ARCTERN_faceDetectRTN_VERSION_REVISION 0)
#install(LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/Docs/TestAlgorithm02.txt DESTINATION libs/)

if(TRT_ALLOWED)
    include(${SOLUTION_SOURCE_DIR}/CmakeCommon/trtAlgorithmCommon.cmake)
else(TRT_ALLOWED)
    include(${SOLUTION_SOURCE_DIR}/CmakeCommon/algorithmCommon.cmake)
endif(TRT_ALLOWED)

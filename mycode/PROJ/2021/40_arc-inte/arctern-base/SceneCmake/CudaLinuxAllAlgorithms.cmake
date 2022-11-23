link_directories(src/CudaAlgorithms/TestAlgorithm)
link_directories(src/CudaAlgorithms/TestAlgorithm02)
add_definitions(-DCUDA_INTERFACE)
add_definitions(-DUSE_MXNET_CUDA)
find_package(CUDA REQUIRED)
if(${CUDA_FOUND})
    include_directories(${CUDA_INCLUDE_DIRS})
    link_directories(${CUDA_LIBS})

    message(STATUS Scuda includes:${CUDA_INCLUDE_DIRS})
    message(STATUS cuda libs:${CUDA_LIBS})
else(${CUDA_FOUND})
    message(STATUS "CUDA cannot find.")
endif(${CUDA_FOUND})



#################algorithms##################
if (MXNET_ALLOWED)
    message(STATUS "--------goto faceDetectTiny")
    add_subdirectory(src/CudaLinuxAlgorithms/faceDetectTiny)

    message(STATUS "--------goto faceQuality")
    add_subdirectory(src/CudaLinuxAlgorithms/faceQuality)

    message(STATUS "--------goto faceMask")
    add_subdirectory(src/CudaLinuxAlgorithms/faceMask)

    message(STATUS "--------goto facePose")
    add_subdirectory(src/CudaLinuxAlgorithms/facePose)

    message(STATUS "--------goto facialLandmark")
    add_subdirectory(src/CudaLinuxAlgorithms/facialLandmark)

    message(STATUS "--------goto faceBasicMultiAttr")
    add_subdirectory(src/CudaLinuxAlgorithms/faceBasicMultiAttr)

    message(STATUS "--------goto faceGluon")
    add_subdirectory(src/CudaLinuxAlgorithms/faceGluon)

    message(STATUS "--------goto TestSequenceMask")
    add_subdirectory(src/CudaLinuxAlgorithms/TestSequenceMask)
endif ()


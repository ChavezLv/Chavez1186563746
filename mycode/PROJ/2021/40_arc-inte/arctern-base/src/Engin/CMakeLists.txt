cmake_minimum_required(VERSION 3.1)

project(ENGIN)
message("--------Here Is ENGINE Project")
set(ENGIN_VERSION_MAJOR    1)
set(ENGIN_VERSION_MINOR    0)
set(ENGIN_VERSION_REVISION 0)



if(MXNET_ALLOWED)
    message("=======use mxnet  allowed===========")
    add_definitions(-DMXNET_ALLOWED)
    file(GLOB LOCAL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mxnet/*.cpp
    )

endif()


if(HISI_ALLOWED)
    message("=======use NNIE  allowed===========")
    add_definitions(-DHISI_ALLOWED)
    file(GLOB LOCAL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hisi/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hisi/nnie/*.cpp
        )

endif()

if(RKNN_ALLOWED)
    message("=======use RKNN engin===========")
    add_definitions(-DRKNN_ALLOWED)
    file(GLOB LOCAL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/rknn/*.cpp
    )
endif()

if(RKNN2_ALLOWED)
    message("=======use RKNN engin===========")
    add_definitions(-DRKNN2_ALLOWED)
    file(GLOB LOCAL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/rknn2/*.cpp
    )
endif()

if(TVM_ALLOWED)
    message("=======use tvm  allowed===========")
    add_definitions(-DTVM_ALLOWED)
    file(GLOB LOCAL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/tvm/*.cpp
            )
endif()

if(MNN_ALLOWED)
    message("=======use mnn  allowed===========")
    add_definitions(-DMNN_ALLOWED)
    file(GLOB LOCAL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/mnn/*.cpp
            )
endif()

if(TRT_ALLOWED)
    message("=======use trt  allowed===========")
    add_definitions(-DTRT_ALLOWED)
    add_subdirectory(src/trt)
    file(GLOB LOCAL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
            )
    set(LOCAL_SOURCES ${LOCAL_SOURCES} ${TRT_CORES} )

endif()
if(ATLAS_ALLOWED)
    message("=======use atlas  allowed===========")

    file(GLOB LOCAL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/atlas/*.cpp 
                            ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/../common/ReadWriteLock.cpp)

    if(NOT PLAIN_MODEL)
        set(LOCAL_SOURCES ${LOCAL_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/../common/aes_cryptor.cpp)
    endif()
    
    
endif()

message("--------add library ${PROJECT_NAME}")
add_library(${PROJECT_NAME} STATIC  ${LOCAL_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
set_target_properties(${PROJECT_NAME} PROPERTIES CUDA_RESOLVE_DEVICE_SYMBOLS ON)
target_link_libraries(${PROJECT_NAME}
    ${LOCAL_OPENCV_LIBS}
    ${EXTRA_DEPLOY_TARGET_LIBRARY}
    ${LOCAL_COMPUTE_LIBS})

install(TARGETS ${PROJECT_NAME}
LIBRARY DESTINATION lib
RUNTIME DESTINATION bin
ARCHIVE DESTINATION libstatic)


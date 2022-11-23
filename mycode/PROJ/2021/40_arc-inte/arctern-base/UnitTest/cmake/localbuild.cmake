
if (${CMAKE_BUILD_TYPE} MATCHES "" OR ${CMAKE_BUILD_TYPE} MATCHES "Release")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    set(TARGET_NAME ${PROJECT_NAME})
elseif (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -ggdb -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb -O0")
    set(TARGET_NAME ${PROJECT_NAME}_Debug)
else ()
    message(FATAL_ERROR "CMAKE_BUILD_TYPE must be Release or Debug")
endif ()

if (RKNN_ALLOWED)
    set(LOCALBUILD_LIBS
        ${OPENCV_LIBS}
        dmlc openblas pthread rt dl z
        )
else ()
    set(LOCALBUILD_LIBS
        "-Wl,-Bsymbolic -Wl,--whole-archive  -lmxnet  -Wl,--no-whole-archive"
        mxnet
        ${OPENCV_LIBS}
        dmlc openblas pthread rt dl z
        )
endif ()

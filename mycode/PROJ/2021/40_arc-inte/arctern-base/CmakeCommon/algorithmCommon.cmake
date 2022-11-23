
#-------------------- add source and link libraries --------------------

file(GLOB LOCAL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp)

if(HISI_ALLOWED)
    file(GLOB LOCAL_HISI_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/hisi/*.cpp)
    list(APPEND LOCAL_SOURCES ${LOCAL_HISI_SOURCES})
endif()

if(RKNN_ALLOWED)
    file(GLOB LOCAL_RKNN_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/rknn/*.cpp)
    list(APPEND LOCAL_SOURCES ${LOCAL_RKNN_SOURCES})
endif()

if(RKNN2_ALLOWED)
    file(GLOB LOCAL_RKNN2_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/rknn2/*.cpp)
    list(APPEND LOCAL_SOURCES ${LOCAL_RKNN2_SOURCES})
endif()

if(TVM_ALLOWED)
    file(GLOB LOCAL_TVM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tvm/*.cpp)
    list(APPEND LOCAL_SOURCES ${LOCAL_TVM_SOURCES})
endif()

if(MNN_ALLOWED)
    file(GLOB LOCAL_MNN_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/mnn/*.cpp)
    list(APPEND LOCAL_SOURCES ${LOCAL_MNN_SOURCES})
endif()

if(TRT_ALLOWED)
    message( STATUS "TRT_ALLOWED")
    file(GLOB LOCAL_TRT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/trt/*.cpp)
    list(APPEND LOCAL_SOURCES ${LOCAL_TRT_SOURCES})
endif(TRT_ALLOWED)

list(APPEND ALGSOURCES ${LOCAL_SOURCES})
set(ALGSOURCES ${ALGSOURCES} PARENT_SCOPE)
add_definitions(-fvisibility=hidden -fPIC)

#--------- add source and link libraries --------------------
file(GLOB TEST_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../UnitTest/UnitTestBase/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../UnitTest/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../UnitTest/Algorithms/${PROJECT_NAME}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/common/autoTime.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/common/encryx.cpp
        )

file(GLOB EXAMPLE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../Example/Algorithms/${PROJECT_NAME}/*.cpp)


message("----------------ADD ${PROJECT_NAME} ${PROJECT_NAME}_UnitTest ${PROJECT_NAME}_Example")
add_library(${PROJECT_NAME} STATIC ${LOCAL_SOURCES} ${LOCALBUILD_SOURCES})

message(${LOCAL_SOURCES})


add_executable(${PROJECT_NAME}_UnitTest_D ${TEST_SOURCES})
add_executable(${PROJECT_NAME}_Example_D ${EXAMPLE_SOURCES})

target_link_libraries(${PROJECT_NAME}
        ${TARGET_STATIC_NAME}   ENGIN     ${LOCAL_OPENCV_LIBS}
        ${LOCAL_COMPUTE_LIBS}        ${LOCAL_EXTRA_DEPLOY_TARGET_LIBRARY} )


target_link_libraries(${PROJECT_NAME}_UnitTest_D
        ${TARGET_NAME}      ${OPENCV_LIBS}
        ${EXTRA_DEPLOY_TARGET_LIBRARY}      ${GTEST_LIBRARY}  ${YAML_CPP_LIBRARY} )
target_link_libraries(${PROJECT_NAME}_Example_D
        ${TARGET_NAME}       ${OPENCV_LIBS}
        ${EXTRA_DEPLOY_TARGET_LIBRARY} )

#-------------------- add source and link libraries --------------------
INSTALL(TARGETS ${PROJECT_NAME}_UnitTest_D ${PROJECT_NAME}_Example_D
    RUNTIME DESTINATION bin/Algorithms/${PROJECT_NAME}
    LIBRARY DESTINATION lib)

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.h DESTINATION include)

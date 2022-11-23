#-------------------- add source and link libraries --------------------

message("===============start to build ${PROJECT_NAME}==================")
file(GLOB LOCAL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB LOCAL_MNN_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/mnn/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp
        )
list(APPEND LOCAL_SOURCES ${LOCAL_MNN_SOURCES})
list(APPEND ALGSOURCES ${LOCAL_SOURCES})

#message("${ALGSOURCES}")
set(ALGSOURCES ${ALGSOURCES} PARENT_SCOPE)

file(GLOB TEST_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../UnitTest/UnitTestBase/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../UnitTest/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../UnitTest/Algorithms/${PROJECT_NAME}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../UnitTest/Algorithms/${PROJECT_NAME}/groundtruth.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/common/autoTime.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/common/encryx.cpp)
message("mnn： ${TEST_SOURCES}")

file(GLOB EXAMPLE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../Example/Algorithms/${PROJECT_NAME}/*.cpp)

if (LOCALBUILD)
    include(${SOLUTION_SOURCE_DIR}/cmake/localbuild.cmake)
    include(cmake/specificlocalbuild.cmake)
endif ()


message("------------Add Library: ${PROJECT_NAME}")
add_library(${PROJECT_NAME} STATIC ${LOCAL_SOURCES} ${LOCALBUILD_SOURCES})
target_link_libraries(
        ${PROJECT_NAME}
        ${TARGET_STATIC_NAME}
        ${LOCAL_COMPUTE_LIBS}
        ${LOCAL_OPENCV_LIBS}
        ${LOCAL_EXTRA_DEPLOY_TARGET_LIBRARY}
        ENGIN)


message("------------Add Executable: ${PROJECT_NAME}_UnitTest_D")
add_executable(${PROJECT_NAME}_UnitTest_D ${TEST_SOURCES})
target_link_libraries(
        ${PROJECT_NAME}_UnitTest_D

        ${TARGET_NAME}
        ${COMPUTE_LIBS}
        ${OPENCV_LIBS}
        ${GTEST_LIBRARY}
        ${YAML_CPP_LIBRARY}
        ${EXTRA_DEPLOY_TARGET_LIBRARY}
        ENGIN)


message("------------Add Executable: ${PROJECT_NAME}_Example_D")
add_executable(${PROJECT_NAME}_Example_D ${EXAMPLE_SOURCES})
target_link_libraries(
        ${PROJECT_NAME}_Example_D
        ${TARGET_NAME}
        ${OPENCV_LIBS}
        ${EXTRA_DEPLOY_TARGET_LIBRARY}
        ENGIN)

message("===============end build ${PROJECT_NAME}==================")
INSTALL(TARGETS ${PROJECT_NAME}_UnitTest_D ${PROJECT_NAME}_Example_D
        RUNTIME DESTINATION bin/Algorithms/${PROJECT_NAME}
        LIBRARY DESTINATION lib)

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.h DESTINATION include)


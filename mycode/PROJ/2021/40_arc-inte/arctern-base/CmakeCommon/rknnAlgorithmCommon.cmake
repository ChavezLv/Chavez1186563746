
add_definitions(-fvisibility=hidden -fPIC)

#-------------------- add source and link libraries --------------------

file(GLOB LOCAL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/rknn/*.cpp)
set(LOCAL_SOURCES ${LOCAL_SOURCES} ${LOCAL_SOURCES02})
list(APPEND ALGSOURCES ${LOCAL_SOURCES})
set(ALGSOURCES ${ALGSOURCES} PARENT_SCOPE)

#--------- add source and link libraries --------------------
file(GLOB EXAMPLE_SOURCES
    ${SOLUTION_SOURCE_DIR}/Example/Algorithms/${PROJECT_NAME}/*.cpp)

file(GLOB TEST_SOURCES
    ${SOLUTION_SOURCE_DIR}/UnitTest/UnitTestBase/*.cpp
        ${SOLUTION_SOURCE_DIR}/UnitTest/main.cpp
    ${SOLUTION_SOURCE_DIR}/UnitTest/RknnAlgorithms/${PROJECT_NAME}/*.cpp
        ${SOLUTION_SOURCE_DIR}/UnitTest/Algorithms/${PROJECT_NAME}/groundtruth.cpp
    ${SOLUTION_SOURCE_DIR}/src/common/autoTime.cpp
    ${SOLUTION_SOURCE_DIR}/src/common/encryx.cpp)

if (LOCALBUILD)
    include(${SOLUTION_SOURCE_DIR}/cmake/localbuild.cmake)
    include(cmake/specificlocalbuild.cmake)
endif ()

add_library(${PROJECT_NAME} STATIC
    ${LOCAL_SOURCES} ${LOCALBUILD_SOURCES})

add_executable(${PROJECT_NAME}_UnitTest ${TEST_SOURCES})
add_executable(${PROJECT_NAME}_Example ${EXAMPLE_SOURCES})
add_executable(${PROJECT_NAME}_UnitTest_D ${TEST_SOURCES})
add_executable(${PROJECT_NAME}_Example_D ${EXAMPLE_SOURCES})

target_link_libraries(
    ${PROJECT_NAME}
    ${TARGET_STATIC_NAME}
    ${LOCAL_OPENCV_LIBS}
    ${LOCAL_COMPUTE_LIBS}
    ${LOCAL_EXTRA_DEPLOY_TARGET_LIBRARY})

target_link_libraries(
    ${PROJECT_NAME}_UnitTest
    ${PROJECT_NAME}
    ${OPENCV_LIBS}
    ${EXTRA_DEPLOY_TARGET_LIBRARY}
    ${GTEST_LIBRARY}
    ${YAML_CPP_LIBRARY})

target_link_libraries(
    ${PROJECT_NAME}_Example
    ${PROJECT_NAME}
    ${OPENCV_LIBS}
    ${EXTRA_DEPLOY_TARGET_LIBRARY})

target_link_libraries(
    ${PROJECT_NAME}_UnitTest_D
    ${TARGET_NAME}
    ${GTEST_LIBRARY}
    ${YAML_CPP_LIBRARY})

target_link_libraries(
    ${PROJECT_NAME}_Example_D
    ${TARGET_NAME}
    )

#-------------------- add source and link libraries --------------------
INSTALL(TARGETS ${PROJECT_NAME}_UnitTest_D ${PROJECT_NAME}_Example_D
        RUNTIME DESTINATION bin/Algorithms/${PROJECT_NAME}
        LIBRARY DESTINATION lib)

set(ALG_BASE_DIR ${SOLUTION_SOURCE_DIR}/src/Algorithms/${PROJECT_NAME})
set(EXAMPLE_DIR ${SOLUTION_SOURCE_DIR}/Example/Algorithms/${PROJECT_NAME})

INSTALL(DIRECTORY ${EXAMPLE_DIR} DESTINATION examples)
INSTALL(DIRECTORY ${ALG_BASE_DIR}/Docs/ DESTINATION docs PATTERN *.md)
INSTALL(DIRECTORY ${ALG_BASE_DIR}/Docs/ DESTINATION docs PATTERN *.txt)
INSTALL(FILES ${ALG_BASE_DIR}/${PROJECT_NAME}.h DESTINATION include)

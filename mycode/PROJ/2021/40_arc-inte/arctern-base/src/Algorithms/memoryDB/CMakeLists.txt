cmake_minimum_required(VERSION 3.1)

project(memoryDB)

set(ARCTERN_memoryDB_VERSION_MAJOR    1)
set(ARCTERN_memoryDB_VERSION_MINOR    0)
set(ARCTERN_memoryDB_VERSION_REVISION 0)

#-------------------- add source and link libraries --------------------

file(GLOB LOCAL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

list(APPEND ALGSOURCES ${LOCAL_SOURCES})
set(ALGSOURCES ${ALGSOURCES} PARENT_SCOPE)
add_definitions(-fvisibility=hidden -fPIC)

#--------- add source and link libraries --------------------
#file(GLOB TEST_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/../../../UnitTest/UnitTestBase/*.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/../../../UnitTest/main.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/../../../UnitTest/Algorithms/${PROJECT_NAME}/*.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/common/autoTime.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/common/encryx.cpp
#        )

file(GLOB EXAMPLE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../Example/Algorithms/${PROJECT_NAME}/*.cpp)

if (LOCALBUILD)
    include(${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake/localbuild.cmake)
    include(cmake/specificlocalbuild.cmake)
endif ()

if (${DEPLOY_TARGET} STREQUAL "linux")
    file(GLOB LOCAL_IMPL ${CMAKE_CURRENT_SOURCE_DIR}/cpu/*.cpp)
endif ()
message("------------ADD ${PROJECT_NAME}")
add_library(${PROJECT_NAME} STATIC ${LOCAL_SOURCES} ${LOCALBUILD_SOURCES})
#add_executable(${PROJECT_NAME}_UnitTest ${TEST_SOURCES})
add_executable(${PROJECT_NAME}_Example ${EXAMPLE_SOURCES})
#add_executable(${PROJECT_NAME}_UnitTest_D ${TEST_SOURCES})
add_executable(${PROJECT_NAME}_Example_D ${EXAMPLE_SOURCES})

target_link_libraries(${PROJECT_NAME}
        ${TARGET_STATIC_NAME}        ${LOCAL_OPENCV_LIBS}
        ${LOCAL_COMPUTE_LIBS}        ${LOCAL_EXTRA_DEPLOY_TARGET_LIBRARY})
target_link_libraries(${PROJECT_NAME}_Example
        ${PROJECT_NAME}        ${OpenCV_LIBS}
        ${EXTRA_DEPLOY_TARGET_LIBRARY})

target_link_libraries(
        ${PROJECT_NAME}_Example_D
        ${TARGET_NAME}
)
#-------------------- add source and link libraries --------------------
#INSTALL(TARGETS ${PROJECT_NAME}_UnitTest_D
#        ${PROJECT_NAME}_Example_D
#        RUNTIME DESTINATION bin/Algorithms/${PROJECT_NAME}
#        LIBRARY DESTINATION lib
#        )
#INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Docs/ DESTINATION docs PATTERN *.md)
#INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Docs/ DESTINATION docs PATTERN *.txt)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.h DESTINATION include)

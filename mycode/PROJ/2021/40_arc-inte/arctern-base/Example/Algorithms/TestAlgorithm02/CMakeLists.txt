cmake_minimum_required(VERSION 3.1)

project(ExampleTestAlgorithm02)

set(ARCTERN_ExampleTestAlgorithm02_VERSION_MAJOR    1)
set(ARCTERN_ExampleTestAlgorithm02_VERSION_MINOR    0)
set(ARCTERN_ExampleTestAlgorithm02_VERSION_REVISION 0)

add_definitions(-fvisibility=hidden  -fPIC)
file(GLOB LOCAL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

if(LOCALBUILD)
    include(cmake/localbuild.cmake)
endif()

#message(${LOCAL_SOURCES})
incLude_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty/include)
incLude_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../src)
#message("${CMAKE_CURRENT_SOURCE_DIR}/../../../src")
####################生成可执行文件#######################
add_executable(${PROJECT_NAME}  ${LOCAL_SOURCES})
target_link_libraries(${PROJECT_NAME} ${TARGET_STATIC_NAME} ${LOCALBUILD_LIBS})




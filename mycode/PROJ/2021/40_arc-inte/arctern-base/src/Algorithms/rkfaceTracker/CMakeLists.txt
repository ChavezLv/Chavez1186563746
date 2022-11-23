cmake_minimum_required(VERSION 3.1)

project(rkfaceTracker)

set(ARCTERN_rkfaceTracker_VERSION_MAJOR    1)
set(ARCTERN_rkfaceTracker_VERSION_MINOR    0)
set(ARCTERN_rkfaceTracker_VERSION_REVISION 0)

#-------------------- add source and link libraries --------------------
file(GLOB LOCAL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/arm/fe_compare/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/arm/lk_track/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/arm/nms/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/arm/resize/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/arm/stn/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/arm/*.c)

list(APPEND ALGSOURCES ${LOCAL_SOURCES})
set(ALGSOURCES ${ALGSOURCES} PARENT_SCOPE)
add_definitions( -fPIC )
add_definitions(-D__ARM_NEON__)
add_definitions(-D__ARM_NEON)

file(GLOB EXAMPLE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../Example/Algorithms/${PROJECT_NAME}/*.cpp)

if (LOCALBUILD)
    include(${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake/localbuild.cmake)
    include(cmake/specificlocalbuild.cmake)
endif ()

message("${LOCAL_SOURCES}")
add_library(${PROJECT_NAME} STATIC ${LOCAL_SOURCES} ${LOCALBUILD_SOURCES})

#add_executable(${PROJECT_NAME}_Example ${EXAMPLE_SOURCES})
add_executable(${PROJECT_NAME}_Example_D ${EXAMPLE_SOURCES})

#target_link_libraries(${PROJECT_NAME}
#        ${TARGET_STATIC_NAME}         ${OpenCV_LIBS}
#        ${LOCAL_COMPUTE_LIBS}        ${LOCAL_EXTRA_DEPLOY_TARGET_LIBRARY})

#target_link_libraries(${PROJECT_NAME}_Example
#        ${PROJECT_NAME}        ${OpenCV_LIBS}
#        ${EXTRA_DEPLOY_TARGET_LIBRARY})

target_link_libraries(${PROJECT_NAME}_Example_D
        ${TARGET_NAME})

INSTALL(DIRECTORY ${SOLUTION_SOURCE_DIR}/Example/Algorithms/${PROJECT_NAME} DESTINATION examples)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.h DESTINATION include)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/define.h DESTINATION include)
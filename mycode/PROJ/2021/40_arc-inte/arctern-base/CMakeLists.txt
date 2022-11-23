cmake_minimum_required(VERSION 3.1)
project(arctern-base LANGUAGES C CXX)
set(Arctern_VERSION_MAJOR    0)
set(Arctern_VERSION_MINOR    1)
set(Arctern_VERSION_REVISION 1)

#-------------------- C & C++ --------------------
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fopenmp")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

set(Arctern_PROJECT_PATH ${CMAKE_CURRENT_LIST_DIR})
set(SOLUTION_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

configure_file("${PROJECT_SOURCE_DIR}/src/version.h.in"
    "${PROJECT_SOURCE_DIR}/src/version.h" )

configure_file("${PROJECT_SOURCE_DIR}/UnitTest/config.h.in"
    "${PROJECT_SOURCE_DIR}/UnitTest/config.h")

include(cmake/colors.cmake)

if(ARCTERN_EXPORT_API)
    add_definitions(-DDLL_EXPORT)
endif()
if(PLAIN_MODEL)
    add_definitions(-DPLAIN_MODEL)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

if(WITH_PYTHON)
    find_package(pybind11 REQUIRED)
endif()

#-------------------- TARGET_NAME and TARGET_STATIC_NAME --------------------

if(CMAKE_BUILD_TYPE)
    message("your CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
else()
    set(CMAKE_BUILD_TYPE Release)
endif()
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message("${Green}================ok This is Relsease  version!!!!===============${ColourReset}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall")
    set(TARGET_NAME ${PROJECT_NAME})
elseif( ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message("${BoldRed}===========WARNING: This is debug  version!!!!=============${ColourReset}")
    #set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -ggdb -O0")
    #set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -ggdb -O0")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb3 -Wall -Wextra -pedantic ")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3 -Wall -Wextra -pedantic ")
    set(TARGET_NAME ${PROJECT_NAME}_Debug)
    if(CODECOVERAGE)
        message("code coverage -------------- added in main cmakelist.txt")
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage -lgcov")
    endif()
else()
    message(FATAL_ERROR "CMAKE_BUILD_TYPE must be Release or Debug")
    set(TARGET_NAME ${PROJECT_NAME}_Unknown)
    exit()
endif()



set(TARGET_STATIC_NAME ${TARGET_NAME}_Static)

#-------------------- some compiler options --------------------
set(DEPLOY_TARGET linux CACHE STRING "Only enable this in production environment")
set_property(CACHE DEPLOY_TARGET PROPERTY STRINGS osx linux arm android win32 win64)

#set(PLATFORM_LINUX ON)
#-------------------- Third party libraries' path --------------------
set(THIRD_PARTY_LIB_ROOT ${CMAKE_CURRENT_LIST_DIR}/../arctern-libs)
message("----THIRD_PARTY_LIB_ROOT => ${THIRD_PARTY_LIB_ROOT}")
if(NOT EXISTS ${THIRD_PARTY_LIB_ROOT})
    message(FATAL_ERROR "${BoldRed} cannot find ${THIRD_PARTY_LIB_ROOT} !!!!!!${ColourReset}")
endif()

# must load tvm static symbol first
message("----PROJECT_SOURCE_DIR => ${PROJECT_SOURCE_DIR} ")
include(cmake/TvmModelStaticLib.cmake)

message("----start to ${DEPLOY_TARGET}.cmake")
include(${CMAKE_SOURCE_DIR}/SystemCmake/${DEPLOY_TARGET}.cmake)
include_directories(${THIRD_PARTY_INC_PATH})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
       ${CMAKE_CURRENT_SOURCE_DIR}/src)


file(GLOB SOURCES
        src/Base/*.cpp
        src/log/*.cc
        src/common/*.cpp
        src/Processors/alignFace.cpp
        src/Processors/opencvPreprocessor.cpp
        src/Processors/ImageFormatConverters/*.cpp)

if(TRT_ALLOWED)

    list(APPEND SOURCES
            src/Processors/NppPreprocessorEx.cpp
            src/Base/trt/ArcternAlgorithmEx.cpp
            src/Base/trt/TrtYoloV3.cpp
            src/common/trt/TrtAlgorithmUtil.cpp)

    #add_definitions(-DTRT_ALLOWED)
    message(STATUS "----goto src/ImgPreProcess")
    #add_subdirectory(src/ImgPreProcess)
    include(${CMAKE_SOURCE_DIR}/src/ImgPreProcess/ImgPreprocessor.cmake)
    message(STATUS "${ImgProc}")
endif(TRT_ALLOWED)

if(ATLAS_ALLOWED)
    file(GLOB ATLAS_SOURCE src/common/atlas/*.cpp src/Base/atlas/*.cpp)
    message("----${ATLAS_SOURCE}")
    list(APPEND SOURCES ${ATLAS_SOURCE} src/Processors/atlasAclPreprocess.cpp)

endif(ATLAS_ALLOWED)

message(${SOURCES})

set(SOURCES ${SOURCES} ${EXTRA_SYSTEM_SOURCES})
#message("----${SOURCES}")

#-------------------- generate library  --------------------
message("----goto src/Engin/CMakeLists.txt")
add_subdirectory(src/Engin)


if(SCENE)
    message("----start to ${SCENE}.cmake")
    include(${CMAKE_SOURCE_DIR}/SceneCmake/${SCENE}.cmake)
endif()

message("----ALGSOURCES:${ALGSOURCES}")
#-------------------- generate library  --------------------

add_library(${TARGET_NAME} SHARED ${SOURCES} ${SCENE_EXTRA_SOURCE} ${ALGSOURCES} ${ENGINSOURCES})
add_library(${TARGET_STATIC_NAME} STATIC ${SOURCES} ${SCENE_EXTRA_SOURCE}  ${ALGSOURCES} ${ENGINSOURCES})


add_dependencies(${TARGET_STATIC_NAME} ${TARGET_NAME})

if(LIBRARY_OUTPUT_NAME)
    message(${LIBRARY_OUTPUT_NAME})
    set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME ${LIBRARY_OUTPUT_NAME})
endif()

#-------------------- link third-party library  --------------------



target_link_libraries(${TARGET_NAME} ${OPENCV_LIBS} ENGIN ${COMPUTE_LIBS}
                     ${EXTRA_DEPLOY_TARGET_LIBRARY} ${EXTRA_SCENE_LIBS} ${EXTRA_SYSTEM_LIBS} ${ImgProc} )
target_link_libraries(${TARGET_STATIC_NAME} ${OPENCV_LIBS} ENGIN ${COMPUTE_LIBS}
                      ${EXTRA_DEPLOY_TARGET_LIBRARY} ${EXTRA_SCENE_LIBS} ${EXTRA_SYSTEM_LIBS} ${ImgProc} )


set_target_properties(${TARGET_STATIC_NAME} PROPERTIES OUTPUT_NAME ${TARGET_NAME})
set_target_properties(${TARGET_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(${TARGET_STATIC_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(${TARGET_NAME} PROPERTIES VERSION "${Arctern_VERSION_MAJOR}.${Arctern_VERSION_MINOR}.${Arctern_VERSION_REVISION}" )

#-------------------- INSTALL  --------------------
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/version.h DESTINATION include)
message("----Install Path => ${CMAKE_INSTALL_PREFIX}")
install(TARGETS ${TARGET_NAME}
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION libstatic)

INSTALL(FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/arctern.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/result.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/runparameter.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/initparameter.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/algorithminterface.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/atlasInit.h
        DESTINATION include/include)


if(WITH_PYTHON)
    set(PYTHON_ARCTERN "arcternbase")
    pybind11_add_module(${PYTHON_ARCTERN} SHARED ${SOURCES} ${SCENE_EXTRA_SOURCE} ${ALGSOURCES} ${ENGINSOURCES} src/python/_arctern.cpp src/python/ndarray_converter.cpp)

    target_link_libraries(${PYTHON_ARCTERN}  PRIVATE    ${OPENCV_LIBS}  ENGIN   ${COMPUTE_LIBS}
        ${EXTRA_DEPLOY_TARGET_LIBRARY}   ${EXTRA_SCENE_LIBS} ${EXTRA_SYSTEM_LIBS}
        )
    set_target_properties(${PYTHON_ARCTERN} PROPERTIES VERSION "${Arctern_VERSION_MAJOR}.${Arctern_VERSION_MINOR}.${Arctern_VERSION_REVISION}" )

    install(TARGETS ${PYTHON_ARCTERN}
            COMPONENT  python
            LIBRARY DESTINATION lib
            RUNTIME DESTINATION bin
	    ARCHIVE DESTINATION libstatic)

endif()





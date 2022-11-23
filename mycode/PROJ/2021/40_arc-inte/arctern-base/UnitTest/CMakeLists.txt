project(ArcternUnitTest)

file(GLOB_RECURSE UNITTEST_SRCS ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

include(cmake/localbuild.cmake)

#find_package(PkgConfig)
#pkg_search_module(YAML REQUIRED yaml-cpp)
#include_directories(${YAML_INCLUDES_DIRS})
#link_directories(${YAML_LIBS_DIRS})

#FOREACH(FILE_PATH ${UNITTEST_SRCS})
#    # message("==========================")
#    MESSAGE("unit ${FILE_PATH}")
#    if(NOT FILE_PATH MATCHES "rununittest*")
#        # MESSAGE("====+++${FILE_PATH}")
#        LIST( APPEND NEW_UNITTEST_SRCS ${FILE_PATH})
#      #STRING(REGEX REPLACE "(rununittest*)" "1"  FILE_NAME "${FILE_PATH}")
#    endif()
#    MESSAGE("${FILE_NAME}")
#
#   # message("==========================")
#ENDFOREACH(FILE_PATH)

message("${CMAKE_CXX_FLAGS}")

###message("@@@@@@@@@@${NEW_UNITTEST_SRCS}")


add_executable(${PROJECT_NAME}_UNITTEST ${UNITTEST_SRCS} )
target_link_libraries(${PROJECT_NAME}_UNITTEST  ${TARGET_STATIC_NAME} ${LOCALBUILD_LIBS} yaml-cpp gtest pthread)

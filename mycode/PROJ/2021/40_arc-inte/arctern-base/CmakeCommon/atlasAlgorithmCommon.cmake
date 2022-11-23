#-------------------- add source and link libraries --------------------

message("===============start to build ${PROJECT_NAME}==================")
file(GLOB LOCAL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.cpp)
file(GLOB LOCAL_ATLAS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/atlas/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp
        )
list(APPEND LOCAL_SOURCES ${LOCAL_ATLAS_SOURCES})
message("${LOCAL_SOURCES}")
list(APPEND ALGSOURCES ${LOCAL_SOURCES})

#message("${ALGSOURCES}")
set(ALGSOURCES ${ALGSOURCES} PARENT_SCOPE)

file(GLOB TEST_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../UnitTest/UnitTestBase/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../UnitTest/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../UnitTest/Algorithms/${PROJECT_NAME}/atlas/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../UnitTest/Algorithms/${PROJECT_NAME}/groundtruth.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/common/autoTime.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/common/encryx.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/common/atlas/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/Processors/atlasAclPreprocess.cpp)

file(GLOB EXAMPLE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../Example/Algorithms/${PROJECT_NAME}/atlas/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/atlasInit.h)

if (LOCALBUILD)
    include(${SOLUTION_SOURCE_DIR}/cmake/localbuild.cmake)
    include(cmake/specificlocalbuild.cmake)
endif ()


# message("------------Add Library: ${PROJECT_NAME}")
# add_library(${PROJECT_NAME} STATIC ${LOCAL_SOURCES})
# target_link_libraries(
#         ${PROJECT_NAME}
#         ${TARGET_STATIC_NAME}
#         ${LOCAL_COMPUTE_LIBS}
#         ${LOCAL_OPENCV_LIBS}
#         ${LOCAL_EXTRA_DEPLOY_TARGET_LIBRARY}
#         ENGIN)


message("------------Add Executable: ${PROJECT_NAME}_UnitTest_D")
add_executable(${PROJECT_NAME}_UnitTest_D ${TEST_SOURCES})
target_link_libraries(
        ${PROJECT_NAME}_UnitTest_D
        ENGIN
        ${TARGET_NAME}
        ${COMPUTE_LIBS}
        ${OPENCV_LIBS}
        ${GTEST_LIBRARY}
        ${YAML_CPP_LIBRARY}
        ${EXTRA_DEPLOY_TARGET_LIBRARY}
        )

# if atlas open this, cross compile will failed
#message("------------Add Executable: ${PROJECT_NAME}_Example_D")
#message("TARGET_NAME:${TARGET_NAME}")

#add_executable(${PROJECT_NAME}_Example_D ${EXAMPLE_SOURCES})
#target_link_libraries(
#        ${PROJECT_NAME}_Example_D
#        ${TARGET_NAME}
#        ${OPENCV_LIBS}
#        ${EXTRA_DEPLOY_TARGET_LIBRARY}
#        ENGIN)

message("===============end build ${PROJECT_NAME}==================")

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.h DESTINATION include)
INSTALL(TARGETS ${PROJECT_NAME}_UnitTest_D 
    RUNTIME DESTINATION bin/Algorithms/${PROJECT_NAME}
    LIBRARY DESTINATION lib)

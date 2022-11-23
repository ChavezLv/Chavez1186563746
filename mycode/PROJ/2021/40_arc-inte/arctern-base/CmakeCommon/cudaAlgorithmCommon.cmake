
#-------------------- local algorithm sources --------------------
file(GLOB LOCAL_SOURCES01 ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
set(LOCAL_SOURCES ${LOCAL_SOURCES} ${LOCAL_SOURCES01})
list(APPEND ALGSOURCES ${LOCAL_SOURCES})
set(ALGSOURCES ${ALGSOURCES} PARENT_SCOPE)
add_definitions(-fvisibility=hidden -fPIC)

#------------------- local test sources --------------------
file(GLOB TEST_SOURCES01
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../UnitTest/UnitTestBase/*.cpp
       ${CMAKE_CURRENT_SOURCE_DIR}/../../../UnitTest/main.cpp
       ${CMAKE_CURRENT_SOURCE_DIR}/../../../UnitTest/CudaAlgorithms/*.cpp
       ${CMAKE_CURRENT_SOURCE_DIR}/../../../UnitTest/CudaAlgorithms/CpUAndGpuPointerTestMethod.h
       ${CMAKE_CURRENT_SOURCE_DIR}/../../../UnitTest/CudaAlgorithms/${PROJECT_NAME}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/common/autoTime.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/common/encryx.cpp
        )
set(TEST_SOURCES ${TEST_SOURCES} ${TEST_SOURCES01})
list(APPEND GLOBLE_TEST_SOURCES ${TEST_SOURCES})
set(GLOBLE_TEST_SOURCES ${GLOBLE_TEST_SOURCES} PARENT_SCOPE)

#------------------- local example sources --------------------
file(GLOB EXAMPLE_SOURCES01
     ${SOLUTION_SOURCE_DIR}/Example/CudaLinuxAlgorithms/${PROJECT_NAME}/*.cpp)
set(EXAMPLE_SOURCES ${EXAMPLE_SOURCES} ${EXAMPLE_SOURCES01})
if (LOCALBUILD)
    include(${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake/localbuild.cmake)
    include(cmake/specificlocalbuild.cmake)
endif ()
message("----------------ADD ${PROJECT_NAME} ${PROJECT_NAME}_UnitTest ${PROJECT_NAME}_Example")
add_library(${PROJECT_NAME} STATIC ${LOCAL_SOURCES} ${LOCALBUILD_SOURCES})
add_executable(${PROJECT_NAME}_UnitTest ${TEST_SOURCES})
add_executable(${PROJECT_NAME}_Example ${EXAMPLE_SOURCES})
add_executable(${PROJECT_NAME}_UnitTest_D ${TEST_SOURCES})
add_executable(${PROJECT_NAME}_Example_D ${EXAMPLE_SOURCES})

if(CODECOVERAGE)
if(CMAKE_BUILD_TYPE MATCHES Debug)
    #message("${PROJECT_NAME} will build for code coverage!! ${PROJECT_NAME}_UnitTest_Coverage")
    #include(CodeCoverage)
    #setup_target_for_coverage(${PROJECT_NAME}_UnitTest_Coverage ${PROJECT_NAME}_UnitTest_D coverage)
    #setup_target_for_coverage(${PROJECT_NAME}_UnitTest_Cobertura ${PROJECT_NAME}_UnitTest_D coverage
    #        --gtest_output=xml:coverage.junit.xml)
else()
    message("if you want use code coverage tool, please use Debug mode!!!!")
endif()
endif()

target_link_libraries(${PROJECT_NAME}
        ${TARGET_STATIC_NAME}   
        ${LOCAL_OPENCV_LIBS}
        ${LOCAL_COMPUTE_LIBS}  
        ${LOCAL_EXTRA_DEPLOY_TARGET_LIBRARY}
        )
target_link_libraries(${PROJECT_NAME}_UnitTest
        ${PROJECT_NAME}      ${TARGET_STATIC_NAME}       ${OPENCV_LIBS}
        ${EXTRA_DEPLOY_TARGET_LIBRARY}   gtest        yaml-cpp)
target_link_libraries(${PROJECT_NAME}_Example
       ${PROJECT_NAME} ${TARGET_STATIC_NAME}    
        ${OPENCV_LIBS}
        ${EXTRA_DEPLOY_TARGET_LIBRARY})
target_link_libraries(${PROJECT_NAME}_UnitTest_D
       ${TARGET_NAME}        ${OPENCV_LIBS}
        ${EXTRA_DEPLOY_TARGET_LIBRARY}      gtest     yaml-cpp)
target_link_libraries(${PROJECT_NAME}_Example_D
        ${TARGET_NAME}        ${OPENCV_LIBS}
        ${EXTRA_DEPLOY_TARGET_LIBRARY})

#-------------------- add source and link libraries --------------------
INSTALL(TARGETS ${PROJECT_NAME}_UnitTest_D
        ${PROJECT_NAME}_Example_D
        RUNTIME DESTINATION bin/CudaAlgorithms/${PROJECT_NAME}
        LIBRARY DESTINATION lib
        )
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Docs/ DESTINATION docs PATTERN *.md)
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Docs/ DESTINATION docs PATTERN *.txt)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.h DESTINATION include)

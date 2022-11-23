file(GLOB OtherTestSources ${CMAKE_CURRENT_SOURCE_DIR}/UnitTest/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UnitTest/Others/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UnitTest/Others/${DEPLOY_TARGET}/*.cpp
        )
#message("======other unittest file :    ${OtherTestSources}")
add_executable(OtherTest_UnitTest ${OtherTestSources})
target_link_libraries(OtherTest_UnitTest
        ${TARGET_STATIC_NAME}
        ${OPENCV_LIBS}
        ${EXTRA_DEPLOY_TARGET_LIBRARY} gtest)
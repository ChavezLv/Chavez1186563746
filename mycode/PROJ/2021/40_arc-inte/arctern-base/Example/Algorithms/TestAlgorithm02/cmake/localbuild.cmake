
if( ${CMAKE_BUILD_TYPE} MATCHES "" OR ${CMAKE_BUILD_TYPE} MATCHES "Release")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    set(TARGET_NAME ${PROJECT_NAME})
elseif( ${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -ggdb -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb -O0")
    set(TARGET_NAME ${PROJECT_NAME}_Debug)
else()
    message(FATAL_ERROR "CMAKE_BUILD_TYPE must be Release or Debug")
endif()
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty/${DEPLOY_TARGET}/lib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../src/Algorithms/TestAlgorithm02/build)
#message("${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty/${DEPLOY_TARGET}/lib")
set(LOCALBUILD_LIBS
   libTestAlgorithm.a
   opencv_video
   opencv_videoio
   opencv_videostab
   opencv_calib3d
   opencv_features2d
   opencv_highgui
   opencv_flann
   opencv_imgcodecs
   opencv_imgproc
   libwebp
   libpng
   libjpeg
   libtiff
   libjasper
   IlmImf
   opencv_ml
   opencv_objdetect
   opencv_photo
   opencv_shape
   opencv_stitching
   opencv_superres
   opencv_core
   )

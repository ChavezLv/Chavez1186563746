link_directories(src/AlgorithmsHisi/TestAlgorithm)

#################algorithms##################
add_definitions(-DHISI_ALLOWED)
add_subdirectory(src/Algorithms/faceDetectTiny)
add_subdirectory(src/Algorithms/faceLivenessIr)
add_subdirectory(src/Algorithms/faceQuality)
add_subdirectory(src/Algorithms/faceMask)
add_subdirectory(src/Algorithms/faceGluon)
add_subdirectory(src/Algorithms/faceNoiseMotionblur)
add_subdirectory(src/Algorithms/faceImageColor)
add_subdirectory(src/Algorithms/facialLandmark)
add_subdirectory(src/Algorithms/hisiFaceTracker)


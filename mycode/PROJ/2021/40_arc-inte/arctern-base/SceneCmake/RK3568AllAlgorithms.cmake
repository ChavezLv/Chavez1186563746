add_definitions(-DRKNN2_ALLOWED)

##################algorithms##################
add_subdirectory(src/Algorithms/faceMask)
add_subdirectory(src/Algorithms/faceGluon)
add_subdirectory(src/Algorithms/facialLandmark)
add_subdirectory(src/Algorithms/faceDetectTiny)
add_subdirectory(src/Algorithms/faceQuality)
add_subdirectory(src/Algorithms/faceNoiseMotionblur)
add_subdirectory(src/Algorithms/faceLivenessIr)
add_subdirectory(src/RknnAlgorithms/rkfaceTracker)

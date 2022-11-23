add_definitions(-DTRT_ALLOWED)


message("--------goto src/Algorithms/faceGluon")
add_subdirectory(src/Algorithms/faceGluon)

message("--------goto src/Algorithms/faceBasicMultiAttr")
add_subdirectory(src/Algorithms/faceBasicMultiAttr)


message("--------goto src/Algorithms/faceDetectRTN")
add_subdirectory(src/Algorithms/faceDetectRTN)


message("--------goto src/Algorithms/faceExpression")
add_subdirectory(src/Algorithms/faceExpression)


message("--------goto src/Algorithms/faceQuality")
add_subdirectory(src/Algorithms/faceQuality)

message("--------goto src/Algorithms/facePose")
add_subdirectory(src/Algorithms/facePose)

message("--------goto src/Algorithms/facialLandmark")
add_subdirectory(src/Algorithms/facialLandmark)


message("--------goto src/Algorithms/personQuality")
add_subdirectory(src/Algorithms/personQuality)



message("--------goto src/Algorithms/faceNoiseMotionblur")
add_subdirectory(src/Algorithms/faceNoiseMotionblur)

message("--------goto src/Algorithms/multiDetectCenter")
add_subdirectory(src/Algorithms/multiDetectCenter)



message("--------goto src/Algorithms/personCompleteness")
add_subdirectory(src/Algorithms/personCompleteness)

message("--------goto src/Algorithms/personPose")
add_subdirectory(src/Algorithms/personPose)

message("--------goto src/Algorithms/personFeat")
add_subdirectory(src/Algorithms/personFeat)
add_subdirectory(src/Algorithms/memoryDB)


message("--------goto src/Algorithms/bannerDet")
add_subdirectory(src/Algorithms/bannerDet)

message("--------goto src/Algorithms/rubbishDetectTiny")
add_subdirectory(src/Algorithms/rubbishDetectTiny)

message("--------goto src/Algorithms/garbageOverflow")
add_subdirectory(src/Algorithms/garbageOverflow)



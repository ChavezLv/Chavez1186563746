if (MXNET_ALLOWED)
    add_definitions(-DMXNET_ALLOWED)

    add_subdirectory(src/Algorithms/memoryDB)

    #add_subdirectory(src/Algorithms/faceDetectRTN)
    #add_subdirectory(src/Algorithms/faceDetectTiny)
    #add_subdirectory(src/Algorithms/facialLandmark)
    #add_subdirectory(src/Algorithms/faceGluon)
    #add_subdirectory(src/Algorithms/ageGender)
    #add_subdirectory(src/Algorithms/faceMask)
    #add_subdirectory(src/Algorithms/faceGlass)
    #add_subdirectory(src/Algorithms/faceHat)
    #add_subdirectory(src/Algorithms/faceAbnomalLight)
    #add_subdirectory(src/Algorithms/faceNoiseMotionblur)
    #add_subdirectory(src/Algorithms/faceQuality)
    #add_subdirectory(src/Algorithms/faceOcclusion)
    #add_subdirectory(src/Algorithms/faceQualityPose)


    #add_subdirectory(src/Algorithms/faceLivenessIr)
    #add_subdirectory(src/Algorithms/faceLivenessRgb)
    #add_subdirectory(src/Algorithms/faceBangs)
    #add_subdirectory(src/Algorithms/faceExpression)
    #add_subdirectory(src/Algorithms/faceHeadwear)
    #add_subdirectory(src/Algorithms/facePose)
    #add_subdirectory(src/Algorithms/facePhone)
    #add_subdirectory(src/Algorithms/faceSmoke)
    #add_subdirectory(src/Algorithms/faceBasicMultiAttr)


    #add_subdirectory(src/Algorithms/personFeat)
    #add_subdirectory(src/Algorithms/licenseRecog)
    #add_subdirectory(src/Algorithms/personHeadwear)
    #add_subdirectory(src/Algorithms/personQuality)
    #add_subdirectory(src/Algorithms/multiDetectCenter)
    #add_subdirectory(src/Algorithms/personCompleteness)
    #add_subdirectory(src/Algorithms/personAttr)
    #add_subdirectory(src/Algorithms/vehicleAttr)


    #add_subdirectory(src/Algorithms/vehicleCycleAttr)
    #add_subdirectory(src/Algorithms/objectQuality)
    #add_subdirectory(src/Algorithms/cameraOcclusionDetect)
    #add_subdirectory(src/Algorithms/personPose)
    #add_subdirectory(src/Algorithms/crowdHeatmap)
    #add_subdirectory(src/Algorithms/dogDetTiny)
    #add_subdirectory(src/Algorithms/faceJoy)
    #add_subdirectory(src/Algorithms/faceType)

    #add_subdirectory(src/Algorithms/personScarf)
    #add_subdirectory(src/Algorithms/personUmbrella)
    add_subdirectory(src/Algorithms/licenseLandmark)

    #add_subdirectory(src/Algorithms/bannerDet)
    #add_subdirectory(src/Algorithms/garbageOverflow)
    #add_subdirectory(src/Algorithms/rubbishDetectTiny)

    #add_subdirectory(src/Algorithms/manHoleCoverDet)
    add_subdirectory(src/Algorithms/cargoIllegalDetect)
    #add_subdirectory(src/Algorithms/fireDet)
endif ()


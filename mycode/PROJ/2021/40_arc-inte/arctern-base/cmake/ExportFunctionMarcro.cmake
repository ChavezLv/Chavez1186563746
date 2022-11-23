#when you define EXPORT_ALL_FUN , all functions can be used defined in artern_manager.h arcterManager class 
if(EXPORT_ALL_FUN)
   add_definitions(-DEXPORT_ALL_FUN)
   message("${BoldGreen}===============ALL FUNCTION will be export=============${ColourReset}")
endif()

#when you define EXPORT_GET_FACE_FEAT_FUN you can use the function get_face_feat
if(EXPORT_GET_FACE_FEAT_FUN)
   add_definitions(-DEXPORT_GET_FACE_FEAT_FUN)
   message("${BoldGreen}===========add macro EXPORT_GET_FACE_FEAT_FUN===========${ColourGreen}")
endif()

# Find the vecLib libraries as part of Accelerate.framework or as standalon framework
#
# The following are set after configuration is done:
#  VECLIB_FOUND
#  vecLib_INCLUDE_DIR
#  vecLib_LINKER_LIBS


if(NOT APPLE)
  return()
endif()

set(__veclib_include_suffix "Frameworks/vecLib.framework/Versions/Current/Headers")

find_path(vecLib_INCLUDE_DIR vecLib.h
          DOC "vecLib include directory"
          PATHS 
                /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/Accelerate.framework/Versions/A/Frameworks/vecLib.framework/Versions/A/Headers/
          )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(vecLib DEFAULT_MSG vecLib_INCLUDE_DIR)

if(VECLIB_FOUND)
  if(vecLib_INCLUDE_DIR MATCHES "^/System/Library/Frameworks/vecLib.framework.*")
    set(vecLib_LINKER_LIBS -lcblas "-framework Accelerate")
    message(STATUS "Found standalone vecLib.framework")
  else()
    set(vecLib_LINKER_LIBS -lcblas "-framework Accelerate")
    message(STATUS "Found vecLib as part of Accelerate.framework " ${vecLib_INCLUDE_DIR})
  endif()

  mark_as_advanced(vecLib_INCLUDE_DIR)
else()
    if(APPLE)
        message(STATUS "vecLib not found")
    endif()
endif()

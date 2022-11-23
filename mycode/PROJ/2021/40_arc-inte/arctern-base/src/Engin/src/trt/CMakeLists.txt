
#set(TRT_CORES  ${TRT_ENGINS})

add_subdirectory(core/plugin)

file(GLOB  TRT_ENGINS
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/core/convert/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/core/parsers/*.cpp
        )

message("----------------######------------------${TRT_ENGINS}")
message("####################***fdasf8")

set(TRT_CORES ${TRT_ENGINS} ${PLUGIN_SOURCES} PARENT_SCOPE)
message("${TRT_ENGINS}")
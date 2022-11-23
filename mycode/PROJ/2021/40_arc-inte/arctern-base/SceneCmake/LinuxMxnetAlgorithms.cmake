
link_directories(src/Algorithms/TestAlgorithm)
link_directories(src/Algorithms/TestAlgorithm02)

#static library
set(EXTRA_SCENE_LIBS TestAlgorithm
                     TestAlgorithm02)

#################algorithms##################
add_subdirectory(src/Algorithms/TestAlgorithm)
add_subdirectory(src/Algorithms/TestAlgorithm02)

set(MXNET_ALLOWED true)

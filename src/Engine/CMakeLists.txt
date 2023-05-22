cmake_minimum_required(VERSION 3.16.3)

project(3DReconstruction)

# ==============================================================================
# Foucs C++11 support
# ==============================================================================
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#================================================================================
set(PROJECT_SOURCES
    Global.cpp 
    OS3DEngine.cpp
    ComputeFeatures.cpp
    IntrinsicsAnalysis.cpp
    ComputeMatches.cpp
    LinuxMessage.cpp
)

# 添加头文件路径
# Add executable
add_executable(R3D   ${PROJECT_SOURCES})
# link_directories(../../libs/openMVG)
target_link_libraries(R3D
  PRIVATE
    openMVG_system
    openMVG_image
    openMVG_features
    openMVG_multiview
    openMVG_sfm
    ${STLPLUS_LIBRARY}
    vlsift
    openMVG_exif
    # ${LEMON_LIBRARY}
    openMVG_graph
    openMVG_matching_image_collection
)
target_include_directories(R3D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(R3D PUBLIC ../../libs/openMVG)
target_include_directories(R3D PUBLIC ../../libs/openMVG/dependencies/cereal/include)


# include_directories(${CMAKE_SOURCE_DIR}/Engine)
# include_directories(${CMAKE_SOURCE_DIR}/Include)
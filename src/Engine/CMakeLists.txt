cmake_minimum_required(VERSION 3.16.3)

project(3DReconstruction)

# ==============================================================================
# Focus C++11 support
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
    FilterMatches.cpp
    SFM.cpp
    ColorizeStructure.cpp
    StructureFromKnownPoses.cpp
    ColorizedRobustTriangulation.cpp
    ExportToOpenMVS.cpp
    LinuxMessage.cpp
    DensifyPointCloud.cpp
    ReconstructMesh.cpp
    RefineMesh.cpp
    TextureTheMesh.cpp
    Utils.cpp
)

find_package(OpenMVS)
if(OpenMVS_FOUND)
	include_directories(${OpenMVS_INCLUDE_DIRS})
	add_definitions(${OpenMVS_DEFINITIONS})
endif()

# 添加头文件路径
# Add executable
add_executable(R3D   ${PROJECT_SOURCES})

find_package(Ceres QUIET HINTS ${CERES_DIR_HINTS})
if (NOT Ceres_FOUND)
  set(OpenMVG_USE_INTERNAL_CERES ON)
  set(CERES_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/libs/openMVG/third_party/ceres-solver/include
    ${CMAKE_SOURCE_DIR}/libs/openMVG/third_party/ceres-solver/internal/ceres/miniglog
    ${PROJECT_BINARY_DIR}/libs/openMVG/third_party/ceres-solver/config)
    FILE(READ "${CMAKE_SOURCE_DIR}/libs/openMVG/third_party/ceres-solver/VERSION" CERES_CONFIG)
    STRING(REGEX REPLACE "version ([0-9.]+).*" "\\1" CERES_VERSION ${CERES_CONFIG})
  set(CERES_LIBRARIES openMVG_ceres)
endif()

find_package(OpenMVS)
if(OpenMVS_FOUND)
	include_directories(${OpenMVS_INCLUDE_DIRS})
	add_definitions(${OpenMVS_DEFINITIONS})
endif()

FIND_PACKAGE(CUDA)
	if(CUDA_FOUND)
		set(CMAKE_CUDA_COMPILER "/usr/local/cuda/bin/nvcc")
		ENABLE_LANGUAGE(CUDA)
    string(REGEX MATCH "^[0-9]+" CUDA_MAJOR ${CMAKE_CUDA_COMPILER_VERSION})
		if(${CUDA_MAJOR} GREATER 10 AND CMAKE_CXX_COMPILER MATCHES "MSVC" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "19.29")
			message("Working around windows build failure with visual studio. Visual studio 16.10 introduced a compiler bug with compilng CUDA code with C++14. Set the cuda standard to 17 as a workaround.")
			set(CMAKE_CUDA_STANDARD 17)
		endif()
  endif()
# ==============================================================================
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
    openMVG_graph
    openMVG_matching_image_collection
    ${CERES_LIBRARIES}
    ${OPENMVS_LIBRARIES}
    IO
    Math
    MVS
    Common
)

target_include_directories(R3D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(R3D PUBLIC ../../libs/openMVG)
target_include_directories(R3D PUBLIC  ${CERES_INCLUDE_DIRS})
target_include_directories(R3D PUBLIC ../../libs/openMVS/libs/Common)
target_include_directories(R3D PUBLIC ../../libs/openMVS/libs)
target_include_directories(R3D PUBLIC ${EIGEN_INCLUDE_DIR_HINTS})
target_include_directories(R3D PRIVATE ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES} ${CUDA_INCLUDE_DIRS})
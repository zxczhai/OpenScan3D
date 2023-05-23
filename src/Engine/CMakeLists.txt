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
    FilterMatches.cpp
    SFM.cpp
    ColorizeStructure.cpp
    StructureFromKnownPoses.cpp
    ColorizedRobustTriangulation.cpp
    ExportToOpenMVS.cpp
    LinuxMessage.cpp
    Utils.cpp
)

# 添加头文件路径
# Add executable
add_executable(R3D   ${PROJECT_SOURCES})

# 手动设置 Ceres Solver 库的路径
# set(CERES_INCLUDE_DIRS "../../libs/openMVG/third_party/ceres-solver/include")
# set(CERES_LIBRARIES "../../libs/openMVG/third_party/ceres-solver/lib/libceres.so")
# 查找 Ceres Solver 库的头文件和库文件
# ==============================================================================
# Ceres
# ==============================================================================
# - external by default if CERES_DIR_HINTS or find_package found a valid Ceres
# - internal if ceres not found (ceres-solver+cxsparse+miniglog)
# ==============================================================================
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
)
target_include_directories(R3D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(R3D PUBLIC ../../libs/openMVG)
target_include_directories(R3D PUBLIC  ${CERES_INCLUDE_DIRS})

# include_directories(${CMAKE_SOURCE_DIR}/Engine)
# include_directories(${CMAKE_SOURCE_DIR}/Include)
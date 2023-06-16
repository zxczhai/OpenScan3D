cmake_minimum_required(VERSION 3.16.3)

project(OpenViewer)

# ==============================================================================
# Focus C++11 support
# ==============================================================================
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ================================================================================
find_package(OpenCV REQUIRED)
find_package(OpenMVS)
if(OpenMVS_FOUND)
  include_directories(${OpenMVS_INCLUDE_DIRS})
  add_definitions(${OpenMVS_DEFINITIONS})
endif()
set(PROJECT_SOURCES
    OpenViewer.cpp
)
set(OPENMVS_LIBRARIES IO Math MVS Common)
set(openMVG_LIBRARIES
  openMVG_system
  openMVG_image
  openMVG_features
  openMVG_multiview
  openMVG_sfm
  ${STLPLUS_LIBRARY}
  vlsift
  openMVG_exif
  openMVG_graph
  openMVG_matching_image_collection)
set(EIGEN_INCLUDE_DIR_HINTS ${PROJECT_SOURCE_DIR}/libs/openMVG/third_party/eigen)
find_package(Eigen QUIET)
add_executable(OpenViewer ${PROJECT_SOURCES})

target_link_libraries(OpenViewer
    PUBLIC
    Viewer
    ${OPENMVS_LIBRARIES}
    ${openMVG_LIBRARIES}
    ${OpenCV_LIBS}
)


target_include_directories(OpenViewer PUBLIC ../../libs/Viewer)
target_include_directories(OpenViewer PUBLIC ../../libs/openMVS/libs/Common)
target_include_directories(OpenViewer PUBLIC ../../libs/openMVS/libs)
target_include_directories(OpenViewer PUBLIC ${EIGEN_INCLUDE_DIR_HINTS})

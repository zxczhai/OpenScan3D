cmake_minimum_required(VERSION 3.16.3)

project(3DReconstruction)


find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})

# Add executable
add_executable(ViewRender Viewer.cpp)
target_include_directories(ViewRender PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(ViewRender PUBLIC ../../)

cmake_minimum_required(VERSION 3.16.3)

project(3DReconstruction)

# Find packages
# find_package(OpenMVG REQUIRED)
find_package(OpenMVS REQUIRED)
# find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# Add executable
add_executable(R3D test.cpp)

# Link libraries
# target_link_libraries(R3D PRIVATE  Qt5::Core Qt5::Widgets)

include_directories(${CMAKE_SOURCE_DIR}/Engine)
include_directories(${CMAKE_SOURCE_DIR}/Include)
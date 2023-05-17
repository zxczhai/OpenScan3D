cmake_minimum_required(VERSION 3.16.3)

project(OpenScan3D)

# Find packages
# find_package(OpenMVG REQUIRED)
find_package(OpenMVS REQUIRED)
# find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# 添加头文件路径
# Add executable
add_executable(R3D   Global.cpp OS3DEngine.cpp)
target_include_directories(R3D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# include_directories(${CMAKE_SOURCE_DIR}/Engine)
# include_directories(${CMAKE_SOURCE_DIR}/Include)
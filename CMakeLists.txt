cmake_minimum_required(VERSION 3.16.3)

# 设置项目名称
project(OpenScan3D)

add_subdirectory(${PROJECT_SOURCE_DIR}/libs/openMVG)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/openMVS)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/obj2stl)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/Viewer)

# 添加子目录 Engine 和 ViewRender
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Engine)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/GUI)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/OpenViewer)

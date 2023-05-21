cmake_minimum_required(VERSION 3.16.3)

# 设置项目名称
project(OpenScan3D)

# 添加子目录 Engine 和 ViewRender
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Engine)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/GUI)

# add_subdirectory(${PROJECT_SOURCE_DIR}/src/ViewRender)
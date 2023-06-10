cmake_minimum_required(VERSION 3.12)
project(obj2stl VERSION 1.0.0)

set (CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE source_files
    "include/obj2stl/*.h"
    "src/obj2stl/*.cpp"
)

add_library(obj2stl ${source_files})

include_directories(include/obj2stl)

# add_executable(obj2stl-exe src/main.cpp)

# target_link_libraries(obj2stl-exe PRIVATE obj2stl)

# set_target_properties(obj2stl-exe PROPERTIES OUTPUT_NAME obj2stl)


# enable_testing()

# add_test(NAME test1 COMMAND obj2stl ${CMAKE_SOURCE_DIR}/data/box.obj -o -)
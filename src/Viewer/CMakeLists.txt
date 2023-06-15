cmake_minimum_required(VERSION 3.16.3)

project(Viewer)

# ==============================================================================
# Focus C++11 support
# ==============================================================================
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(OPENMVS_LIBRARIES IO Math MVS Common)
set(VIEWER_NAME "OpenViewer")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
# Find OpenGL library and header files
find_package(OpenGL REQUIRED)

if(OPENGL_FOUND)
    message("Found OpenGL library: ${OPENGL_LIBRARIES}")
    message("Found OpenGL headers: ${OPENGL_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "OpenGL library not found")
endif()
# Find OpenMVS library and header files
find_package(OpenMVS)
if(OpenMVS_FOUND)
	include_directories(${OpenMVS_INCLUDE_DIRS})
	add_definitions(${OpenMVS_DEFINITIONS})
endif()
# Find X11 library
find_package(X11 REQUIRED)
# Find required packages
FIND_PACKAGE(GLEW QUIET)
if(GLEW_FOUND)
	INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIRS})
	ADD_DEFINITIONS(${GLEW_DEFINITIONS})
	MESSAGE(STATUS "GLEW ${GLEW_VERSION} found (include: ${GLEW_INCLUDE_DIRS})")
else()
	MESSAGE("-- Can't find GLEW. Continuing without it.")
	RETURN()
endif()
FIND_PACKAGE(glfw3 QUIET)
if(glfw3_FOUND)
	INCLUDE_DIRECTORIES(${glfw3_INCLUDE_DIRS})
	ADD_DEFINITIONS(${glfw3_DEFINITIONS})
	MESSAGE(STATUS "GLFW3 ${glfw3_VERSION} found (include: ${glfw3_INCLUDE_DIRS})")
else()
	MESSAGE("-- Can't find GLFW3. Continuing without it.")
	RETURN()
endif()

# List sources files
if(MSVC)
	FILE(GLOB LIBRARY_FILES_C "*.cpp" "*.rc")
else()
	FILE(GLOB LIBRARY_FILES_C "*.cpp")
endif()
FILE(GLOB LIBRARY_FILES_H "*.h" "*.inl")

add_executable(${VIEWER_NAME} ${LIBRARY_FILES_C} ${LIBRARY_FILES_H})

target_link_libraries(${VIEWER_NAME} PRIVATE MVS ${OPENGL_LIBRARIES} GLEW::GLEW glfw  ${OPENMVS_LIBRARIES} X11)
target_include_directories(${VIEWER_NAME} PRIVATE ../../libs/openMVS/libs/Common)
target_include_directories(${VIEWER_NAME} PRIVATE ../../libs/openMVS/libs)
target_include_directories(${VIEWER_NAME} PRIVATE ${EIGEN_INCLUDE_DIR_HINTS})
target_include_directories(${VIEWER_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})

# Manually set Common.h as the precompiled header
IF(CMAKE_VERSION VERSION_GREATER_EQUAL 3.16.0)
	TARGET_PRECOMPILE_HEADERS(${VIEWER_NAME} PRIVATE "Common.h")
endif()

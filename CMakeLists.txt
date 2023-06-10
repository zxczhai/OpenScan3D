cmake_minimum_required(VERSION 3.5)

project(OpenScan3D VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(X11 REQUIRED)

include_directories(${EXIV2_INCLUDE_DIRS})

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        myResource.qrc
        choose_file.h choose_file.cpp choose_file.ui
#        viewer.h viewer.cpp
#        qmywidget.h
        embedExternalApp.cpp embedExternalApp.h
        utils.cpp utils.h
        dialog_featurematch.cpp dialog_featurematch.h dialog_featurematch.ui
        dialog_sfm.cpp dialog_sfm.h dialog_sfm.ui
        dialog_densifypointcloud.cpp dialog_densifypointcloud.h dialog_densifypointcloud.ui
        dialog_trimeshrecon.cpp dialog_trimeshrecon.h dialog_trimeshrecon.ui
        dialog_texturemesh.cpp dialog_texturemesh.h dialog_texturemesh.ui
        global.cpp global.h
        message.hpp
        dialog_autorebuild.cpp dialog_autorebuild.h dialog_autorebuild.ui
        dialog_importvideoframes.cpp dialog_importvideoframes.h dialog_importvideoframes.ui
        dialog_addpictures.cpp dialog_addpictures.h dialog_addpictures.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(OpenScan3D
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET OpenScan3D APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(OpenScan3D SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(OpenScan3D
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(OpenScan3D PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(OpenScan3D PUBLIC ${X11_LIBRARIES})
target_include_directories(OpenScan3D PRIVATE ${EXIV2_INCLUDE_DIRS})

set_target_properties(OpenScan3D PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS OpenScan3D
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(OpenScan3D)
endif()

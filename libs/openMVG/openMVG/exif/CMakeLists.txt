
add_library(openMVG_exif exif_IO_EasyExif.cpp)
target_compile_features(openMVG_exif INTERFACE ${CXX11_FEATURES})
target_link_libraries(openMVG_exif LINK_PRIVATE openMVG_easyexif)
target_include_directories(openMVG_exif PRIVATE ${CMAKE_SOURCE_DIR}/libs/openMVG ${CMAKE_SOURCE_DIR}libs/openMVG/third_party/include/easyexif)
set_target_properties(openMVG_exif PROPERTIES SOVERSION ${OPENMVG_VERSION_MAJOR} VERSION "${OPENMVG_VERSION_MAJOR}.${OPENMVG_VERSION_MINOR}")
set_property(TARGET openMVG_exif PROPERTY FOLDER OpenMVG/OpenMVG)
install(TARGETS openMVG_exif DESTINATION ${CMAKE_INSTALL_LIBDIR} EXPORT openMVG-targets)


UNIT_TEST(openMVG exif_IO "openMVG_testing;${STLPLUS_LIBRARY};openMVG_exif")

add_subdirectory(sensor_width_database)

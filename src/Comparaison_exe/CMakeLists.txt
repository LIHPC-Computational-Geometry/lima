set (CURRENT_PACKAGE_NAME "Comparaison_exe")

include (${CMAKE_SOURCE_DIR}/cmake/version.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/common.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/workarounds.cmake)

file (GLOB CPP_SOURCES *.cpp)
add_executable (comparaison ${CPP_SOURCES})

target_compile_definitions (comparaison PRIVATE ${LIMA_CFLAGS})
target_include_directories (comparaison PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/protected)
target_link_libraries (comparaison PUBLIC Lima)

# INSTALL_RPATH modifie le rpath pour les libs internes au projet :
set_target_properties (comparaison PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1 INSTALL_RPATH ${CMAKE_PACKAGE_RPATH_DIR})




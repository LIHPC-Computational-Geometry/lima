set (CURRENT_PACKAGE_NAME "Symetrise")

include (${CMAKE_SOURCE_DIR}/cmake/version.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/common.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/workarounds.cmake)

file (GLOB CPP_SOURCES *.cpp)
add_executable (symetrise ${CPP_SOURCES})

target_compile_definitions (symetrise PRIVATE ${LIMA_CFLAGS})
target_include_directories (symetrise PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/protected)
target_link_libraries (symetrise PUBLIC Lima)

# INSTALL_RPATH modifie le rpath pour les libs internes au projet :
set_target_properties (symetrise PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1 INSTALL_RPATH ${CMAKE_PACKAGE_RPATH_DIR})

install(TARGETS symetrise EXPORT Symetrise DESTINATION ${CMAKE_INSTALL_BINDIR}
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

set (ConfigPackageLocation ${CMAKE_CMAKE_DIR})
install (EXPORT Symetrise FILE SymetriseTargets.cmake NAMESPACE ${PROJECT_NAMESPACE} DESTINATION ${ConfigPackageLocation})

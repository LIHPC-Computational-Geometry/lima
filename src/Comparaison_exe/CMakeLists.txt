set (CURRENT_PACKAGE_NAME "Comparaison")

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

install(TARGETS comparaison EXPORT Comparaison DESTINATION ${CMAKE_INSTALL_BINDIR}
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

set (ConfigPackageLocation ${CMAKE_CMAKE_DIR})
install (EXPORT Comparaison FILE ComparaisonTargets.cmake NAMESPACE ${PROJECT_NAMESPACE} DESTINATION ${ConfigPackageLocation})


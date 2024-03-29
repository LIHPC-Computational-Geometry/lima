set (CURRENT_PACKAGE_NAME "Lima")
set (CURRENT_PACKAGE_MACRO_NAME "LIMA_KERNEL")
include (${CMAKE_SOURCE_DIR}/cmake/version.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/common.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/compilation_options.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/workarounds.cmake)


if (MACHINE_TYPES)
	find_package (MachineTypes REQUIRED)
endif (MACHINE_TYPES)
if (FORMAT_MLI)
	include (${CMAKE_SOURCE_DIR}/cmake/FindHdf145.cmake)
	if (NOT HDF145_FOUND)
		message (FATAL_ERROR "Support du format mli demandé mais bibliothèque HDF145 non trouvée.")
	endif (NOT HDF145_FOUND)
endif (FORMAT_MLI)
unset (HDF5_USE_STATIC_LIBRARIES)
if (FORMAT_MLI2)
	find_package (HDF5 COMPONENTS CXX REQUIRED)
endif (FORMAT_MLI2)
if (SUMESH)
	find_package (Sumesh REQUIRED)
endif (SUMESH)
find_package (Threads)

file (GLOB HEADERS public/Lima/*.h public/LimaP/*.h)
file (GLOB PUBLIC_HEADERS public/Lima/*.h)
file (GLOB PRIVATE_HEADERS public/LimaP/*.h)
file (GLOB CPP_SOURCES *.cpp *.f90)

set (ICEM_SUPPORTED OFF)
if ((PLATFORM STREQUAL "CentOS") OR (PLATFORM STREQUAL "Atos_7__x86_64") OR (PLATFORM STREQUAL "Atos_7__mic") OR (PLATFORM STREQUAL "RedHat-7-x86_64") OR (PLATFORM STREQUAL "Rhel_8__x86_64") OR (PLATFORM STREQUAL "RedHat-8-x86_64") OR (PLATFORM STREQUAL "Ubuntu"))
	message (STATUS "======================> Ajout du support du format ICEM-CFD")
	file (GLOB ICEMCFD_OBJS ../icemcfd/RHEL_4__x86_64/*.o)
	set (ICEMCFD_FLAGS "__INTERNE_ICEMCFD")
	set (ICEM_SUPPORTED ON)
endif ()

add_library (Lima ${CPP_SOURCES} ${HEADERS} ${ICEMCFD_OBJS})
add_library (lima_fortran_compile_flags INTERFACE)

set (MLI_SUPPORTED OFF)
set (EXTERNAL_LIBS_FLAGS)
if (${HDF145_FOUND})
	message (STATUS "======================> Ajout du support du format mli")
	list (APPEND EXTERNAL_LIBS_FLAGS "HDF5_1_4_3")
	list (APPEND EXTERNAL_LIBS_FLAGS "-D__INTERNE_MALIPP")
	target_link_libraries (Lima PUBLIC HDF145::hdf145_cpp)
	set (MLI_SUPPORTED ON)
endif (${HDF145_FOUND})

set (MLI2_SUPPORTED OFF)
if (HDF5_FOUND AND HDF5_INCLUDE_DIRS AND HDF5_CXX_LIBRARIES AND (HDF5_VERSION VERSION_GREATER_EQUAL 1.10.0))
#message (STATUS " ========================================= HDF5_INCLUDE_DIRS=${HDF5_INCLUDE_DIRS} HDF5_CXX_LIBRARIES=${HDF5_CXX_LIBRARIES} HDF5_VERSION=${HDF5_VERSION} =========================================")
	message (STATUS "======================> Ajout du support du format mli2")
	list (APPEND EXTERNAL_LIBS_FLAGS "-D__INTERNE_MALIPP2")
# Pb de portabilité avec cmake/hdf ...	
#	target_link_libraries (Lima PUBLIC hdf5::hdf5_cpp-shared)
	target_include_directories (Lima PUBLIC ${HDF5_INCLUDE_DIRS})
	target_link_libraries (Lima PRIVATE ${HDF5_CXX_LIBRARIES})
	set (MLI2_SUPPORTED ON)
else (HDF5_FOUND AND HDF5_INCLUDE_DIRS AND HDF5_CXX_LIBRARIES AND (HDF5_VERSION VERSION_GREATER_EQUAL 1.10.0))
	message (WARNING "======================>  API HDF5 NON TROUVEE (ou version < 1.10.0 ou absence d'API c++) : pas de support du format mli2 <======================")
endif (HDF5_FOUND AND HDF5_INCLUDE_DIRS AND HDF5_CXX_LIBRARIES AND (HDF5_VERSION VERSION_GREATER_EQUAL 1.10.0))

list (APPEND LIMA_CFLAGS "-D${LIMA_INT_SIZE}")
list (APPEND LIMA_CFLAGS "-D${LIMA_REAL_SIZE}")
target_compile_options (lima_fortran_compile_flags INTERFACE "${FORTRAN_FLAGS_LIST}")

set (MachineTypes_ENABLED OFF)
if (${MachineTypes_FOUND})
	message (STATUS "======================> Utilisation de l'API MachineTypes")
	set (MachineTypes_ENABLED ON)	# Pour find_depency
	target_link_libraries (Lima PUBLIC MachineTypes::include)
	target_compile_definitions (Lima PUBLIC __INTERNE_MACHINE_TYPES)
	if (MachineTypes_INCLUDE_DIR)
		target_include_directories (Lima PUBLIC ${MachineTypes_INCLUDE_DIR})
	elseif (MachineTypes_INCLUDE_DIRS)
		target_include_directories (Lima PUBLIC ${MachineTypes_INCLUDE_DIRS})
	else ( )
		# Si il y a une demande d'utilisation de MachineTypes ce peut être très important, ça défini entre autres les types des variables transmis en arguments des fonctions
		# => Pas le droit à l'erreur, on bloque donc en cas de doute.
		message (FATAL_ERROR "Variables MachineTypes_INCLUDE_DIRS et MachineTypes_INCLUDE_DIRS nulles.")
	endif ( )
endif (${MachineTypes_FOUND})

set (Sumesh_ENABLED OFF)
if (${Sumesh_FOUND})
	message (STATUS "======================> Ajout du support de la bibliotheque sumesh")
	set (Sumesh_ENABLED ON)		# Pour find_depency
	list (APPEND EXTERNAL_LIBS_FLAGS "-D__INTERNE_LMSUMESH")
	target_link_libraries (Lima PRIVATE Sumesh::sumesh)
endif (${Sumesh_FOUND})


set (LIMA_KERNEL_PUBLIC_FLAGS "${LIMA_CFLAGS}" LIMA_MT LIMA_USES_ATOMIC_API)
#set (LIMA_KERNEL_PRIVATE_FLAGS "${CMAKE_CXX_FLAGS}")
set (ALL_TARGETS Lima lima_fortran_compile_flags)
set_property (TARGET Lima PROPERTY VERSION ${LIMA_VERSION})
set_property (TARGET Lima PROPERTY SOVERSION ${LIMA_MAJOR_VERSION})
target_include_directories (Lima PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)
if (ICEM_SUPPORTED)
	target_include_directories (Lima PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../icemcfd/include)
endif (ICEM_SUPPORTED)
target_compile_definitions (Lima PUBLIC ${LIMA_KERNEL_PUBLIC_FLAGS})
target_compile_options (Lima PRIVATE ${LIMA_KERNEL_PRIVATE_FLAGS})
target_compile_definitions (Lima PUBLIC ${ICEMCFD_FLAGS})
target_compile_definitions (Lima PUBLIC ${EXTERNAL_LIBS_FLAGS})
if (DISABLE_MLI_WARNING)
	target_compile_definitions (Lima PRIVATE "DISABLE_MLI_WARNING")
endif (DISABLE_MLI_WARNING)
# v 7.10.0 : Lima a au moins 1 appel direct à la lm (pow), et HDF145 en a aussi (ceil), mais HDF145 ne déclare pas de dépendance à la lm. Dans de rares cas ça manque au linker ...
# Donc la dépendance ci-dessous est à laisser.
target_link_libraries (Lima PUBLIC Threads::Threads m)	 


# Etre capable une fois installée de retrouver Lima*, ... :
# (Rem : en son absence on a Set runtime path of "/tmp/pignerol/install/lib/libLima.so.5.0.0" to "") ...
set_target_properties (Lima PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1)

# INSTALLATION :
include(CMakePackageConfigHelpers)
# Génération du fichier contenant les informations sur la distribution :
configure_file (cmake/LimaDistribution.h.in  ${CMAKE_CURRENT_BINARY_DIR}/LimaDistribution.tmp)
configure_file (${CMAKE_CURRENT_BINARY_DIR}/LimaDistribution.tmp  ${CMAKE_CURRENT_BINARY_DIR}/LimaDistribution.h)
 
# ConfigPackageLocation : c'est plus ou moins standardisé, le défaut étant lib/cmake.
# On nos recommande très vivement d'utiliser ce répertoire afin de limiter les soucis par la suite,
# notamment au niveau des vues.
set (ConfigPackageLocation ${CMAKE_CMAKE_DIR})

# La génération des règles d'installation :
install (TARGETS ${ALL_TARGETS} EXPORT ${CMAKE_PACKAGE_TARGETS_NAME}
         ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
         LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
         RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
         INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Recopie des en-têtes dans le répertoire d'installation :
install(FILES ${PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Lima)
install(FILES ${PRIVATE_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/LimaP)
# Dont LimaDistribution.h généré plus haut :
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/LimaDistribution.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Lima)

# Le fichier contenant les informations de version de ce module :
write_basic_package_version_file(${CMAKE_PACKAGE_VERSION_FILE} VERSION ${LIMA_VERSION} COMPATIBILITY SameMinorVersion)
export (EXPORT ${CMAKE_PACKAGE_TARGETS_NAME} FILE ${CMAKE_PACKAGE_TARGETS_FILE} NAMESPACE ${CMAKE_MODULE_NAMESPACE})

# Utilisation par d'autres modules de Lima :
set (INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR})	# Lima_INCLUDE_DIR avec LimaConfig.cmake.in
set (LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})			# Lima_LIB_DIR avec LimaConfig.cmake.in
set (HAS_LIMA_SCRIPTING ${BUILD_SCRIPTING})				# Python/Swig wrapper
set (HAS_LIMA_TOOLS ${BUILD_XLMLIMA})					# XlmLima, Symetrise
set (HAS_LIMA_TESTS ${BUILD_TESTS})						# Comparaison

#set (RUNTIME_INSTALL_DIR ${CMAKE_INSTALL_BINDIR})		# Lima_RUNTIME_DIR avec LimaConfig.cmake.in
# Création du fichier LimaConfig.cmake qui définira Lima_INCLUDE_DIR, Lima_LIB_DIR et Lima_BIN_DIR :
configure_package_config_file (cmake/${CMAKE_PACKAGE_CONFIG_NAME}.cmake.in ${CMAKE_PACKAGE_CONFIG_FILE}
	INSTALL_DESTINATION ${CMAKE_CMAKE_DIR} PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR ICEM_SUPPORTED MLI_SUPPORTED MLI2_SUPPORTED MachineTypes_ENABLED Sumesh_ENABLED 
	HAS_LIMA_SCRIPTING HAS_LIMA_TOOLS HAS_LIMA_TESTS
	)

# Copie du fichier LimaTargets.cmake dans prefix/lib/Lima/cmake :
install (EXPORT ${CMAKE_PACKAGE_TARGETS_NAME}
	FILE ${CMAKE_PACKAGE_TARGETS_BASENAME} NAMESPACE ${CMAKE_MODULE_NAMESPACE}
	DESTINATION ${ConfigPackageLocation} PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ)
# Copie des fichiers LimaConfig.cmake et LimaConfigVersion.cmake dans prefix/lib/Lima/cmake :
install (FILES ${CMAKE_PACKAGE_CONFIG_FILE} ${CMAKE_PACKAGE_VERSION_FILE} DESTINATION ${ConfigPackageLocation} PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ)

# Le fichier FindHDF5.cmake :
install(FILES ${CMAKE_SOURCE_DIR}/cmake/FindHdf145.cmake DESTINATION ${ConfigPackageLocation} PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ)



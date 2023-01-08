set (CURRENT_PACKAGE_NAME "LimaScripting")
set (CURRENT_PACKAGE_MACRO_NAME "LIMA_SCRIPTING")

include (${CMAKE_SOURCE_DIR}/cmake/version.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/common.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/python_binding.cmake)
#include (${CMAKE_SOURCE_DIR}/cmake/workarounds.cmake)

file (GLOB HEADERS public/${CURRENT_PACKAGE_NAME}/*.h)
file (GLOB CPP_SOURCES *.i *.cpp)

# LimaScripting est la cible "binding swig" => donnera _LimaScripting.so et LimaScripting.py. 
# Dans LimaScripting.py on aura import.find_module('_LimaScripting', [dirname(__file__)])
# => le nom du module (ici nom de la cible préfixé) est utilisé un peu partout (nom des fichiers, mais également
# par les sources du binding).
# On ne cherche plus à obtenir de libLimaScripting, de quelque manière que ce soit :
# - L'objectif est d'utiliser les facilités apportées par CMake (donc find_package (SWIG 3)/SWIG_USE_FILE)
# - Ces facilités fournissent de base _LimaScripting (dont le seul regret est de ne pas avoir libLimaScripting)
# - Renommer _LimaScripting en libLimaScripting (via les PROPERTIES SWIG_MODULE_NAME et PREFIX "lib") ne fonctionne pas complétement
# bien. Dans le fichier python généré on a un import_module ("_LimaScripting") ou import_module ("libLimaScripting")
# selon l'ordre de création des cibles ... (LimaScripting.py est créé 2 fois, une pour _LimaScripting, une pour libLimaScripting)
# - Avoir un lien libLimaScripting -> _LimaScripting (ou inverse) ne marche pas forcément très bien car le soname
# n'est pas le même que le nom de l'archive, et du coup le fichier recherché au final ne sera pas forcément dans le RPATH ...
# => dans cette version on ne génère que _LimaScripting qui sera utilisé directement depuis les scripts comme par les exécutables.
set (ALL_TARGETS LimaScripting)
set_property (SOURCE LimaScripting.i PROPERTY CPLUSPLUS ON)
swig_add_library (LimaScripting TYPE SHARED LANGUAGE PYTHON SOURCES ${CPP_SOURCES})
cmake_policy (SET CMP0086 NEW)
set_source_files_properties (LimaScripting.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties (LimaScripting.i PROPERTIES SWIG_MODULE_NAME LimaScripting)

# Objectif avoir les flags de compilation SWIG suivants :
# -globals LimaScripting -shadow -DIN_STD="std::" -DSWIG_FORBIDS_FINALIZATION -DIN_UTIL="Lima::" -DIN_STD="std::"
# Ne marche pas sans le -c++ malgré les CPLUSPLUS ON avant le swig_add_library.
set (MT_SWIG_COMPILE_OPTIONS "-c++;-module;LimaScripting;-globals;LimaScripting;-shadow;-DIN_STD=\"std::\";-DSWIG_FORBIDS_FINALIZATION;-DIN_STD=\"std::\"")
foreach (flag ${LIMA_CFLAGS})
	list (APPEND MT_SWIG_COMPILE_OPTIONS ${flag})
endforeach (flag)
# Avec cmake_policy (SET CMP0086 NEW) -module ${SWIG_MODULE_NAME} devrait être automatiquement passé, SWIG_MODULE_NAME étant défini comme propriété du fichier swig. Mais ...
#set (MT_SWIG_COMPILE_OPTIONS "-c++;-globals;LimaScripting;-shadow;-DIN_STD=\"std::\";-DSWIG_FORBIDS_FINALIZATION;-DIN_UTIL=\"Lima::\";-DIN_STD=\"std::\"")
swig_link_libraries (LimaScripting PUBLIC Lima ${Python_LIBRARIES})
foreach (lib ${ALL_TARGETS})
#   target_include_directories (${lib} PRIVATE ${Python2_INCLUDE_DIRS})	# Ne devrait pas être nécesaire, non ? C'est pour le wrapper swig ...
   target_include_directories (${lib} PRIVATE ${Python_INCLUDE_DIRS})
   target_compile_options (${lib} PUBLIC ${MANDATORY_CXX_OPTIONS})
   set_property (TARGET ${lib} PROPERTY VERSION ${LIMA_VERSION})
   set_property (TARGET ${lib} PROPERTY SOVERSION ${LIMA_MAJOR_VERSION})
   set_property (TARGET ${lib} PROPERTY SWIG_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/src/Lima/public)
   set_property (TARGET ${lib} PROPERTY SWIG_COMPILE_OPTIONS ${MT_SWIG_COMPILE_OPTIONS})
# INSTALL_RPATH modifie le rpath pour les libs internes au projet :
# Etre capable une fois installée de retrouver les libs utilisées (iconv ?), ... :
message ("INSTALL RPATH=" "${CMAKE_PACKAGE_RPATH_DIR};${CMAKE_PYTHON_RPATH_DIR}")
message ("INSTALL RPATH=" "${CMAKE_PACKAGE_RPATH_DIR};${CMAKE_PYTHON_RPATH_DIR};$ORIGIN/../${PYTHON_BINDING_DIR}")
#   set_target_properties (${lib} PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1 INSTALL_RPATH "${CMAKE_PACKAGE_RPATH_DIR};${CMAKE_PYTHON_RPATH_DIR}")
   set_target_properties (${lib} PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1 INSTALL_RPATH "${CMAKE_PACKAGE_RPATH_DIR};$ORIGIN/../${PYTHON_BINDING_DIR}")
endforeach (lib)

# INSTALLATION :
include(CMakePackageConfigHelpers)
# Génération du fichier contenant les informations sur la distribution :
configure_file (cmake/LimaScriptingDistribution.h.in  ${CMAKE_CURRENT_BINARY_DIR}/LimaScriptingDistribution.tmp)
configure_file (${CMAKE_CURRENT_BINARY_DIR}/LimaScriptingDistribution.tmp  ${CMAKE_CURRENT_BINARY_DIR}/LimaScriptingDistribution.h)

# ConfigPackageLocation : c'est plus ou moins standardisé, le défaut étant lib/cmake.
# On nos recommande très vivement d'utiliser ce répertoire afin de limiter les soucis par la suite,
# notamment au niveau des vues.
set (ConfigPackageLocation ${CMAKE_CMAKE_DIR})

# Création du fichier LimaConfig.cmake qui définira LimaScripting_INCLUDE_DIR, LimaScripting_LIB_DIR et LimaScripting_RUNTIME_DIR (_LimaScripting.so et .py) :
# On fait ici une installation "inhabituelle". S'agissant d'un binding python il est installé dans /install_prefix/lib/python/site-packages
# Mais comme on en veut une lib également (_LimaScripting.so), pour les logiciels, on installe également la version bibliothèque (via un lien) dans le répertoire standard.
# Installation de la bibliothèque :
install (TARGETS LimaScripting EXPORT ${CMAKE_PACKAGE_TARGETS_NAME} 
         LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
         INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_PACKAGE_NAME}
         )

# Recopie des en-têtes dans le répertoire d'installation :
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_PACKAGE_NAME}
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ)
# Dont LimaScriptingDistribution.h généré plus haut :
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/LimaScriptingDistribution.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_PACKAGE_NAME}
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ)

# Installation du binding python :
# Recopie du fichier binding python (=> création de PYTHON_BINDING_DIR utilisé après par install) :
install (PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/LimaScripting.py DESTINATION ${PYTHON_BINDING_DIR}
         PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
# Le lien lib/_LimaScripting.so.x.y.z vers lib/python/site-packages/_LimaScripting.so.x.y.z n'est pas bon car ils ne sont plus dans le même répertoire ...
install (CODE "execute_process (COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/_LimaScripting.so.${LIMA_VERSION} ${CMAKE_INSTALL_PREFIX}/${PYTHON_BINDING_DIR}/_LimaScripting.so.${LIMA_VERSION})")
install (CODE "execute_process (COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_INSTALL_PREFIX}/${PYTHON_BINDING_DIR}/_LimaScripting.so.${LIMA_VERSION} ${CMAKE_INSTALL_PREFIX}/${PYTHON_BINDING_DIR}/_LimaScripting.so.${LIMA_MAJOR_VERSION})")
install (CODE "execute_process (COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_INSTALL_PREFIX}/${PYTHON_BINDING_DIR}/_LimaScripting.so.${LIMA_MAJOR_VERSION} ${CMAKE_INSTALL_PREFIX}/${PYTHON_BINDING_DIR}/_LimaScripting.so)")

# Le fichier contenant les informations de version de ce module :
write_basic_package_version_file (${CMAKE_PACKAGE_VERSION_FILE} VERSION ${LIMA_VERSION} COMPATIBILITY SameMajorVersion)
export (EXPORT ${CMAKE_PACKAGE_TARGETS_NAME} FILE ${CMAKE_PACKAGE_TARGETS_FILE} NAMESPACE ${CMAKE_MODULE_NAMESPACE})

# Utilisation par d'autres modules de LimaScripting :
set (INCLUDE_INSTALL_DIR include/)					# LimaScripting_INCLUDE_DIR avec LimaScriptingConfig.cmake.in
set (LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})		# Lima_LIB_DIR avec LimaConfig.cmake.in
set (RUNTIME_INSTALL_DIR ${PYTHON_BINDING_DIR}/)	# LimaScripting_RUNTIME_DIR avec LimaScriptingConfig.cmake.in 
# Création du fichier LimaScriptingConfig.cmake qui définira LimaScripting_INCLUDE_DIR, LimaScripting_LIB_DIR et LimaScripting_RUNTIME_DIR :
configure_package_config_file (cmake/${CMAKE_PACKAGE_CONFIG_NAME}.cmake.in ${CMAKE_PACKAGE_CONFIG_FILE}
	INSTALL_DESTINATION ${CMAKE_CMAKE_DIR}
	PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR RUNTIME_INSTALL_DIR PYTHON_MAJOR_VERSION)

# Copie du fichier LimaScriptingTargets.cmake dans prefix/lib/LimaScripting/cmake :
install (EXPORT ${CMAKE_PACKAGE_TARGETS_NAME}
	FILE ${CMAKE_PACKAGE_TARGETS_BASENAME} NAMESPACE ${CMAKE_MODULE_NAMESPACE}
	DESTINATION ${ConfigPackageLocation}
	PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ
	)

# Copie des fichiers LimaScriptingConfig.cmake et LimaScriptingConfigVersion.cmake dans prefix/lib/LimaScripting/cmake :
install (
   FILES ${CMAKE_PACKAGE_CONFIG_FILE} ${CMAKE_PACKAGE_VERSION_FILE}
   DESTINATION ${ConfigPackageLocation}
   PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ
   )

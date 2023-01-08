include (GNUInstallDirs)
find_package (SWIG 3 REQUIRED)

include (${SWIG_USE_FILE})

#find_package (Python REQUIRED COMPONENTS Interpreter Development)	# Rem : Python3 a la priorité => inutilisé car empêche l'accès à Python2
if (NOT USE_PYTHON_3)
	find_package (Python2 REQUIRED COMPONENTS Interpreter Development)
	set (PYTHON_BINDING_DIR ${CMAKE_INSTALL_LIBDIR}/python${Python2_VERSION_MAJOR}.${Python2_VERSION_MINOR}/site-packages/)
	set (Python_INCLUDE_DIRS ${Python2_INCLUDE_DIRS})
	set (PYTHON_MAJOR_VERSION 2)
else ( )
	find_package (Python3 REQUIRED COMPONENTS Interpreter Development)
	set (PYTHON_BINDING_DIR ${CMAKE_INSTALL_LIBDIR}/python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}/site-packages/)
	set (Python_INCLUDE_DIRS ${Python3_INCLUDE_DIRS})
	set (PYTHON_MAJOR_VERSION 3)
endif (NOT USE_PYTHON_3)

# Répertoire d'installation des modules (pour le RPATH) :
set (CMAKE_PYTHON_RPATH_DIR ${CMAKE_INSTALL_PREFIX}/${PYTHON_BINDING_DIR})


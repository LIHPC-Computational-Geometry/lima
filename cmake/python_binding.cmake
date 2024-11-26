# Version 0.7 (26/11/24, support Python 2/Python 3, répertoire d'installation des modules python fourni par le python utilisé)

# On utilise Python 3 sauf si python 2 est demandé

include (GNUInstallDirs)
find_package (SWIG 3 REQUIRED)


#find_package (Python REQUIRED COMPONENTS Interpreter Development)	# Rem : Python3 a la priorité => inutilisé car empêche l'accès à Python2
if (USE_PYTHON_2)
	message (STATUS "========================================= UTILISATION DE PYTHON 2 =========================================")
	find_package (Python2 REQUIRED COMPONENTS Interpreter Development)
	set (Python_INCLUDE_DIRS ${Python2_INCLUDE_DIRS})
	set (Python_EXECUTABLE ${Python2_EXECUTABLE})
	set (Python_VERSION ${Python2_VERSION})
	set (Python_LIBRARIES ${Python2_LIBRARIES})
	set (Python_LIBRARY_DIRS ${Python2_LIBRARY_DIRS})
	set (PYTHON_MAJOR_VERSION 2)
else ( )
	message (STATUS "========================================= UTILISATION DE PYTHON 3 =========================================")
	set (USE_PYTHON_3 ON)
	set (Python3_FIND_STRATEGY LOCATION)	# Nécessaire pour python >= 3.10
	find_package (Python3 REQUIRED COMPONENTS Interpreter Development)
	set (Python_INCLUDE_DIRS ${Python3_INCLUDE_DIRS})
	set (Python_EXECUTABLE ${Python3_EXECUTABLE})
	set (Python_VERSION ${Python3_VERSION})
	set (Python_LIBRARIES ${Python3_LIBRARIES})
	set (Python_LIBRARY_DIRS ${Python3_LIBRARY_DIRS})
	set (PYTHON_MAJOR_VERSION 3)
endif (USE_PYTHON_2)


# Recherche du répertoire d'installation des modules (procédure spack organizer) : 
# get python installation path
macro (_set_from_python outvar python_code)
	if (NOT ${outvar})
		execute_process (
			COMMAND ${Python_EXECUTABLE} -c "${python_code}"
			OUTPUT_VARIABLE ${outvar}
			OUTPUT_STRIP_TRAILING_WHITESPACE)
		if (NOT ${outvar})
			message (FATAL_ERROR "Failed to detect ${outvar} from command:${Python_EXECUTABLE} -c ${python_code}")
		endif ( )
	endif ( )
endmacro ( )

if (USE_PYTHON_3)	# ATTENTION, ne marche peut être pas pour 3.0 <= python < 3.12. Le cas échéant la commande du else doit convenir.
	_set_from_python (_GET_PYTHON_SITEARCH "import os, sys, sysconfig; sitepackages=os.path.relpath (sysconfig.get_path('platlib'), sys.base_prefix); sys.stdout.write (sitepackages)")
else (USE_PYTHON_3)
	_set_from_python (_GET_PYTHON_SITEARCH "import sys; from distutils import sysconfig; sys.stdout.write (sysconfig.get_python_lib (plat_specific=True, standard_lib=False, prefix=''))")
endif (USE_PYTHON_3)

set (PYTHON_BINDING_DIR ${_GET_PYTHON_SITEARCH})
set (CMAKE_PYTHON_RPATH_DIR ${CMAKE_INSTALL_PREFIX}/${_GET_PYTHON_SITEARCH})

include (${SWIG_USE_FILE})

message (STATUS "========================================= VARIABLES LIEES A PYTHON =========================================")
message (STATUS "PYTHON_BINDING_DIR=${PYTHON_BINDING_DIR}")
message (STATUS "Python_INCLUDE_DIRS=${Python_INCLUDE_DIRS}")
message (STATUS "Python_EXECUTABLE=${Python_EXECUTABLE}")
message (STATUS "Python_VERSION=${Python_VERSION} PYTHON_MAJOR_VERSION=${PYTHON_MAJOR_VERSION}")
message (STATUS "Python_LIBRARIES=${Python_LIBRARIES}")
message (STATUS "Python_LIBRARY_DIRS=${Python_LIBRARY_DIRS}")
message (STATUS "PYTHON_BINDING_DIR=${PYTHON_BINDING_DIR}")
message (STATUS "CMAKE_PYTHON_RPATH_DIR=${CMAKE_PYTHON_RPATH_DIR}")
message (STATUS "============================================================================================================")







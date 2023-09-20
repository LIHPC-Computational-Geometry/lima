cmake_minimum_required (VERSION 3.20)

project (Lima CXX C Fortran)

include (GNUInstallDirs)
include (cmake/version.cmake)
include (cmake/common.cmake)

set (Lima_VERSION ${LIMA_VERSION})
set (PROJECT_NAMESPACE "${PROJECT_NAME}::")

option (VERBOSE_MAKEFILE ON)
option (BUILD_SHARED_LIBS "Creation de bibliotheques dynamiques (defaut : ON)" ON)

# Divers dépendances optionnelles :
option (MACHINE_TYPES "Définition des types numériques simples." ON)
option (SUMESH "Utilisation de la bibliothèque de sumesh." ON)

# Les formats de fichiers optionnellement supportés :
option (FORMAT_MLI "Format obsolète déconseillé en externe. Requiert HDF145." ON)
option (FORMAT_MLI2 "Format remplaçant le format MLI recommandé partout. Requiert HDF 5 v >= 1.10.0." ON)

# Binaires optionnels :
option (BUILD_SCRIPTING "Génération du wrapper swig/python pour utilisation scriptée de Lima." ON)
option (BUILD_XLMLIMA "Utilitaire xlmlima (conversions, préparations de maillages), symetrise (symétrisation / x, y ou z)." ON)
option (BUILD_TESTS "Création de l'utilitaire comparaison." OFF)

add_subdirectory (src)
enable_testing ( )

#!/bin/sh

# Exemple d'utilisation :
#
# Installation dans /opt/install de la bibliothèque compilée en I4.R8 avec Xlmlima + tests :
#
# Lima/configurations/build.sh -installdir /tmp/install_lima -builddir /tmp/build_lima -toolchain ~/Lima/configurations/Ubuntu_GNU11.cmake -meshparameters ~/Lima/configurations/Ubuntu_GNU11_external_meshlibs.cmake -i8 OFF -r8 ON -python 3
#
# ATTENTION : les chemins donnés doivent être en absolus
#

# ATTENTION : S'ASSURER QUE LA VERSION UTILISEE DE CMAKE EST >= 3.14

# # =============================================================================================================================================
# Pour une installation en production : -debug OFF -production ON
# Pour une installation en mode développement/tests de non régression : -debug OFF -production OFF
# Pour une installation en mode développement : -debug ON -production OFF
# =============================================================================================================================================

# Rem : -v affiche les lignes au fur et à mesure qu'elles sont exécutées.
#set -eo pipefail

libs='Lima'


# Paramètres par défaut :
# =======================

if [ "$TMPDIR" = "" ]; then
	TMPDIR=/tmp/"$USER"
fi
INSTALLDIR="$TMPDIR"
BUILDDIR="$TMPDIR"
DEBUG=OFF
PRODUCTION=ON
NBPROCS=16
PYTHONVERSION=3

# Spécificités application :
INT8=OFF
REAL8=ON

# Paramètres utilisés :
# =====================

# On évite PWD qui peut varier selon la façon d'accéder au répertoire, surtout si il y a des liens ...
SOURCES_DIR=`realpath .`
INSTALL_DIR="$INSTALLDIR"
BUILD_DIR="$BUILDDIR"
TOOLCHAIN_FILE=""
MESHPARAMETERS_FILE=""
ENABLE_DEBUG="$DEBUG"
ENABLE_PRODUCTION="$PRODUCTION"
NB_PROCS="$NBPROCS"
PYTHON_VERSION="$PYTHONVERSION"
# Spécificités application :
INT_8="$INT8"
REAL_8="$REAL8"


# Fonctions :
# ===========

usage ( )
{
	echo ""$0" -tmpdir dir -destdir directory -toolchain toolchain.cmakefile -graphicparameters graphicparameters.cmakefile -meshparameters meshparameters.cmake [-procnum num]"
	echo "-installdir .................... : répertoire où seront installées toutes les dépendances de ce logiciel. Défaut : "$INSTALLDIR""
	echo "-builddir ...................... : répertoire où sera installé le logiciel (sans ses dépendances => faire éventuellement -installdir et -builddir avec le même répertoire). Défaut : "$BUILDDIR""
	echo "-production .................... : compilation pour déploiement en production (ON/OFF). Défaut : "$DEBUG" (influence les fichiers d'environnement - binding python utilisés)"
	echo "-debug ......................... : compilation en mode debug (ON/OFF). Défaut : "$DEBUG""
	echo "-nbprocs ....................... : nombre de processeurs à utiliser. Défaut : "$NBPROCS""
	echo "-toolchain ..................... : fichier cmake contenant la déclaration des compilateurs utilisés."
	echo "-meshparameters ................ : fichier cmake contenant la déclaration des variables cmake propres au maillage."
# Spécificités applications :
	echo "-i8 ............................ : entiers sur 8 bits (ON/OFF). Défaut : "$INT8""
	echo "-r8 ............................ : floats sur 8 bits (ON/OFF). Défaut : "$REAL8""
	echo "-python ........................ :  (2/3). Défaut : "$PYTHON_VERSION""
}	# usage


parse_args ( )
{
	echo "=====================> EXECUTION DU SCRIPT "$0" TRAITEMENT DES PARAMETRES TRANSMIS A LA LIGNE DE COMMANDE :"
		while [ "$1" != "" ]; do
		case $1 in
			-h | -help | --help)	usage
						exit
						;;
			-installdir)		shift
						INSTALL_DIR=$1
						;;
			-builddir)		shift
						BUILD_DIR=$1
						;;
			-production)	shift
                        ENABLE_PRODUCTION=$1
                        ;;
			-debug)			shift
                        ENABLE_DEBUG=$1
                        ;;
            -nbprocs)		shift
						NB_PROCS=$1
						;;
			-toolchain)		shift
						TOOLCHAIN_FILE=$1
						;;
			-meshparameters)	shift
						MESHPARAMETERS_FILE=$1
						;;
			-i8) shift
						INT_8=$1
						;;
			-r8) shift
						REAL_8=$1
						;;
			-python) shift
						PYTHON_VERSION=$1
						;;
			* )			echo "Erreur, argument inconnu : $1"
						usage
						exit 1
		esac
		shift
	done

	echo "=====================> EXECUTION DU SCRIPT "$0" TRAITEMENT DES PARAMETRES TRANSMIS A LA LIGNE DE COMMANDE EFFECTUE"
}	# parse_args


validate_files ( )
{
	echo "=====================> VALIDATION DE L'EXISTENCE DES FICHIERS DE PARAMETRAGE ..."
	if [ ! -e $TOOLCHAIN_FILE ]
	then
		echo "Fichier de variables cmake contenant la déclaration des compilateurs utilisés invalide ($TOOLCHAIN_FILE)."
		exit -1
	fi
	if [ ! -e $OSPARAMETERS_FILE ]
	then
		echo "Fichier de variables cmake contenant la déclaration déclaration des variables cmake propres à cet environnement invalide ($OSPARAMETERS_FILE)."
		exit -1
	fi
	if [ ! -e $MESHPARAMETERS_FILE ]
	then
		echo "Fichier de variables cmake propres au maillage invalide ($MESHPARAMETERS_FILE)."
		exit -1
	fi
	echo "=====================> EXISTENCE DES FICHIERS DE PARAMETRAGE VALIDEE"
}	# validate_files


# Paramètres composés utilisés :
# ==============================

parse_args $*
validate_files

INT_4=ON
if [ $INT_8 = ON ]
then
	INT_4=OFF
fi
REAL_4=ON
if [ $REAL_8 = ON ]
then
	REAL_4=OFF
fi
if [ $PYTHON_VERSION != "2" ] && [ $PYTHON_VERSION != "3" ]
then
	echo "Version de python invalide : $PYTHON_VERSION. Version attendue : 2 ou 3."
	exit -1
fi

COMMON_ARGS="-DBUILD_SHARED_LIBS:BOOL=ON -DINT_8:BOOL=$INT_8 -DREAL_8:BOOL=$REAL_8"	# Toujours en mode shared, pour le scripting
if [ $PYTHON_VERSION = 3 ]
then
	COMMON_ARGS="$COMMON_ARGS -DUSE_PYTHON_3:BOOL=ON"
fi
LIMA_ARGS="-DBUILD_XLMLIMA:BOOL=ON -DBUILD_TESTS:BOOL=ON -DBUILD_SCRIPTING:BOOL=ON -DMACHINE_TYPES:BOOL=OFF -DSUMESH:BOOL=OFF"
CMAKE_CFLAGS_ARGS='-DUSING_NAMESPACES'
if [ $ENABLE_DEBUG = ON ]
then
	PRODUCTION_MODE="Debug"
else
	PRODUCTION_MODE="Release"
fi

# Spécifier le nombre de processeurs au build :
export CMAKE_BUILD_PARALLEL_LEVEL="$NB_PROCS"

# Les bibliothèques et produits maison sous cmake :
for lib in $libs
do
	echo "==> PREPARING " $lib " ..."
	echo "COMMON ARGS="$COMMON_ARGS
	echo "ADDITIONNAL_LIBS_ARGS="$SABRE_COMMON_ARGS
	LIB_BUILD_DIR=$BUILD_DIR/$lib
	umask u=rwx,g=rwx,o=rx; mkdir -p $LIB_BUILD_DIR
	umask u=rwx,g=rwx,o=rx; cd $LIB_BUILD_DIR; cmake -DCMAKE_BUILD_TYPE:STRING="$PRODUCTION_MODE" -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR" \
	                                                 -DCMAKE_PREFIX_PATH="$INSTALL_DIR" -DCMAKE_VERBOSE_MAKEFILE=ON -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
	                                                 -C"$MESHPARAMETERS_FILE" \
	                                                 -DPRODUCTION="$ENABLE_PRODUCTION" \
	                                                 $COMMON_ARGS $LIMA_ARGS $ADDITIONNAL_LIBS_ARGS -DEXTRA_CXX_FLAGS="$CMAKE_CFLAGS_ARGS" \
	                                                 -S $SOURCES_DIR/$lib -B . ; cmake --build $LIB_BUILD_DIR; make install
	echo "==> " $lib " PREPARED."
done	# for lib in $libs

